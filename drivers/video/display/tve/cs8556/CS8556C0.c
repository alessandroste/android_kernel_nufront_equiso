/*
 * CS8556 TV0UT driver
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
#include "CS8556C0.h"

//#include "Global.h"
//#include "Normal_CCIR.h"


#define DRV_NAME "cs8556"

#undef  dev_dbg
#define dev_dbg(dev, format, arg...) \
	dev_printk(KERN_CRIT, dev, format, ##arg)

#define DRIVER_VERSION		"1.0.0"


static bool enable_cs8556 = false;
struct cs8556 cs8556;



extern int ns115_fb_switch_screen(const struct fb_videomode *mode,int enable ,int id);


int cs8556_write_reg(char reg, char value)
{
	struct i2c_msg msg[1];
	unsigned char buf[2];
	int ret;

	if(cs8556.client == NULL)
		return -1;

	buf[0] = reg;
	buf[1] = value;
	msg[0].addr = cs8556.client->addr;
	msg[0].flags = cs8556.client->flags & I2C_M_TEN;
	msg[0].buf = buf;
	msg[0].len = 2;

	ret = i2c_transfer(cs8556.client->adapter, msg, ARRAY_SIZE(msg));
	if (ret < 0){
		dev_err(&cs8556.client->dev, "cs8556_write_reg:write transfer error\n");
	}

	return ret;
}

int cs8556_read_reg(char page,char saddr)
{
	struct i2c_msg msg[2];
	unsigned char data;
	int ret;
	char reg[2];
	reg[0] = page;
	reg[1] = saddr;

	if(cs8556.client == NULL)
		return -1;

	msg[0].addr = cs8556.client->addr;
	msg[0].flags = cs8556.client->flags & I2C_M_TEN;
	msg[0].buf = &reg;
	msg[0].len = 2;
	msg[1].addr = cs8556.client->addr;
	msg[1].flags = (cs8556.client->flags & I2C_M_TEN) | I2C_M_RD;
	msg[1].buf = &data;
	msg[1].len = 1;

	ret = i2c_transfer(cs8556.client->adapter, msg, ARRAY_SIZE(msg));
	if (ret < 0){
		dev_err(&cs8556.client->dev, "cs8556_read_reg:read transfer error\n");
	return ret;
	}

	return data;
}

int cs8556_write_mutil_reg(char page, char saddr,char *value, int number)
{
	struct i2c_msg msg[1];
	unsigned char buf[257];
	int ret;
	int i0 = 0;

	if(cs8556.client == NULL)
		return -1;

	buf[0] = page;
	buf[1] = saddr;
	while(i0<number)
	{
		buf[i0+2] = value[i0];
		i0++;
	}

	msg[0].addr = cs8556.client->addr;
	msg[0].flags = cs8556.client->flags & I2C_M_TEN;
	msg[0].buf = buf;
	msg[0].len = number + 2;

	ret = i2c_transfer(cs8556.client->adapter, msg, ARRAY_SIZE(msg));

	if (ret < 0){
		dev_err(&cs8556.client->dev, "cs8556_write_reg:write transfer error\n");
	}

	return ret;
}

static int cs8556_connect_detect(void)
{
	unsigned char data = 0;

	cs8556_write_reg( 0x02, 0x01 );
	cs8556_write_reg( 0x02, 0x03 );
	cs8556_write_reg( 0x03, 0x00 );

	data |=(1<<3)|(1<<4)|(1<<5);
	cs8556_write_reg( 0x04, data );
	cs8556_write_reg( 0x7d, 0x01 );

	msleep(100);
//	data=cs8556_read_reg(0x7f);
	DBG("%s DAC Connected detcet 0x%x", __FUNCTION__,data);

	if((data & 0x03) == 0x01)
		DBG("%s DAC0 Connected", __FUNCTION__);

	if(((data >> 2) & 0x03) == 0x01)
		DBG("%s DAC1 Connected", __FUNCTION__);

	if(((data >> 4) & 0x03) == 0x01)
		DBG("%s DAC2 Connected", __FUNCTION__);

	cs8556_write_reg( 0x7d, 0x01 );
	cs8556_write_reg( 0x04, 0x00 ); 

	return 1;

}

#if 1
int cs8556_switch_fb(const struct fb_videomode *modedb, int mode)
{
	if(modedb == NULL)
		return -1;
	cs8556.mode = mode;
	ns115_fb_switch_screen(modedb, 1 , cs8556.video_source);

	return 0;  
}
#endif

static void cs8556_disable(void)
{
	DBG("%s start", __FUNCTION__);
}

static void cs8556_enable(void)
{
	DBG("%s start", __FUNCTION__);
}

void cs8556_standby(void)
{
	int cvbs = 0,vga = 0;

	if(cs8556.cvbs)
		cvbs = cs8556.cvbs->enable;
	if(cs8556.vga)
		cvbs = cs8556.vga->enable;

	if(!cvbs && !vga)
		cs8556_disable();
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void cs8556_early_suspend(struct early_suspend *h)
{
	DBG("cs8556 enter early suspend");

	if(cs8556.cvbs)
		cs8556.cvbs->ddev->ops->setenable(cs8556.cvbs->ddev, 0);
	if(cs8556.vga)
		cs8556.vga->ddev->ops->setenable(cs8556.vga->ddev, 0);

	return;
}

static void cs8556_early_resume(struct early_suspend *h)
{
	DBG("cs8556 exit early resume");

	if(cs8556.cvbs && (cs8556.mode < TVOUT_VGA_800x600p_60))
	{
		ns115_display_device_enable((cs8556.cvbs)->ddev);
	}
	else if(cs8556.vga && (cs8556.mode > TVOUT_CVBS_PAL)){
		ns115_display_device_enable((cs8556.vga)->ddev);
	}
	return;
}
#endif

static irqreturn_t cs8556_cvbs_detect_irq(int irq, void *_info)
{
	disable_irq_nosync(irq);
	schedule_work(&cs8556.work);

	return IRQ_HANDLED;
}

static void cs8556_cvbs_detect_irq_work(struct work_struct * work)
{
	unsigned long irqflags;
	int ret,cvbs_status,state_tmp;
	int deb_cnt=0;

	msleep(100);
	cvbs_status = 1;//gpio_get_value(cs8556.io_cvbs_det_pin);
	for(deb_cnt = 0; deb_cnt < 5; )
	{
		msleep(60);
		state_tmp = 1;//gpio_get_value(cs8556.io_cvbs_det_pin);
		if(cvbs_status == state_tmp){
			deb_cnt++;
		}else{
			cvbs_status = state_tmp;
			deb_cnt = 0;
		}
	}

	if(cvbs_status)
	{
		if(cs8556.mode != TVOUT_CVBS_NTSC && cs8556.mode !=TVOUT_CVBS_PAL)
			cs8556.mode = TVOUT_CVBS_PAL;

	}
	else
		cs8556.mode = TVOUT_VGA_800x600p_60;

	if( cs8556.cvbs && (cs8556.mode < TVOUT_VGA_800x600p_60) )
		ns115_display_device_enable((cs8556.cvbs)->ddev);

	if( cs8556.vga && (cs8556.mode > TVOUT_CVBS_PAL) ) 
		ns115_display_device_enable((cs8556.cvbs)->ddev);

	irqflags = cvbs_status?IRQF_TRIGGER_FALLING:IRQF_TRIGGER_RISING;

	free_irq(cs8556.cvbs_det_irq, NULL);
	ret = request_threaded_irq(cs8556.cvbs_det_irq, NULL, cs8556_cvbs_detect_irq,
			irqflags, "cs8556_cvbs_detect", NULL);
}

#if 1		//JamesLee
static int cs8556_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int cs8556_close(struct inode *inode, struct file *file)
{
	return 0;
}

static long cs8556_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret;
	unsigned char temp,temp_2;
	switch (cmd)
	{
		case	TVE_STAT:	// 6
#if 0
			if(cs8556.cvbs)
				temp = cs8556.cvbs->ddev->ops->getenable(cs8556.cvbs->ddev);
			if(cs8556.vga)
				temp_2 = cs8556.vga->ddev->ops->getenable(cs8556.vga->ddev);
			return (temp | temp_2);
#else
			return 1;
#endif
			break;
		case	TVE_DISABLE:		// 1
		{
			gpio_direction_output(cs8556.io_rst_pin, GPIO_LOW);
			msleep(1);
			gpio_direction_output(cs8556.io_rst_pin, GPIO_HIGH);
		}
			break;
		case	TVE_ENABLE:		// 2
		{
			gpio_direction_output(cs8556.io_rst_pin, GPIO_LOW);
			msleep(1);
			gpio_direction_output(cs8556.io_rst_pin, GPIO_HIGH);
		}
			break;
#ifdef CONFIG_CS8556_TVOUT_CVBS
		case	TVE_SET_RESOLUTION_PAL:	// 3	/* CVBS 720x576p */
		{
			cs8556.mode = TVOUT_CVBS_PAL;
			ret = cs8556.vga->ddev->ops->setenable(cs8556.vga->ddev,0);
			ret = cs8556.cvbs->ddev->ops->setenable(cs8556.cvbs->ddev,0);
			ret = cs8556.cvbs->ddev->ops->setenable(cs8556.cvbs->ddev,1);
		}
			break;
		case TVE_SET_RESOLUTION_NTSC:	// 4	/* CVBS 720x480p */
		{
			//cs8556.mode = TVOUT_VGA_640x480p_60;
			cs8556.mode = TVOUT_CVBS_PAL;
			ret = cs8556.vga->ddev->ops->setenable(cs8556.vga->ddev,0);
			ret = cs8556.cvbs->ddev->ops->setenable(cs8556.cvbs->ddev,0);
			ret = cs8556.cvbs->ddev->ops->setenable(cs8556.cvbs->ddev,1);
		}
			break;
#endif
#ifdef CONFIG_CS8556_TVOUT_VGA
		case TVE_SET_RESOLUTION_800_600_60:	// 5
		{
			cs8556.mode = TVOUT_VGA_800x600p_60;
			ret = cs8556.cvbs->ddev->ops->setenable(cs8556.cvbs->ddev,0);
			ret = cs8556.vga->ddev->ops->setenable(cs8556.vga->ddev,0);
			ret = cs8556.vga->ddev->ops->setenable(cs8556.vga->ddev,1);
		}
			break;
		case TVE_SET_RESOLUTION_1024_768_60:	// 6
		{
#if 0
			cs8556.mode = TVOUT_VGA_1024x768p_60;
			cs8556_unregister_display_vga((cs8556.vga)->ddev);
			cs8556_register_display_vga(&(cs8556.client->dev));
			if( cs8556.vga && (cs8556.mode >= TVOUT_VGA_640x480p_60) )
				ns115_display_device_enable((cs8556.vga)->ddev);
#endif
			/*cs8556_write_mutil_reg(0x00,0x00,&VGA_800x600_offset_00,256);*/
			/*cs8556_write_mutil_reg(0x01,0x00,&VGA_640x480_offset_01,256);*/
		}
			break;
#endif
		default:
			return -EINVAL;
		return 0;
	}
}

static const struct file_operations cs8556_fops = {
	.owner          = THIS_MODULE,
	.open           = cs8556_open,
	.release        = cs8556_close,
	.unlocked_ioctl = cs8556_ioctl,
};


#endif




static int cs8556_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	struct ns115_display_platform_data *tv_data;
	int cvbs_status = 0;
	unsigned long irqflags = 0;
	unsigned char temp;
	int m_loop = 0;

	printk(KERN_CRIT "[JamesLee]cs8556_probe %s\n",DRIVER_VERSION);

	memset(&cs8556, 0, sizeof(struct cs8556));
	cs8556.client = client;
	cs8556.dev = &client->dev;
	cs8556.mode = CS8556_DEFAULT_MODE;


	if(client->dev.platform_data) {
		tv_data = client->dev.platform_data;
		cs8556.io_rst_pin = tv_data->io_reset_pin;
	}

	cs8556.miscdev.minor = 255;
	cs8556.miscdev.name = DRV_NAME;
	cs8556.miscdev.fops = &cs8556_fops;
	cs8556.miscdev.parent = cs8556.dev;
	ret = misc_register(&cs8556.miscdev);
	cs8556.mode = TVOUT_CVBS_PAL;

	// Reset
	if(cs8556.io_rst_pin != INVALID_GPIO) {
		ret = gpio_request(cs8556.io_rst_pin, "cs8556_tvout_rst");
		if(ret) {
			printk(KERN_ERR "cs8556 request reset gpio error\n");
			return -1;
		}
#ifndef CS8556_UBOOT_SUPPORT
		gpio_direction_output(cs8556.io_rst_pin, GPIO_LOW);
		msleep(1);
		gpio_direction_output(cs8556.io_rst_pin, GPIO_HIGH);
#endif
	}

#ifdef CONFIG_CS8556_TVOUT_CVBS
	cs8556_register_display_cvbs(&client->dev);
	/*if( cs8556.cvbs && (cs8556.mode < TVOUT_VGA_800x600p_60) )*/
	/*{*/
		/*ns115_display_device_enable((cs8556.cvbs)->ddev);*/
	/*}*/
#endif
#ifdef CONFIG_CS8556_TVOUT_VGA
	cs8556_register_display_vga(&client->dev);
	/*if( cs8556.vga && (cs8556.mode > TVOUT_CVBS_PAL) )*/
		/*ns115_display_device_enable((cs8556.vga)->ddev);*/
#endif

#if 0 //def CONFIG_HAS_EARLYSUSPEND
	cs8556.early_suspend.suspend = cs8556_early_suspend;
	cs8556.early_suspend.resume = cs8556_early_resume;
	cs8556.early_suspend.level = EARLY_SUSPEND_LEVEL_DISABLE_FB - 10;
	register_early_suspend(&(cs8556.early_suspend));
#endif

	printk(KERN_INFO "cs8556 probe sucess.\n");


	return 0;

}

static int cs8556_remove(struct i2c_client *client)
{
	return 0;
}




static const struct i2c_device_id cs8556_id[] = {
	{ DRV_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, cs8556_id);

static struct i2c_driver cs8556_driver = {
	.driver 	= {
		.name	= DRV_NAME,
	},
	.id_table = cs8556_id,
	.probe = cs8556_probe,
	.remove = cs8556_remove,
};


static int __init cs8556_init(void)
{
	int ret = 0;

	printk("cs8556_init++++++++\n");

	ret = i2c_add_driver(&cs8556_driver);
	if(ret < 0){
		printk("i2c_add_driver err, ret = %d\n", ret);
	}
	return ret;
}

static void __exit cs8556_exit(void)
{
	i2c_del_driver(&cs8556_driver);
}

module_init(cs8556_init);
module_exit(cs8556_exit);









