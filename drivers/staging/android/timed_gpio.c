/* drivers/misc/timed_gpio.c
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/hrtimer.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>

#include "timed_output.h"
#include "timed_gpio.h"


struct timed_gpio_data {
	struct timed_output_dev dev;
	struct hrtimer timer;
	spinlock_t lock;
	unsigned gpio;
	int max_timeout;
	u8 active_low;
};

static enum hrtimer_restart gpio_timer_func(struct hrtimer *timer)
{
	struct timed_gpio_data *data =
		container_of(timer, struct timed_gpio_data, timer);

	gpio_direction_output(data->gpio, data->active_low ? 1 : 0);
	return HRTIMER_NORESTART;
}

static int gpio_get_time(struct timed_output_dev *dev)
{
	struct timed_gpio_data	*data =
		container_of(dev, struct timed_gpio_data, dev);

	if (hrtimer_active(&data->timer)) {
		ktime_t r = hrtimer_get_remaining(&data->timer);
		struct timeval t = ktime_to_timeval(r);
		return t.tv_sec * 1000 + t.tv_usec / 1000;
	} else
		return 0;
}

static void gpio_enable(struct timed_output_dev *dev, int value)
{
	struct timed_gpio_data	*data =
		container_of(dev, struct timed_gpio_data, dev);
	unsigned long	flags;

	spin_lock_irqsave(&data->lock, flags);

	/* cancel previous timer and set GPIO according to value */
	hrtimer_cancel(&data->timer);
	gpio_direction_output(data->gpio, data->active_low ? !value : !!value);

	if (value > 0) {
		if (value > data->max_timeout)
			value = data->max_timeout;

		hrtimer_start(&data->timer,
			ktime_set(value / 1000, (value % 1000) * 1000000),
			HRTIMER_MODE_REL);
	}

	spin_unlock_irqrestore(&data->lock, flags);
}

#ifdef CONFIG_USE_OF
static int timed_gpio_get_devtree_pdata(struct platform_device *pdev,
                struct timed_gpio_platform_data *pdata)
{
	struct device_node *node, *pp;
	int i;
	struct timed_gpio *gpios;
	u32 reg;

	node = pdev->dev.of_node;
	if (node == NULL)
		return -ENODEV;

	memset(pdata, 0, sizeof *pdata);
	/* First count the subnodes */
	pdata->num_gpios = 0;
	pp = NULL;
	while ((pp = of_get_next_child(node, pp)))
		pdata->num_gpios++;

	if (pdata->num_gpios == 0)
		return -ENODEV;

	gpios = kzalloc(pdata->num_gpios * (sizeof *gpios), GFP_KERNEL);
	if (!gpios)
		return -ENOMEM;

	pp = NULL;
	i = 0;
	while ((pp = of_get_next_child(node, pp))) {

		gpios[i].name=of_get_property(node,"name",NULL);
		if (of_property_read_u32(pp, "gpio", &reg)) {
			printk(KERN_ERR "%s:Read node gpio failed\n",__func__);
			goto out_fail;
        }
		gpios[i].gpio=reg;

		if (of_property_read_u32(pp, "max_timeout", &reg)) {
			printk(KERN_ERR "%s:Read node max_timeout failed\n",__func__);
			goto out_fail;
        }
		gpios[i].max_timeout=reg;

		if (of_property_read_u32(pp, "active_low", &reg)) {
            printk(KERN_ERR "%s:Read node active_low failed\n",__func__);
			goto out_fail;
        }
		gpios[i].active_low=reg;

		//printk(KERN_INFO"gpios[%d]:\n name:%s\n gpio:%d\n max_timeout:%d\n active_low:%d",
		//				i,gpios[i].name, gpios[i].gpio,gpios[i].max_timeout,gpios[i].active_low);

		i++;
	}

	pdata->gpios=gpios;
	return 0;

out_fail:
	kfree(gpios);
	return -ENODEV;
}
#else
static int timed_gpio_get_devtree_pdata(struct platform_device *pdev,
                struct timed_gpio_platform_data *pdata)
{
	return -ENODEV;
}
#endif

static int timed_gpio_probe(struct platform_device *pdev)
{
	struct timed_gpio_platform_data *pdata = pdev->dev.platform_data;
	struct timed_gpio *cur_gpio;
	struct timed_gpio_data *gpio_data, *gpio_dat;
	struct timed_gpio_platform_data alt_pdata;
	int i, ret;
	int error;

	if (!pdata) {
		error = timed_gpio_get_devtree_pdata(pdev, &alt_pdata);
        if (error)
			return -EBUSY;
		pdata = &alt_pdata;
	}

	gpio_data = kzalloc(sizeof(struct timed_gpio_data) * pdata->num_gpios,
			GFP_KERNEL);
	if (!gpio_data)
		return -ENOMEM;

	for (i = 0; i < pdata->num_gpios; i++) {
		cur_gpio = &pdata->gpios[i];
		gpio_dat = &gpio_data[i];

		hrtimer_init(&gpio_dat->timer, CLOCK_MONOTONIC,
				HRTIMER_MODE_REL);
		gpio_dat->timer.function = gpio_timer_func;
		spin_lock_init(&gpio_dat->lock);

		gpio_dat->dev.name = cur_gpio->name;
		gpio_dat->dev.get_time = gpio_get_time;
		gpio_dat->dev.enable = gpio_enable;
		ret = gpio_request(cur_gpio->gpio, cur_gpio->name);
		if (ret < 0)
			goto err_out;
		ret = timed_output_dev_register(&gpio_dat->dev);
		if (ret < 0) {
			gpio_free(cur_gpio->gpio);
			goto err_out;
		}

		gpio_dat->gpio = cur_gpio->gpio;
		gpio_dat->max_timeout = cur_gpio->max_timeout;
		gpio_dat->active_low = cur_gpio->active_low;
		gpio_direction_output(gpio_dat->gpio, gpio_dat->active_low);
	}

	platform_set_drvdata(pdev, gpio_data);

	return 0;

err_out:
	while (--i >= 0) {
		timed_output_dev_unregister(&gpio_data[i].dev);
		gpio_free(gpio_data[i].gpio);
	}
	kfree(gpio_data);

	return ret;
}

static int timed_gpio_remove(struct platform_device *pdev)
{
	struct timed_gpio_platform_data *pdata = pdev->dev.platform_data;
	struct timed_gpio_data *gpio_data = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i < pdata->num_gpios; i++) {
		timed_output_dev_unregister(&gpio_data[i].dev);
		gpio_free(gpio_data[i].gpio);
	}

	kfree(gpio_data);

	return 0;
}

#if defined(CONFIG_OF)
static const struct of_device_id timed_gpio_of_match[] __devinitconst = {
	{ .compatible = "nufront,timed-gpio", },
	{},
};
MODULE_DEVICE_TABLE(of, timed_gpio_of_match);
#else
#define timed_gpio_of_match NULL
#endif

static struct platform_driver timed_gpio_driver = {
	.probe		= timed_gpio_probe,
	.remove		= timed_gpio_remove,
	.driver		= {
		.name		= TIMED_GPIO_NAME,
		.owner		= THIS_MODULE,
		.of_match_table = timed_gpio_of_match,
	},
};

static int __init timed_gpio_init(void)
{
	return platform_driver_register(&timed_gpio_driver);
}

static void __exit timed_gpio_exit(void)
{
	platform_driver_unregister(&timed_gpio_driver);
}

module_init(timed_gpio_init);
module_exit(timed_gpio_exit);

MODULE_AUTHOR("Mike Lockwood <lockwood@android.com>");
MODULE_DESCRIPTION("timed gpio driver");
MODULE_LICENSE("GPL");
