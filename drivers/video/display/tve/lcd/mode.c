/*
 * CH7025/CH7026 TV0UT VGA driver
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
#include <linux/ctype.h>
#include <linux/string.h>
#include "lcd.h"

static struct stick_minor_display_monspecs	lcd_monspecs;

static const struct fb_videomode lcd_mode[] = {
	//name		refresh xres	yres	pixclk	h_bp	h_fp 	v_bp 	v_fp 	h_pw 	v_pw	polariry	PorI	flag(used for vic)
    { " 640x480@60Hz  " , 60 , 640  , 480 , 39721 , 40  , 24 , 32 , 11 , 96  , 2 , 0 , 0                      , 0}  ,
    { " 800x600@60Hz  " , 60 , 800  , 600 , 25000 , 88  , 40 , 23 , 1  , 128 , 4 , 0 , 0                      , 17} ,
    { " 1024x768@60Hz " , 60 , 1024 , 768 , 15384 , 160 , 24 , 29 , 3  , 136 , 6 , 0 , 0                      , 0}  ,
    { " 1024x600@60Hz " , 60 , 1024 , 600 , 22857 , 53  , 88 , 4  , 16 , 35  , 5 , 0 , 0                      , 0}  ,
	{ " 800x480@60Hz  " , 50 , 800  , 480 , 40000 , 40  , 40 , 13 , 29 , 48  , 3 , 0 , FB_VMODE_NONINTERLACED , 0}  ,
};

static const struct fb_videomode stick_minor_display_lcd_mode [] = {
    { " 640x480@60Hz  " , 60 , 640  , 480 , 39721 , 40  , 24 , 32 , 11 , 96  , 2 , 0 , 0                      , 0}  ,
    { " 800x600@60Hz  " , 60 , 800  , 600 , 25000 , 88  , 40 , 23 , 1  , 128 , 4 , 0 , 0                      , 17} ,
    { " 1024x768@60Hz " , 60 , 1024 , 768 , 15384 , 160 , 24 , 29 , 3  , 136 , 6 , 0 , 0                      , 0}  ,
    { " 1024x600@60Hz " , 60 , 1024 , 600 , 22857 , 53  , 88 , 4  , 16 , 35  , 5 , 0 , 0                      , 0}  ,
	{ " 800x480@60Hz  " , 50 , 800  , 480 , 40000 , 40  , 40 , 13 , 29 , 48  , 3 , 0 , FB_VMODE_NONINTERLACED , 0}  ,
};

static void stick_minor_display_lcd_init(const struct fb_videomode *mode)
{	
	int temp;
	int out_select;
	const struct fb_videomode	*outmode;

	DBG("%s start", __FUNCTION__);

	return 0;
}

#if 0
static 
#endif
int lcd_set_enable(struct ns115_display_device *device, int enable)
{
	DBG("%s  %d", __FUNCTION__, enable);
    
	/*if(lcd_monspecs.enable != enable || (lcd_monspecs.mode_set != stick_minor_display.mode))*/
	{
		/*if(enable == 0 && lcd_monspecs.enable)*/
		if(enable == 0)
		{
			lcd_monspecs.enable = 0;
			/*stick_minor_display_standby();*/
		}
		else if(enable == 1)
		{
			/*lcd_monspecs.mode = stick_minor_display_lcd_mode[4];*/
			JCG("mode set:%d",lcd_monspecs.mode_set);
			stick_minor_display_switch_fb(&stick_minor_display_lcd_mode[4], lcd_monspecs.mode_set);
			/*stick_minor_display_switch_fb(lcd_monspecs.mode, lcd_monspecs.mode_set);*/
			stick_minor_display_lcd_init(lcd_monspecs.mode);
			lcd_monspecs.enable = 1;
		}
	}
	return 0;
}

static int lcd_get_enable(struct ns115_display_device *device)
{
	DBG("%s start", __FUNCTION__);
    
	return lcd_monspecs.enable;
}

static int lcd_get_status(struct ns115_display_device *device)
{
	DBG("%s start", __FUNCTION__);
    
	return stick_minor_display.mode;
}

static int lcd_get_modelist(struct ns115_display_device *device, struct list_head **modelist)
{
	DBG("%s start", __FUNCTION__);
    
	*modelist = &(lcd_monspecs.modelist);
	return 0;
}

static int lcd_set_mode(struct ns115_display_device *device, struct fb_videomode *mode)
{
	int i;

	DBG("%s start", __FUNCTION__);
    
	for(i = 0; i < ARRAY_SIZE(lcd_mode); i++)
	{
		if(fb_mode_is_equal(&lcd_mode[i], mode))
		{	
			if( ( i != stick_minor_display.mode) )
			{
				lcd_monspecs.mode_set = i;
				lcd_monspecs.mode = (struct fb_videomode *)&lcd_mode[i];
			}
			return 0;
		}
	}
	return -1;
}

static int lcd_get_mode(struct ns115_display_device *device, struct fb_videomode *mode)
{
	DBG("%s start", __FUNCTION__);
    
	*mode = *(lcd_monspecs.mode);
	return 0;
}

static struct ns115_display_ops lcd_display_ops = {
	.setenable = lcd_set_enable,
	.getenable = lcd_get_enable,
	.getstatus = lcd_get_status,
	.getmodelist = lcd_get_modelist,
	.setmode = lcd_set_mode,
	.getmode = lcd_get_mode,
};

static int display_lcd_probe(struct ns115_display_device *device, void *devdata)
{
	DBG("%s start", __FUNCTION__);
       
	device->owner = THIS_MODULE;
	strcpy(device->type, "lcd");
	device->name = "lcd";
	device->priority = DISPLAY_PRIORITY_VGA;
	device->priv_data = devdata;
	device->ops = &lcd_display_ops;

	return 1;
}

static struct ns115_display_driver display_lcd = {
	.probe = display_lcd_probe,
};

int stick_minor_display_register_display_lcd(struct device *parent)
{
	int i;

	DBG("%s start", __FUNCTION__);
	memset(&lcd_monspecs, 0, sizeof(struct stick_minor_display_monspecs));
    
	INIT_LIST_HEAD(&lcd_monspecs.modelist);
	for(i = 0; i < ARRAY_SIZE(lcd_mode); i++)
	{
		fb_add_videomode(&lcd_mode[i], &lcd_monspecs.modelist);
	}
	
	lcd_monspecs.mode = (struct fb_videomode *)&(lcd_mode[stick_minor_display.mode]);
	lcd_monspecs.mode_set = stick_minor_display.mode;

	lcd_monspecs.ddev = ns115_display_device_register(&display_lcd, parent, &stick_minor_display);
	if(lcd_monspecs.ddev != NULL)
	{
		stick_minor_display.lcd = &lcd_monspecs;
	}
	else
	{
		stick_minor_display.lcd = NULL;
		return -1;
	}

    return 0;
}


int stick_minor_display_unregister_display_lcd(struct ns115_display_device *device)
{
	stick_minor_display.lcd = NULL;
	ns115_display_device_unregister(device);
}



