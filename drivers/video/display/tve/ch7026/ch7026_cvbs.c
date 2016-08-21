/*
 * CH7025/CH7026 TV0UT CVBS driver
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
#include "ch7026.h"


static struct ch7026_monspecs	cvbs_monspecs;

static const struct fb_videomode cvbs_mode [] = {
    {"720x480p@60Hz", 60, 720, 480, KHZ2PICOS(28000), 62, 30, 30, 14, 62, 10,	0, FB_VMODE_NONINTERLACED, 0},
    {"720x576p@50Hz", 50, 720, 576, KHZ2PICOS(28000), 68, 24, 39, 13, 68, 8,	0, FB_VMODE_NONINTERLACED, 0},
    /*{"720x480p@60Hz", 60, 720, 480, KHZ2PICOS(27500), 62, 21, 30, 10, 62, 10,	0, FB_VMODE_NONINTERLACED, 0},*/
    /*{"720x576p@50Hz", 50, 720, 576, KHZ2PICOS(27500), 68, 17, 39, 7, 68, 8,	0, FB_VMODE_NONINTERLACED, 0},*/
    /*{"720x480p@60Hz", 60, 720, 480, KHZ2PICOS(27000), 60, 16, 30, 9, 62, 6,	0, FB_VMODE_NONINTERLACED, 0},*/
    /*{"720x576p@50Hz", 50, 720, 576, KHZ2PICOS(27000), 68, 12, 39, 5, 64, 5,	0, FB_VMODE_NONINTERLACED, 0},*/
};

static const struct fb_videomode ch7026_cvbs_mode [] = {
    {	"720x480p@60Hz",	60, 1440, 480,	27000000,	114,	38,	30,	9,	124,	6,	0,	FB_VMODE_NONINTERLACED, 0},
    {	"720x576p@50Hz",	50, 1440, 576,	27000000,	138,	24,	39,	5,	126,	5,    0,	FB_VMODE_NONINTERLACED, 0},	
};


static int ch7026_cvbs_init(struct fb_videomode *mode)
{
	int temp;
	const struct fb_videomode	*outmode;
	
	DBG("%s start", __FUNCTION__);

#if 1	
	ch7026_write_reg( 0x02, 0x01 );
	ch7026_write_reg( 0x02, 0x03 );
	ch7026_write_reg( 0x03, 0x00 );
	ch7026_write_reg( 0x04, 0x39 );
	temp = v_DEPO_O(1) | v_DEPO_I(1);
	if(mode->sync & FB_SYNC_HOR_HIGH_ACT)
		temp |= v_HPO_I(1) | v_HPO_O(1);
	if(mode->sync & FB_SYNC_VERT_HIGH_ACT)
		temp |= v_VPO_I(1) | v_VPO_O(1);
	ch7026_write_reg( SYNC_CONFIG, temp & 0xFF );
	ch7026_write_reg( 0x0A, 0x10 );
	if(ch7026.mode == TVOUT_CVBS_NTSC) {
		temp = NTSC_M;
		outmode = &ch7026_cvbs_mode[0];
	}
	else {
		temp = PAL_B_D_G_H_I;
		outmode = &ch7026_cvbs_mode[1];
	}
	ch7026_write_reg( OUT_FORMAT, v_WRFAST(0) | v_YPBPR_ENABLE(0) | v_YC2RGB(0) | v_FMT(temp));

	// input video timing
	// hactive and htotal
	temp = mode->xres + mode->left_margin + mode->hsync_len + mode->right_margin;
	ch7026_write_reg(HTI_HAI_HIGH, v_HVAUTO(0) | v_HREPT(0) | v_HTI_HIGH((temp >> 8) & 0xFF) | v_HAI_HIGH((mode->xres >> 8) & 0xFF) );
	ch7026_write_reg(HAI_LOW, mode->xres & 0xFF);
	ch7026_write_reg(HTI_LOW, temp & 0xFF);
	
	// hsync and hoffset
	ch7026_write_reg(HW_HO_HIGH, v_HDTVEN(0) | v_EDGE_ENH(1) | v_HW_HIGH((mode->hsync_len >> 8) & 0xFF));
	if(ch7026.mode == TVOUT_CVBS_NTSC)
	{
		ch7026_write_reg(HO_LOW, (mode->right_margin/2) & 0xFF);
	}
	else
	{
		ch7026_write_reg(HO_LOW, 0);
	}

	ch7026_write_reg(HW_LOW, mode->hsync_len & 0xFF);
		
	// vactive and vtotal
	temp = mode->yres + mode->upper_margin + mode->vsync_len + mode->lower_margin;
	ch7026_write_reg(VTI_VAI_HIGH, v_FIELDSW(0) | v_TVBYPSS(0) | v_VTI_HIGH((temp >> 8) & 0xFF) | v_VAI_HIGH((mode->yres >> 8) & 0xFF) );
	ch7026_write_reg(VTI_LOW, temp & 0xFF);
	ch7026_write_reg(VAI_LOW, mode->yres & 0xFF);
	
	// vsync and voffset
	ch7026_write_reg(VW_VO_HIGH, v_VW_HIGH( (mode->vsync_len >> 8) & 0xFF) | v_VO_HIGH( (mode->lower_margin >> 8) & 0xFF));
	ch7026_write_reg(VO_LOW, mode->lower_margin & 0xFF);
	ch7026_write_reg(VW_LOW, mode->vsync_len & 0xFF);
	
	// output video timing
	// hactive and htotal
	temp = outmode->xres + outmode->left_margin + outmode->hsync_len + outmode->right_margin;
	ch7026_write_reg(HTO_HAO_HIGH, v_HTO_HIGH((temp >> 8) & 0xFF) | v_HAO_HIGH((outmode->xres >> 8) & 0xFF) );
	ch7026_write_reg(HAO_LOW, outmode->xres & 0xFF);
	ch7026_write_reg(HTO_LOW, temp & 0xFF);
	// hsync and hoffset
	ch7026_write_reg(HWO_HOO_HIGH, v_HW_HIGH((outmode->hsync_len >> 8) & 0xFF) |  v_HO_HIGH((outmode->right_margin >> 8) & 0xFF));
	ch7026_write_reg(HOO_LOW, outmode->right_margin & 0xFF);
	ch7026_write_reg(HWO_LOW, outmode->hsync_len & 0xFF);
	
//	// vactive and vtotal
	temp = outmode->yres + outmode->upper_margin + outmode->vsync_len + outmode->lower_margin;
	ch7026_write_reg(VTO_VAO_HIGH,  v_VTO_HIGH((temp >> 8) & 0xFF) | v_VAO_HIGH((outmode->yres >> 8) & 0xFF) );
	ch7026_write_reg(VTO_LOW, temp & 0xFF);
	ch7026_write_reg(VAO_LOW, outmode->yres & 0xFF);
	
	// vsync and voffset
	ch7026_write_reg(VWO_VOO_HIGH, v_VWO_HIGH( (outmode->vsync_len >> 8) & 0xFF) | v_VOO_HIGH( (outmode->lower_margin >> 8) & 0xFF));
	ch7026_write_reg(VOO_LOW, outmode->lower_margin & 0xFF);
	ch7026_write_reg(VWO_LOW, outmode->vsync_len & 0xFF);

	if(outmode->vmode == FB_VMODE_INTERLACED)
	{
		int m_tmp = 0;
		m_tmp = ch7026_read_reg(0x0B);
		ch7026_write_reg(0x0B, m_tmp & 0xFF | 0x80);
	}
	else if(outmode->vmode == FB_VMODE_NONINTERLACED)
	{
		int m_tmp = 0;
		m_tmp = ch7026_read_reg(0x0B);
	//	ch7026_write_reg(0x0B, m_tmp & 0xFF | 0x00);
	}


	for(temp = 0x0F; temp < 0x27; temp++)
		DBG("%02x %02x", temp&0xFF, ch7026_read_reg(temp));
//	ch7026_write_reg(DEFLIKER_FILER, v_MONOB(1) | v_CHROM_BW(0) | v_FLIKER_LEVEL(4));

    ch7026_write_reg( 0x41, 0xDA );   //26M    
    ch7026_write_reg( 0x4D, 0x04 );
    ch7026_write_reg( 0x4E, 0x27 );
    ch7026_write_reg( 0x4F, 0x62 );
    ch7026_write_reg( 0x50, 0x76 );
                                  
    ch7026_write_reg( 0x51, 0x51 );
    ch7026_write_reg( 0x52, 0x1B );
    ch7026_write_reg( 0x53, 0x1A );
    ch7026_write_reg( 0x55, 0xE5 );
    ch7026_write_reg( 0x5E, 0x80 );
    ch7026_write_reg( 0x7D, 0x62 );
    ch7026_write_reg( 0x04, 0x38 );
    ch7026_write_reg( 0x06, 0x71 );
#endif

#if 0   //output cvbs
    ch7026_write_reg( 0x02, 0x01 );
    ch7026_write_reg( 0x02, 0x03 );
    ch7026_write_reg( 0x03, 0x00 );
    ch7026_write_reg( 0x04, 0x39 );
    ch7026_write_reg( 0x0F, 0x1A );
    ch7026_write_reg( 0x10, 0xD0 );
    ch7026_write_reg( 0x11, 0x5A );
    ch7026_write_reg( 0x12, 0x40 );
    ch7026_write_reg( 0x13, 0x10 );
    ch7026_write_reg( 0x14, 0x3E );
    ch7026_write_reg( 0x15, 0x11 );
    ch7026_write_reg( 0x16, 0xE0 );
    ch7026_write_reg( 0x17, 0x0D );
    ch7026_write_reg( 0x19, 0x09 );
    ch7026_write_reg( 0x1A, 0x06 );
    ch7026_write_reg( 0x41, 0xDA );
    ch7026_write_reg( 0x4D, 0x04 );
    ch7026_write_reg( 0x4E, 0x27 );
    ch7026_write_reg( 0x4F, 0x62 );
    ch7026_write_reg( 0x50, 0x76 );
    ch7026_write_reg( 0x51, 0x51 );
    ch7026_write_reg( 0x52, 0x1B );
    ch7026_write_reg( 0x53, 0x1A );
    ch7026_write_reg( 0x55, 0xE5 );
    ch7026_write_reg( 0x5E, 0x80 );
    ch7026_write_reg( 0x7D, 0x62 );
    ch7026_write_reg( 0x04, 0x38 );
    ch7026_write_reg( 0x06, 0x71 );
#endif

	/*
	NOTE: The following five repeated sentences are used here to wait memory initial complete, please don't remove...(you could refer to Appendix A of programming guide document (CH7025(26)B Programming Guide Rev2.03.pdf or later version) for detailed information about memory initialization! 
	*/
	ch7026_write_reg( 0x03, 0x00 );
	ch7026_write_reg( 0x03, 0x00 );
	ch7026_write_reg( 0x03, 0x00 );
	ch7026_write_reg( 0x03, 0x00 );
	ch7026_write_reg( 0x03, 0x00 );
	
	ch7026_write_reg( 0x06, 0x70 );
	ch7026_write_reg( 0x02, 0x02 );
	ch7026_write_reg( 0x02, 0x03 );
	ch7026_write_reg( 0x04, 0x00 );
//	ch7026_write_reg(PWR_STATE1, v_PD_DAC(6));

	return 0;
}

static int cvbs_set_enable(struct ns115_display_device *device, int enable)
{
	DBG("%s start enable %d", __FUNCTION__, enable);
    
	if(cvbs_monspecs.enable != enable || cvbs_monspecs.mode_set != ch7026.mode)
	{
		if(enable == 0 && cvbs_monspecs.enable)
		{
			cvbs_monspecs.enable = 0;
			ch7026_standby();
		}
		else if(enable == 1)
		{
			ch7026_switch_fb(cvbs_monspecs.mode, cvbs_monspecs.mode_set);
			ch7026_cvbs_init(cvbs_monspecs.mode);
			cvbs_monspecs.enable = 1;
		}
	}
	return 0;
}

static int cvbs_get_enable(struct ns115_display_device *device)
{
	DBG("%s start", __FUNCTION__);
    
	return cvbs_monspecs.enable;
}

static int cvbs_get_status(struct ns115_display_device *device)
{
	DBG("%s start", __FUNCTION__);
    
	if(ch7026.mode < TVOUT_VGA_640x480p_60)
		return 1;
	else
		return 0;
}

static int cvbs_get_modelist(struct ns115_display_device *device, struct list_head **modelist)
{
	DBG("%s start", __FUNCTION__);
    
	*modelist = &(cvbs_monspecs.modelist);
	return 0;
}

static int cvbs_set_mode(struct ns115_display_device *device, struct fb_videomode *mode)
{
	int i;
    
	DBG("%s start", __FUNCTION__);
    
	for(i = 0; i < ARRAY_SIZE(cvbs_mode); i++)
	{
		if(fb_mode_is_equal(&cvbs_mode[i], mode))
		{	
			if( ((i + 1) != ch7026.mode) )
			{
				cvbs_monspecs.mode_set = i + 1;
				cvbs_monspecs.mode = (struct fb_videomode *)&cvbs_mode[i];
			}
			return 0;
		}
	}
	
	return -1;
}

static int cvbs_get_mode(struct ns115_display_device *device, struct fb_videomode *mode)
{
	DBG("%s start", __FUNCTION__);
    
	*mode = *(cvbs_monspecs.mode);
    
	return 0;
}

static struct ns115_display_ops cvbs_display_ops = {
	.setenable = cvbs_set_enable,
	.getenable = cvbs_get_enable,
	.getstatus = cvbs_get_status,
	.getmodelist = cvbs_get_modelist,
	.setmode = cvbs_set_mode,
	.getmode = cvbs_get_mode,
};

static int display_cvbs_probe(struct ns115_display_device *device, void *devdata)
{
       DBG("%s start", __FUNCTION__);
       
	device->owner = THIS_MODULE;
	strcpy(device->type, "TV");
	device->name = "cvbs";
	device->priority = DISPLAY_PRIORITY_TV;
	device->priv_data = devdata;
	device->ops = &cvbs_display_ops;
    
	return 1;
}

static struct ns115_display_driver display_cvbs = {
	.probe = display_cvbs_probe,
};

int ch7026_register_display_cvbs(struct device *parent)
{
    int i;

    DBG("%s start", __FUNCTION__);
	
    memset(&cvbs_monspecs, 0, sizeof(struct ch7026_monspecs));
    
    INIT_LIST_HEAD(&cvbs_monspecs.modelist);
    for(i = 0; i < ARRAY_SIZE(cvbs_mode); i++)
    {
        fb_add_videomode(&cvbs_mode[i], &cvbs_monspecs.modelist);
    }
		
    if(ch7026.mode < TVOUT_VGA_640x480p_60) {
        cvbs_monspecs.mode = (struct fb_videomode *)&(cvbs_mode[ch7026.mode - 1]);
        cvbs_monspecs.mode_set = ch7026.mode;
    }else {
        cvbs_monspecs.mode = (struct fb_videomode *)&(cvbs_mode[0]);
        cvbs_monspecs.mode_set = TVOUT_CVBS_PAL;
    }
	
    cvbs_monspecs.ddev = ns115_display_device_register(&display_cvbs, parent, &ch7026);
	if(cvbs_monspecs.ddev != NULL)
	    ch7026.cvbs = &cvbs_monspecs;
	else
	{
		ch7026.cvbs = NULL;
		return -1;
	}
    
    return 0;
}


int ch7026_unregister_display_cvbs(struct ns115_display_device *device)
{
    ch7026.cvbs = NULL;
	ns115_display_device_unregister(device);
}

