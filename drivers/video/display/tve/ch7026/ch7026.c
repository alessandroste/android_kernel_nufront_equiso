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
#include "ch7026.h"

#define DRV_NAME "ch7026"

#undef  dev_dbg
#define dev_dbg(dev, format, arg...) \
	dev_printk(KERN_CRIT, dev, format, ##arg)

#define DRIVER_VERSION		"1.0.0"


static bool enable_ch7026 = false;
struct ch7026 ch7026;



extern int ns115_fb_switch_screen(const struct fb_videomode *mode,int enable ,int id);


int ch7026_write_reg(char reg, char value)
{
    struct i2c_msg msg[1];
    unsigned char buf[2];
    int ret;
    
    if(ch7026.client == NULL)
        return -1;

    buf[0] = reg;
    buf[1] = value;
    msg[0].addr = ch7026.client->addr;
    msg[0].flags = ch7026.client->flags & I2C_M_TEN;
    msg[0].buf = buf;
    msg[0].len = 2;

    ret = i2c_transfer(ch7026.client->adapter, msg, ARRAY_SIZE(msg));
    if (ret < 0){
        dev_err(&ch7026.client->dev, "ch7026_write_reg:write transfer error\n");
    }

    return ret;
}

int ch7026_read_reg(char reg)
{
    struct i2c_msg msg[2];
    unsigned char data;
    int ret;
	
    if(ch7026.client == NULL)
        return -1;  

    msg[0].addr = ch7026.client->addr;
    msg[0].flags = ch7026.client->flags & I2C_M_TEN;
    msg[0].buf = &reg;
    msg[0].len = 1;
    msg[1].addr = ch7026.client->addr;
    msg[1].flags = (ch7026.client->flags & I2C_M_TEN) | I2C_M_RD;
    msg[1].buf = &data;
    msg[1].len = 1;

    ret = i2c_transfer(ch7026.client->adapter, msg, ARRAY_SIZE(msg));
    if (ret < 0){
        dev_err(&ch7026.client->dev, "ch7026_read_reg:read transfer error\n");
        return ret;
    }

    return data;
}

static int ch7026_connect_detect(void)
{
    unsigned char data = 0;
    
    ch7026_write_reg( 0x02, 0x01 );
    ch7026_write_reg( 0x02, 0x03 );
    ch7026_write_reg( 0x03, 0x00 ); 

    data |=(1<<3)|(1<<4)|(1<<5);
    ch7026_write_reg( 0x04, data ); 
    ch7026_write_reg( 0x7d, 0x01 );

    msleep(100);
    data=ch7026_read_reg(0x7f);
    DBG("%s DAC Connected detcet 0x%x", __FUNCTION__,data);

    if((data & 0x03) == 0x01)
        DBG("%s DAC0 Connected", __FUNCTION__);
    
    if(((data >> 2) & 0x03) == 0x01)
        DBG("%s DAC1 Connected", __FUNCTION__);
    
    if(((data >> 4) & 0x03) == 0x01)
        DBG("%s DAC2 Connected", __FUNCTION__);
    
    ch7026_write_reg( 0x7d, 0x01 );
    ch7026_write_reg( 0x04, 0x00 ); 
    return 1;
    
}

int ch7026_switch_fb(const struct fb_videomode *modedb, int mode)
{

    if(modedb == NULL)
		return -1;

    ch7026.mode = mode;

    ns115_fb_switch_screen(modedb, 1 , ch7026.video_source);

    if(ch7026.io_switch_pin != INVALID_GPIO) {
   		if(mode < TVOUT_VGA_640x480p_60)
		{
   			gpio_direction_output(ch7026.io_switch_pin, GPIO_LOW);
		}
   		else
		{
   			gpio_direction_output(ch7026.io_switch_pin, GPIO_HIGH);
		}
   	}
    
	return 0;  
}

static void ch7206_disable(void)
{
	DBG("%s start", __FUNCTION__);
    
	ch7026_write_reg( 0x04, v_PD_DAC(7) | v_PD_PWR(1) );
}

static void ch7206_enable(void)
{
	DBG("%s start", __FUNCTION__);
	ch7026_write_reg( 0x04, v_PD_DAC(0) | v_PD_PWR(0) );
}

void ch7026_standby(void)
{
	int cvbs = 0,vga = 0;
	
	if(ch7026.cvbs)
		cvbs = ch7026.cvbs->enable;
       if(ch7026.vga)
	    vga = ch7026.vga->enable;
       
	if(!cvbs && !vga)
		ch7206_disable();
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void ch7026_early_suspend(struct early_suspend *h)
{
	DBG("ch7026 enter early suspend");
    
	if(ch7026.cvbs)
		ch7026.cvbs->ddev->ops->setenable(ch7026.cvbs->ddev, 0);
       if(ch7026.vga)
             ch7026.vga->ddev->ops->setenable(ch7026.vga->ddev, 0);
       
	return;
}

static void ch7026_early_resume(struct early_suspend *h)
{
	DBG("ch7026 exit early resume");
    
#if 1
	if( ch7026.cvbs && (ch7026.mode < TVOUT_VGA_800x600p_60) ) {
		ns115_display_device_enable((ch7026.cvbs)->ddev);
	}
       else if(ch7026.vga && (ch7026.mode > TVOUT_CVBS_PAL)){
             ns115_display_device_enable((ch7026.vga)->ddev);
       }
#endif
    
	return;
}
#endif

static irqreturn_t ch7026_cvbs_detect_irq(int irq, void *_info)
{
       disable_irq_nosync(irq);
	schedule_work(&ch7026.work);

	return IRQ_HANDLED;
}

static void ch7026_cvbs_detect_irq_work(struct work_struct * work)
{
    unsigned long irqflags;
    int ret,cvbs_status,state_tmp;
    int deb_cnt=0;
        
    msleep(100);
//	cvbs_status = gpio_get_value(ch7026.io_cvbs_det_pin);
	if(ch7026.mode < TVOUT_VGA_640x480p_60)
		cvbs_status = 1;
	else
		cvbs_status = 0;
    for(deb_cnt = 0; deb_cnt < 5; )
    {
        msleep(60);
        state_tmp = gpio_get_value(ch7026.io_cvbs_det_pin);
	if(ch7026.mode < TVOUT_VGA_640x480p_60)
		state_tmp = 1;
	else
		state_tmp = 0;
        if(cvbs_status == state_tmp){
            deb_cnt++;
        }else{
            cvbs_status = state_tmp;
            deb_cnt = 0;
        }
    }

    if(cvbs_status)
    {
	    if(ch7026.mode != TVOUT_CVBS_NTSC && ch7026.mode !=TVOUT_CVBS_PAL)
		ch7026.mode = TVOUT_CVBS_PAL;
	    
    }
       else
            ch7026.mode = TVOUT_VGA_800x600p_60;

    if( ch7026.cvbs && (ch7026.mode < TVOUT_VGA_640x480p_60) )
		ns115_display_device_enable((ch7026.cvbs)->ddev);

    if( ch7026.vga && (ch7026.mode > TVOUT_CVBS_PAL) ) 
		ns115_display_device_enable((ch7026.cvbs)->ddev);
    
    irqflags = cvbs_status?IRQF_TRIGGER_FALLING:IRQF_TRIGGER_RISING;

    free_irq(ch7026.cvbs_det_irq, NULL);
    ret = request_threaded_irq(ch7026.cvbs_det_irq, NULL, ch7026_cvbs_detect_irq,
			irqflags, "ch7026_cvbs_detect", NULL);   
}

#if 1		//JamesLee
static int ch7026_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int ch7026_close(struct inode *inode, struct file *file)
{
	return 0;
}

static long ch7026_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret;
	int temp,temp_2;
	switch (cmd)
	{
		case	TVE_STAT:	// 6
			if(ch7026.cvbs)
				temp = ch7026.cvbs->ddev->ops->getenable(ch7026.cvbs->ddev);
			if(ch7026.vga)
				temp_2 = ch7026.vga->ddev->ops->getenable(ch7026.vga->ddev);
			return (temp | temp_2);
		case	TVE_DISABLE:
			JCG("disable CH7026");
			ch7206_disable();
			enable_ch7026 = false;
			break;
		case	TVE_ENABLE:
			ch7206_enable();
			JCG("enable CH7026");
	//		enable_ch7026 = true;
			break;
#ifdef CONFIG_CH7026_TVOUT_CVBS
		case	TVE_SET_RESOLUTION_PAL:
			JCG("Set resolution to PAL");
			ch7026.mode = TVOUT_CVBS_PAL;
			ch7026.vga->ddev->ops->setenable(ch7026.vga->ddev, 0);
			ch7026.cvbs->ddev->ops->setenable(ch7026.cvbs->ddev, 1);
			enable_ch7026 = true;
			break;
		case	TVE_SET_RESOLUTION_NTSC:
			JCG("Set resolution to NTSC");
			ch7026.mode = TVOUT_CVBS_NTSC;
			ch7026.vga->ddev->ops->setenable(ch7026.vga->ddev, 0);
			ch7026.cvbs->ddev->ops->setenable(ch7026.cvbs->ddev, 1);
			enable_ch7026 = true;
			break;
#endif
#ifdef CONFIG_CH7026_TVOUT_VGA
		case	TVE_SET_RESOLUTION_800_600_60:
			JCG("Set resolution to vga 800x600");
			ch7026.mode = TVOUT_VGA_1024x600p_60;
			ch7026.cvbs->ddev->ops->setenable(ch7026.cvbs->ddev, 0);
			ch7026.vga->ddev->ops->setenable(ch7026.vga->ddev, 1);
			enable_ch7026 = true;
			break;
#endif
		default:
			return -EINVAL;
		return 0;
	}
}

static const struct file_operations ch7026_fops = { 
	.owner          = THIS_MODULE,
	.open           = ch7026_open,
	.release        = ch7026_close,
	.unlocked_ioctl = ch7026_ioctl,
};


#endif




static int ch7026_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	struct ns115_display_platform_data *tv_data;
       int cvbs_status = 0;
       unsigned long irqflags = 0;

       printk(KERN_CRIT "[pchen]ch7026_probe %s\n",DRIVER_VERSION);
	memset(&ch7026, 0, sizeof(struct ch7026));
	ch7026.client = client;
	ch7026.dev = &client->dev;
	ch7026.mode = CH7026_DEFAULT_MODE;


	if(client->dev.platform_data) {
		tv_data = client->dev.platform_data;
		ch7026.io_rst_pin = tv_data->io_reset_pin;
		ch7026.io_switch_pin = tv_data->io_switch_pin;
		ch7026.io_cvbs_det_pin = tv_data->io_cvbs_det_pin;
		printk(KERN_CRIT"ch7026_probe: io_rst_pin %d,io_switch_pin %d,io_cvbs_det_pin %d",
		ch7026.io_rst_pin,ch7026.io_switch_pin,ch7026.io_cvbs_det_pin);
	}



	ch7026.miscdev.minor = 255;
	ch7026.miscdev.name = DRV_NAME;
	ch7026.miscdev.fops = &ch7026_fops;
	ch7026.miscdev.parent = ch7026.dev;
	ret = misc_register(&ch7026.miscdev);


	// Reset
	if(ch7026.io_rst_pin != INVALID_GPIO) {
		ret = gpio_request(ch7026.io_rst_pin, "tvout_rst");
		if(ret) {
			printk(KERN_ERR "ch7026 request reset gpio error\n");
			return -1;
		}
#ifndef CH7026_UBOOT_SUPPORT	//uboot reset the chip
		gpio_direction_output(ch7026.io_rst_pin, GPIO_LOW);
		msleep(1);
		gpio_direction_output(ch7026.io_rst_pin, GPIO_HIGH);
#endif
	}

       if(ch7026.io_switch_pin != INVALID_GPIO) {
		ret = gpio_request(ch7026.io_switch_pin, "tvout_sw");
		if(ret) {
			printk(KERN_ERR "ch7026 request video switch gpio error\n");
			return -1;
		}
	}

        if(ch7026.io_cvbs_det_pin != INVALID_GPIO) {
		ret = gpio_request(ch7026.io_cvbs_det_pin, "cvbs_det");
		if(ret) {
			printk(KERN_ERR "ch7026 request cvbs det gpio error\n");
			return -1;
		}
	}

	gpio_direction_input(ch7026.io_cvbs_det_pin);
#if 0
	cvbs_status = gpio_get_value(ch7026.io_cvbs_det_pin);
	printk(KERN_CRIT"ch7026_probe: cvbs_status = 0x%x", cvbs_status);
       if(cvbs_status){
	       if(ch7026.mode != TVOUT_CVBS_NTSC && ch7026.mode != TVOUT_CVBS_PAL)
	       {
            		printk(KERN_CRIT"ch7026_probe: output TVOUT_CVBS_NTSC");
            		ch7026.mode = TVOUT_CVBS_PAL;
	       }
	       else
            		printk(KERN_CRIT"ch7026_probe: output TVOUT_CVBS_PAL");
       }else{
            printk(KERN_CRIT"ch7026_probe: output TVOUT_VGA_800x600p_60");
            ch7026.mode = TVOUT_VGA_800x600p_60;
       }
#endif
/*
        if(ch7026->io_cvbs_det_pin != INVALID_GPIO) 
             ch7026.cvbs_det_irq = gpio_to_irq(tv_data->io_cvbs_det_pin);
        else
             goto  no_det_irq;

        ret = gpio_request(ch7026->io_cvbs_det_pin, "cvbs_det");
        if(ret < 0)
	{
		printk(KERN_ERR"ch7026 request cvbs det gpio failed \n");
		return -1;
	}
       gpio_direction_input(ch7026->io_cvbs_det_pin);
	cvbs_status = gpio_get_value(ch7026->io_cvbs_det_pin);
       irqflags = cvbs_status?IRQF_TRIGGER_FALLING:IRQF_TRIGGER_RISING;

       INIT_WORK(&ch7026->work, ch7026_cvbs_detect_irq_work);
	ret = request_threaded_irq(ch7026.cvbs_det_irq, NULL, ch7026_cvbs_detect_irq,
			irqflags, "ch7026_cvbs_detect", NULL);
	if (ret < 0) {
		dev_err(&ch7026->dev, "Can't get %d IRQ for ch7026 cvbs detect: %d\n",
				ch7026.cvbs_det_irq, ret);
        return -1;
	}

no_det_irq:  
*/
	//Read Chip ID
	ch7026.chip_id = ch7026_read_reg(DEVICE_ID);
	printk(KERN_CRIT"chip id is 0x%02x revision id %02x", ch7026.chip_id, ch7026_read_reg(REVISION_ID)&0xFF);

//        ch7026_connect_detect();
        
#ifdef CONFIG_CH7026_TVOUT_CVBS
	ch7026_register_display_cvbs(&client->dev);
	if( ch7026.cvbs && (ch7026.mode < TVOUT_VGA_800x600p_60) )
		ns115_display_device_enable((ch7026.cvbs)->ddev);
#endif

#ifdef CONFIG_CH7026_TVOUT_VGA
	ch7026_register_display_vga(&client->dev);
	if( ch7026.vga && (ch7026.mode > TVOUT_CVBS_PAL) )
		ns115_display_device_enable((ch7026.vga)->ddev);
#endif
	
#ifdef CONFIG_HAS_EARLYSUSPEND
	ch7026.early_suspend.suspend = ch7026_early_suspend;
	ch7026.early_suspend.resume = ch7026_early_resume;
	ch7026.early_suspend.level = EARLY_SUSPEND_LEVEL_DISABLE_FB - 10;
	register_early_suspend(&(ch7026.early_suspend));
#endif
	printk(KERN_INFO "ch7026 probe sucess.\n");

	return 0;
}

static int ch7026_remove(struct i2c_client *client)
{
	return 0;
}




static const struct i2c_device_id ch7026_id[] = {
	{ DRV_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ch7026_id);

static struct i2c_driver ch7026_driver = {
	.driver 	= {
		.name	= DRV_NAME,
	},
	.id_table = ch7026_id,
	.probe = ch7026_probe,
	.remove = ch7026_remove,
};


static int __init ch7026_init(void)
{
	int ret = 0;

       printk("ch7026_init++++++++\n");
       
	ret = i2c_add_driver(&ch7026_driver);
	if(ret < 0){
		printk("i2c_add_driver err, ret = %d\n", ret);
	}
	return ret;
}

static void __exit ch7026_exit(void)
{
    i2c_del_driver(&ch7026_driver);
}

module_init(ch7026_init);
module_exit(ch7026_exit);
