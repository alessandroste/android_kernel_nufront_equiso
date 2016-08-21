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
#include "ch7026.h"

static struct ch7026_monspecs	vga_monspecs;

static const struct fb_videomode vga_mode[] = {
	//name		refresh xres	yres	pixclk	h_bp	h_fp 	v_bp 	v_fp 	h_pw 	v_pw	polariry	PorI	flag(used for vic)
    {	"640x480@60Hz", 60, 	640, 	480, 	39721, 	40, 	24, 	32, 	11, 	96, 	2,	0, 		0, 	0}, 
    {	"800x600@60Hz", 60, 	800, 	600, 	25000, 	88, 	40, 	23, 	1, 	128, 	4, 	0, 		0, 	17},
    {	"1024x768@60Hz",60, 	1024, 	768, 	15384, 	160, 	24, 	29, 	3, 	136, 	6, 	0, 		0, 	0},	
    {	"1024x600@60Hz",60, 	1024,	600,	22857,	53,	88,	4,	16,	35,	5, 	0, 		0, 	0},	
};

static const struct fb_videomode ch7026_vga_mode [] = {
    {	"640x480@60Hz", 60, 	640, 	480, 	39721, 	40, 	24, 	32, 	11, 	96, 	2,	0, 		0, 	0}, 
    {	"800x600@60Hz",	60, 	800, 	600, 	25000, 	88, 	40, 	23, 	1, 	128, 	4, 	0, 	0, 	17},
    {	"1024x768@60Hz",60, 	1024, 	768, 	15384, 	160, 	24, 	29, 	3, 	136, 	6, 	0, 	0, 	0},	
    {	"1024x600@60Hz",60, 	1024,	600,	22857,	53,	88,	4,	16,	35,	5, 	0, 		0, 	0},	
};

static void ch7026_vga_init(const struct fb_videomode *mode)
{	
       int temp;
       int out_select;
	const struct fb_videomode	*outmode;
	
	DBG("%s start", __FUNCTION__);

#if 1
	ch7026_write_reg( 0x02, 0x01 );
	ch7026_write_reg( 0x02, 0x03 );
	ch7026_write_reg( 0x03, 0x00 );
	ch7026_write_reg( 0x04, 0x39 );
	//Test Mode
	
	#if 0
	ch7026_write_reg( 0x03, 0x01 );
	ch7026_write_reg( 0x04, 0x34 );
	ch7026_write_reg( 0x03, 0x00 );
	//Exit test mode
	/*
	ch7026_write_reg( 0x03, 0x00 );
	ch7026_write_reg( 0x04, 0x00 );
	ch7026_write_reg( 0x03, 0x01 );
	*/
	#endif
	
	temp = v_DEPO_O(1) | v_DEPO_I(1);		//temp = 0010 0100
	if(mode->sync & FB_SYNC_HOR_HIGH_ACT)		//sync & 0001
	{
		temp |= v_HPO_I(1) | v_HPO_O(1);	//temp |= 0001 0010
	}
	if(mode->sync & FB_SYNC_VERT_HIGH_ACT)		//sync & 0010
	{
		temp |= v_VPO_I(1) | v_VPO_O(1);	//temp |= 0000 1001
	}
	
	ch7026_write_reg( SYNC_CONFIG, temp & 0xFF );	//0x07 , temp
	
//	ch7026_write_reg( 0x0A, 0x10 );

	if(ch7026.mode == TVOUT_VGA_640x480p_60&& out_select == TVOUT_VGA_1024x768p_60)
	{
		;
	}
	else	
	{
		ch7026_write_reg( 0x06, 0x6B );
	}
    	ch7026_write_reg( 0x08, 0x08 );
    	ch7026_write_reg( 0x09, 0x80 );
    //	ch7026_write_reg( 0x0D, 0x08 );
    	ch7026_write_reg( 0x0D, 0x88 );
	

#if 1
	temp = VGA;
	out_select = TVOUT_VGA_800x600p_60;
	out_select = TVOUT_VGA_1024x768p_60;
//	out_select = TVOUT_VGA_1024x600p_60;
	outmode = &ch7026_vga_mode[out_select - 3];

	if(ch7026.mode == TVOUT_VGA_1024x600p_60)
		ch7026_write_reg( 0x0E, 0xE4 );

	ch7026_write_reg( OUT_FORMAT, v_WRFAST(0) | v_YPBPR_ENABLE(0) | v_YC2RGB(0) | v_FMT(temp));
	// input video timing
	// hactive and htotal
	temp = mode->xres + mode->left_margin + mode->hsync_len + mode->right_margin;
	ch7026_write_reg(HTI_HAI_HIGH, v_HVAUTO(0) | v_HREPT(0) | v_HTI_HIGH((temp >> 8) & 0xFF) | v_HAI_HIGH((mode->xres >> 8) & 0xFF) );
	ch7026_write_reg(HAI_LOW, mode->xres & 0xFF);
	ch7026_write_reg(HTI_LOW, temp & 0xFF);
	
	// hsync and hoffset
	ch7026_write_reg(HW_HO_HIGH, v_HDTVEN(0) | v_EDGE_ENH(1) | v_HW_HIGH((mode->hsync_len >> 8) & 0xFF));
	ch7026_write_reg(HO_LOW, mode->right_margin & 0xFF);
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
	// vactive and vtotal
	temp = outmode->yres + outmode->upper_margin + outmode->vsync_len + outmode->lower_margin;
	ch7026_write_reg(VTO_VAO_HIGH,  v_VTO_HIGH((temp >> 8) & 0xFF) | v_VAO_HIGH((outmode->yres >> 8) & 0xFF) );
	ch7026_write_reg(VTO_LOW, temp & 0xFF);
	ch7026_write_reg(VAO_LOW, outmode->yres & 0xFF);
	
	// vsync and voffset
	ch7026_write_reg(VWO_VOO_HIGH, v_VWO_HIGH( (outmode->vsync_len >> 8) & 0xFF) | v_VOO_HIGH( (outmode->lower_margin >> 8) & 0xFF));
	ch7026_write_reg(VOO_LOW, outmode->lower_margin & 0xFF);
	ch7026_write_reg(VWO_LOW, outmode->vsync_len & 0xFF);


//	for(temp = 0x0F; temp < 0x27; temp++)
//		DBG("%02x %02x", temp&0xFF, ch7025_read_reg(temp));
//	ch7026_write_reg(DEFLIKER_FILER, v_MONOB(1) | v_CHROM_BW(0) | v_FLIKER_LEVEL(4));

	#if 0
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
	#else

#if 0	//rotate
	ch7026_write_reg( 0x2D, 0x02 );
#endif

	ch7026_write_reg( 0x37, 0x20 );
	ch7026_write_reg( 0x39, 0x20 );
	ch7026_write_reg( 0x3B, 0x20 );
	ch7026_write_reg( 0x41, 0xDA );

	//	For 800x600 output
	if(out_select == TVOUT_VGA_800x600p_60)
	{
		ch7026_write_reg( 0x4D, 0x03 );		//clk divided:	A1:	0x030f_8e92	//37.8k
		ch7026_write_reg( 0x4E, 0x0F );
		ch7026_write_reg( 0x4F, 0x8E );
		ch7026_write_reg( 0x50, 0x92 );
		ch7026_write_reg( 0x51, 0x51 );		//clk divided:	A2:	0x51
	}
	else if (out_select == TVOUT_VGA_1024x768p_60)
	{
		//for 800x600 input 1024x768 output
		ch7026_write_reg( 0x4D, 0x05 );	//clk divided:	A1:0x0500_0000	//48.4k
		ch7026_write_reg( 0x4E, 0x00 );
		ch7026_write_reg( 0x4F, 0x00 );
		ch7026_write_reg( 0x50, 0x00 );
		if(ch7026.mode == TVOUT_VGA_800x600p_60)
		{
			ch7026_write_reg( 0x51, 0x40 );	//clk divided:	A2:0x40
		}
		else
		{
			ch7026_write_reg( 0x51, 0x48 );	//clk divided:	A2:0x40
		}
	}
	else
	{
		ch7026_write_reg( 0x4D, 0x03 );	//clk divided:	A1:0x030f_8e92	//37.8k
		ch7026_write_reg( 0x4E, 0x0F );
		ch7026_write_reg( 0x4F, 0x8E );
		ch7026_write_reg( 0x50, 0x92 );
		ch7026_write_reg( 0x51, 0x51 );	//clk divided:	A2:0x51
	}
	#endif

	ch7026_write_reg( 0x52, 0x1B );
 	ch7026_write_reg( 0x53, 0x12 );
 	ch7026_write_reg( 0x55, 0xE5 );
 	ch7026_write_reg( 0x5E, 0x80 );
	if(ch7026.mode == TVOUT_VGA_640x480p_60 && out_select == TVOUT_VGA_1024x768p_60)
	{
	}
	else
	{
 		ch7026_write_reg( 0x69, 0x64 );
	}
 	ch7026_write_reg( 0x7D, 0x62 );
	ch7026_write_reg( 0x04, 0x38 );
	ch7026_write_reg( 0x06, 0x69 );
#endif
#endif
/*
NOTE: The following five repeated sentences are used here to wait memory initial complete, please don't remove...(you could refer to Appendix A of programming guide document (CH7025(26)B Programming Guide Rev2.03.pdf or later version) for detailed information about memory initialization! 
*/
    ch7026_write_reg( 0x03, 0x00 );
    ch7026_write_reg( 0x03, 0x00 );
    ch7026_write_reg( 0x03, 0x00 );
    ch7026_write_reg( 0x03, 0x00 );
    ch7026_write_reg( 0x03, 0x00 );

    ch7026_write_reg( 0x06, 0x68 );
    ch7026_write_reg( 0x02, 0x02 );
    ch7026_write_reg( 0x02, 0x03 );
    ch7026_write_reg( 0x04, 0x00 );

	return 0;
}

static int vga_set_enable(struct ns115_display_device *device, int enable)
{
	DBG("%s  %d", __FUNCTION__, enable);
    
	if(vga_monspecs.enable != enable || (vga_monspecs.mode_set != ch7026.mode))
	{
		if(enable == 0 && vga_monspecs.enable)
		{
			vga_monspecs.enable = 0;
			ch7026_standby();
		}
		else if(enable == 1)
		{
			ch7026_switch_fb(vga_monspecs.mode, vga_monspecs.mode_set);
			ch7026_vga_init(vga_monspecs.mode);
			vga_monspecs.enable = 1;
		}
	}
	return 0;
}

static int vga_get_enable(struct ns115_display_device *device)
{
	DBG("%s start", __FUNCTION__);
    
	return vga_monspecs.enable;
}

static int vga_get_status(struct ns115_display_device *device)
{
	DBG("%s start", __FUNCTION__);
    
	if(ch7026.mode > TVOUT_CVBS_PAL)
		return 1;
	else
		return 0;
}

static int vga_get_modelist(struct ns115_display_device *device, struct list_head **modelist)
{
	DBG("%s start", __FUNCTION__);
    
	*modelist = &(vga_monspecs.modelist);
	return 0;
}

static int vga_set_mode(struct ns115_display_device *device, struct fb_videomode *mode)
{
	int i;

	DBG("%s start", __FUNCTION__);
    
	for(i = 0; i < ARRAY_SIZE(vga_mode); i++)
	{
		if(fb_mode_is_equal(&vga_mode[i], mode))
		{	
			if( ((i + 3) != ch7026.mode) )
			{
				vga_monspecs.mode_set = i + 3;
				vga_monspecs.mode = (struct fb_videomode *)&vga_mode[i];
			}
			return 0;
		}
	}
	return -1;
}

static int vga_get_mode(struct ns115_display_device *device, struct fb_videomode *mode)
{
	DBG("%s start", __FUNCTION__);
    
	*mode = *(vga_monspecs.mode);
	return 0;
}

static struct ns115_display_ops vga_display_ops = {
	.setenable = vga_set_enable,
	.getenable = vga_get_enable,
	.getstatus = vga_get_status,
	.getmodelist = vga_get_modelist,
	.setmode = vga_set_mode,
	.getmode = vga_get_mode,
};

static int display_vga_probe(struct ns115_display_device *device, void *devdata)
{
       DBG("%s start", __FUNCTION__);
       
	device->owner = THIS_MODULE;
	strcpy(device->type, "vga");
	device->name = "vga";
	device->priority = DISPLAY_PRIORITY_VGA;
	device->priv_data = devdata;
	device->ops = &vga_display_ops;

	return 1;
}

static struct ns115_display_driver display_vga = {
	.probe = display_vga_probe,
};

int ch7026_register_display_vga(struct device *parent)
{
	int i;

       DBG("%s start", __FUNCTION__);
	
	memset(&vga_monspecs, 0, sizeof(struct ch7026_monspecs));
    
	INIT_LIST_HEAD(&vga_monspecs.modelist);
	for(i = 0; i < ARRAY_SIZE(vga_mode); i++)
	{
		fb_add_videomode(&vga_mode[i], &vga_monspecs.modelist);
	}
	
	if(ch7026.mode > TVOUT_CVBS_PAL) {
		vga_monspecs.mode = (struct fb_videomode *)&(vga_mode[ch7026.mode - 3]);
		vga_monspecs.mode_set = ch7026.mode;
	}
	else {
		vga_monspecs.mode = (struct fb_videomode *)&(vga_mode[0]);
		vga_monspecs.mode_set = TVOUT_VGA_800x600p_60;
	}

	vga_monspecs.ddev = ns115_display_device_register(&display_vga, parent, &ch7026);
	if(vga_monspecs.ddev != NULL)
	{
		ch7026.vga = &vga_monspecs;
	}
	else
	{
		ch7026.vga = NULL;
		return -1;
	}

    return 0;
}


int ch7026_unregister_display_vga(struct ns115_display_device *device)
{
	ch7026.vga = NULL;
	ns115_display_device_unregister(device);
}



