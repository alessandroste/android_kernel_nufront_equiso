
#ifndef __CS8556_H__
#define __CS8556_H__



#include <mach/gpio.h>
#include <linux/ns115_display-sys.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
//#include "cs8556_regs.h"
#include <linux/miscdevice.h>
#include "../tve.h"
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/fcntl.h>



#define INVALID_GPIO          -1
#define GPIO_LOW		    0
#define GPIO_HIGH		    1


extern void CS8556_Initial(void);
extern void CS8556_SetOutputChannel(unsigned char channel);


/* CS8556 default output mode */
#define CS8556_UBOOT_SUPPORT
#ifdef CONFIG_CS8556_TVOUT_CVBS
	#define CS8556_DEFAULT_MODE		TVOUT_CVBS_PAL
#else
	#define CS8556_DEFAULT_MODE		TVOUT_VGA_800x600p_60
#endif

struct cs8556_monspecs {
       struct ns115_display_device	*ddev;
	unsigned int				enable;
	struct fb_videomode		*mode;
	struct list_head			modelist;
	unsigned int 				mode_set;
};

struct cs8556 {
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
	int 	chip_id;	//0x55 for ch7025; 0x54 for cs8556
	int		mode;
	struct cs8556_monspecs *cvbs;
	struct cs8556_monspecs *ypbpr;
    struct cs8556_monspecs *vga;
};

extern struct cs8556 cs8556;

//#define DEBUG

#ifdef DEBUG
#define JCG(frm,args...) 	printk(KERN_CRIT "----By James--[%s:%d]--<%s>--"frm,__FILE__,__LINE__,__func__,##args)
#define DBG(frm,args...) 	printk(KERN_CRIT "----By James--[%s:%d]--<%s>--"frm,__FILE__,__LINE__,__func__,##args)
//#define DBG(format, ...) \
//		printk(KERN_INFO "CS8556: " format "\n", ## __VA_ARGS__)
#else
#define DBG(format, ...)
#define JCG(format, ...)
#endif

extern int cs8556_write_reg(char reg, char value);
extern int cs8556_read_reg(char page,char saddr);
extern int cs8556_write_mutil_reg(char page, char saddr,char *value, int number);
extern void cs8556_standby(void);
extern int cs8556_switch_fb(const struct fb_videomode *modedb, int mode);
#ifdef CONFIG_CS8556_TVOUT_CVBS
extern int cs8556_register_display_cvbs(struct device *parent);
extern int cs8556_unregister_display_cvbs(struct ns115_display_device *device);
#endif
#ifdef CONFIG_CS8556_TVOUT_VGA
extern int cs8556_register_display_vga(struct device *parent);
extern int cs8556_unregister_display_vga(struct ns115_display_device *device);
#endif

#endif //__CS8556_H__
















