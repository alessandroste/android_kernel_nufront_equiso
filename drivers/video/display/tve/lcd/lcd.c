/*
 * CH7025/CH7026 TV0UT driver
 *
 * Copyright (C) 2012 NUFRONT, Inc.
 * Written by Ping Chen <ping.chen@nufront.com>
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#include <linux/module.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/fb.h>
#include <linux/console.h>
#include <asm/uaccess.h>
#include "lcd.h"

#define DRV_NAME "stick_minor_display"

#undef  dev_dbg
#define dev_dbg(dev, format, arg...) \
	dev_printk(KERN_CRIT, dev, format, ##arg)

#define DRIVER_VERSION		"1.0.0"


static bool enable_stick_minor_display = false;
struct stick_minor_display stick_minor_display;



extern int ns115_fb_switch_screen(const struct fb_videomode *mode,int enable ,int id);


int stick_minor_display_write_reg(char reg, char value)
{
	return 0;
}

int stick_minor_display_read_reg(char reg)
{
	return 0;
}

int stick_minor_display_switch_fb(const struct fb_videomode *modedb, int mode)
{
    if(modedb == NULL)
	{
		JCG();
		return -1;
	}
	JCG();

    stick_minor_display.mode = mode;
    ns115_fb_switch_screen(modedb, 1 , stick_minor_display.video_source);

	return 0;  
}

static void stick_minor_display_disable(void)
{
	DBG("%s start", __FUNCTION__);
}

static void stick_minor_display_enable(void)
{
	DBG("%s start", __FUNCTION__);
}

void stick_minor_display_standby(void)
{
	int ret;
	int lcd = 0;
	
	/*if(!lcd)*/
		/*stick_minor_display_disable();*/
}

static int stick_minor_display_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int stick_minor_display_close(struct inode *inode, struct file *file)
{
	return 0;
}

static long stick_minor_display_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret;
	int temp,temp_2;
	JCG();
	switch (cmd)
	{
		case	TVE_STAT:	// 6
			JCG();
			if(stick_minor_display.lcd)
				temp_2 = stick_minor_display.lcd->ddev->ops->getenable(stick_minor_display.lcd->ddev);
			return (temp_2);
		case	TVE_DISABLE:
			JCG("disable CH7026");
			stick_minor_display_disable();
#if 1
		ret = gpio_request(5, "stick_lcd_vdd");
		ret = gpio_request(6, "stick_lcd_bl");
		gpio_direction_output(5, 0);
		gpio_direction_output(6, 0);
		msleep(10);
		gpio_direction_output(5, 0);
		gpio_direction_output(6, 0);
		gpio_free(5);
		gpio_free(6);
#endif
			/*stick_minor_display.lcd->ddev->ops->setenable(stick_minor_display.lcd->ddev, 0);*/
			break;
		case	TVE_ENABLE:
			stick_minor_display_enable();
			JCG("enable CH7026");
			break;
		case	TVE_SET_RESOLUTION_PAL:
		case	TVE_SET_RESOLUTION_NTSC:
		case	TVE_SET_RESOLUTION_800_600_60:
			JCG("Set resolution to lcd 800x600");
			stick_minor_display.mode = TVOUT_VGA_1024x600p_60;
			/*stick_minor_display.dev->ops->setenable(stick_minor_display.lcd->ddev, 1);*/
#if 1
		ret = gpio_request(5, "stick_lcd_vdd");
		ret = gpio_request(6, "stick_lcd_bl");
		gpio_direction_output(5, 1);
		gpio_direction_output(6, 1);
		msleep(10);
		gpio_direction_output(5, 1);
		gpio_direction_output(6, 1);
		gpio_free(5);
		gpio_free(6);
#endif
			lcd_set_enable(NULL, 1);
			enable_stick_minor_display = true;
			break;
		default:
			return -EINVAL;
		return 0;
	}
}

static const struct file_operations stick_minor_display_fops = { 
	.owner          = THIS_MODULE,
	.open           = stick_minor_display_open,
	.release        = stick_minor_display_close,
	.unlocked_ioctl = stick_minor_display_ioctl,
};



static int __init stick_minor_display_init(void)
{
	int ret = 0,err=0;

	memset(&(stick_minor_display.miscdev), 0, sizeof(struct miscdevice));

	stick_minor_display.miscdev.minor = 255;
	stick_minor_display.miscdev.name = DRV_NAME;
	stick_minor_display.miscdev.fops = &stick_minor_display_fops;
	/*stick_minor_display.miscdev.parent = stick_minor_display.dev;*/
	ret = misc_register(&stick_minor_display.miscdev);
	printk("tve_init++++++++\n");	   

	/*stick_minor_display_register_display_lcd(&stick_minor_display.dev);*/

	if(ret < 0){
		printk("i2c_add_driver err, ret = %d\n", ret);
	}
	return ret;
}

static void __exit stick_minor_display_exit(void)
{
}

module_init(stick_minor_display_init);
module_exit(stick_minor_display_exit);
