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
#include "tve.h"
#ifdef CONFIG_CH7025_7026_TVOUT
#include "ch7026/ch7026.h"
#endif
#ifdef CONFIG_CS8556_TVOUT
#include "cs8556/CS8556C0.h"
#endif
#ifdef CONFIG_NS115_STICK_LCD_OUT
#include "lcd/lcd.h"
#endif


#define DRV_NAME "tve"

#undef  dev_dbg
#define dev_dbg(dev, format, arg...) \
	dev_printk(KERN_CRIT, dev, format, ##arg)

#define DRIVER_VERSION		"1.0.0"
#if 0
#define JCG(frm,args...) 	printk(KERN_CRIT "----By James--[%s:%d]--<%s>--"frm,__FILE__,__LINE__,__func__,##args);
#define JDG(frm,args...) 	printk(KERN_CRIT " "frm,##args);
#else
#define JCG(frm,args...)
#define JDG(frm,args...) 	
#endif


struct miscdevice miscdev_tve;
static unsigned short tve_stat = STAT_UNKNOW;

#ifdef CONFIG_CH7025_7026_TVOUT
extern struct	ch7026	ch7026;
#endif
#ifdef  CONFIG_CS8556_TVOUT
extern struct cs8556	cs8556;
#endif
#ifdef CONFIG_NS115_STICK_LCD_OUT
extern struct stick_minor_display stick_minor_display;
#endif

#if 0
static void ns115_tve_hot_connect()
{
   	char *soft_connected[2]    = { "TVE_SOFT_STATE=1", NULL };
	printk(KERN_WARNING "ns115_tve_hot_connect()\n");
	JCG("\n");
	kobject_uevent_env(&miscdev_tve.this_device->kobj, KOBJ_CHANGE, soft_connected);
}
static void ns115_tve_hot_disconnect()
{
   	char *soft_connected[2]    = { "TVE_SOFT_STATE=1", NULL };
	printk(KERN_WARNING "ns115_tve_hot_connect()\n");
	JCG("\n");
	kobject_uevent_env(&miscdev_tve.this_device->kobj, KOBJ_CHANGE, soft_connected);
}

static ssize_t tve_test_show(struct device *pdev, struct device_attribute *attr, char *buf)
{
	JCG();
	ns115_tve_hot_connect();
}

static ssize_t tve_test_store(struct device *pdev, struct device_attribute *attr,const char *buff, size_t size)
{
	JCG();
	ns115_tve_hot_disconnect();
}


static DEVICE_ATTR(tve_test, S_IRUGO | S_IWUSR, tve_test_show, tve_test_store);
static struct device_attribute *tve_attributes[] = {
        &dev_attr_tve_test,
        NULL
};
#endif


#if 1		//JamesLee
static int tve_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int tve_close(struct inode *inode, struct file *file)
{
	return 0;
}

static long tve_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	switch (cmd)
	{
		case	TVE_STAT:	// 6
			JCG("change stat");
			break;
		case	TVE_DISABLE:		// 1
			tve_stat = STAT_DISCONNECT;
			JCG("TVE_DISABLE");
			tve_stat = STAT_CONNECT;
			break;
		case	TVE_ENABLE:		// 2
			JCG("TVE_ENABLE");
			break;
		case	TVE_SET_RESOLUTION_PAL:	// 3	/* VGA 720x576p */
			JCG("TVE_PAL");
			break;
		case	TVE_SET_RESOLUTION_NTSC:	// 4	/* VGA 640x480p */
			JCG("TVE_NTSC");
			break;
		case	TVE_SET_RESOLUTION_800_600_60:	// 5
			JCG("TVE_800_600");
			break;
		case	TVE_SET_RESOLUTION_1024_768_60:	// 6
			JCG("TVE_1024_768");
			break;
		default:
			JCG("TVE_INVAL");
		//	return -EINVAL;
			break;
	}
#ifdef CONFIG_CS8556_TVOUT
		ret |= cs8556.miscdev.fops->unlocked_ioctl(file,cmd,arg);
#endif
#ifdef CONFIG_CH7025_7026_TVOUT
		ret |= ch7026.miscdev.fops->unlocked_ioctl(file,cmd,arg);
#endif
#ifdef CONFIG_NS115_STICK_LCD_OUT
		ret |= stick_minor_display.miscdev.fops->unlocked_ioctl(file,cmd,arg);
#endif
			JCG("---===============ret:%d================--------",ret);
		if(cmd == TVE_STAT)
			return ret;
		return 0;
}

static const struct file_operations tve_fops = { 
	.owner          = THIS_MODULE,
	.open           = tve_open,
	.release        = tve_close,
	.unlocked_ioctl = tve_ioctl,
};


#endif





static int __init tve_init(void)
{
	int ret = 0,err=0;

	memset(&miscdev_tve, 0, sizeof(struct miscdevice));

	miscdev_tve.minor = 255;
	miscdev_tve.name = DRV_NAME;
	miscdev_tve.fops = &tve_fops;
//	miscdev_tve.parent = tve.dev;
	ret = misc_register(&miscdev_tve);
	printk("tve_init++++++++\n");	   

	if(ret < 0){
		printk("i2c_add_driver err, ret = %d\n", ret);
	}

#if 0
	err = device_create_file(miscdev_tve.this_device, &dev_attr_tve_test);
	if(ret < 0){
		printk("i2c_add_driver err, ret = %d\n", ret);
	}
#endif
	return ret;
}

static void __exit tve_exit(void)
{
	misc_deregister(&miscdev_tve);
}

module_init(tve_init);
module_exit(tve_exit);












