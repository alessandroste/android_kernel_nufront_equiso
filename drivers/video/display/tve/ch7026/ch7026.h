#ifndef __CH7026_H__
#define __CH7026_H__
#include <mach/gpio.h>
#include <linux/ns115_display-sys.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include "ch7026_regs.h"
#include "../tve.h"
#include <linux/miscdevice.h>

#define INVALID_GPIO          -1
#define GPIO_LOW		    0
#define GPIO_HIGH		    1


#if 0
#define CH7026_MAGIC     'f'
#define CH7026_DISABLE   	_IOW(CH7026_MAGIC, 0x01, unsigned int)
#define CH7026_ENABLE   	_IOW(CH7026_MAGIC, 0x02, unsigned int)
#define CH7026_SET_RESOLUTION_PAL   _IOW(CH7026_MAGIC, 0x03, unsigned int)
#define CH7026_SET_RESOLUTION_NTSC   _IOW(CH7026_MAGIC, 0x04, unsigned int)
#define CH7026_SET_RESOLUTION_800_600_60   _IOW(CH7026_MAGIC, 0x05, unsigned int)


enum {
	TVOUT_CVBS_NTSC = 1,
	TVOUT_CVBS_PAL = 2,
//	TVOUT_YPbPr_720x480p_60,
//	TVOUT_YPbPr_720x576p_50,
//	TVOUT_YPbPr_1280x720p_50,
//	TVOUT_YPbPr_1280x720p_60,
	TVOUT_VGA_640x480p_60,
	TVOUT_VGA_800x600p_60,
	TVOUT_VGA_1024x768p_60,
	TVOUT_VGA_1024x600p_60,
};
#endif

/* CH7026 default output mode */
#if 0 //def CONFIG_CH7026_TVOUT_CVBS
	#define CH7026_DEFAULT_MODE		TVOUT_CVBS_PAL
#else
	#define CH7026_DEFAULT_MODE		TVOUT_VGA_1024x600p_60
#endif


//#define CH7026_UBOOT_SUPPORT


struct ch7026_monspecs {
       struct ns115_display_device	*ddev;
	unsigned int				enable;
	struct fb_videomode		*mode;
	struct list_head			modelist;
	unsigned int 				mode_set;
};

struct ch7026 {
	struct device		*dev;
	struct miscdevice	miscdev;
	struct i2c_client	*client;
       struct work_struct  work;
#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend	early_suspend;
#endif
	int		io_rst_pin;
    int     io_switch_pin;
    int     io_cvbs_det_pin;
       int  cvbs_det_irq; 
	int		video_source;
	int 	chip_id;	//0x55 for ch7025; 0x54 for ch7026
	int		mode;
	struct ch7026_monspecs *cvbs;
	struct ch7026_monspecs *ypbpr;
    struct ch7026_monspecs *vga;
};

extern struct ch7026 ch7026;

//#define DEBUG

#ifdef DEBUG
#define JCG(frm,args...) 	printk(KERN_CRIT "----By James--[%s:%d]--<%s>--"frm,__FILE__,__LINE__,__func__,##args)
#else
#define JCG(frm,args...) 	
#endif



#ifdef DEBUG
//#define DBG(format, ...) \
//		printk(KERN_INFO "CH7026: " format "\n", ## __VA_ARGS__)
#define DBG(frm,args...) 	printk(KERN_CRIT "----By James--[%s:%d]--<%s>--"frm,__FILE__,__LINE__,__func__,##args)
#else
#define DBG(format, ...)
#endif

extern int ch7026_write_reg(char reg, char value);
extern int ch7026_read_reg(char reg);
extern void ch7026_standby(void);
extern int ch7026_switch_fb(const struct fb_videomode *modedb, int mode);
#ifdef CONFIG_CH7026_TVOUT_CVBS
extern int ch7026_register_display_cvbs(struct device *parent);
extern int ch7026_unregister_display_cvbs(struct ns115_display_device *device);
#endif
#ifdef CONFIG_CH7026_TVOUT_VGA
extern int ch7026_register_display_vga(struct device *parent);
extern int ch7026_unregister_display_vga(struct ns115_display_device *device);
#endif

#endif //__CH7026_H__
