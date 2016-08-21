#ifndef __CH7026_H__
#define __CH7026_H__
#include <mach/gpio.h>
#include <linux/ns115_display-sys.h>
#include <linux/miscdevice.h>
#include "../tve.h"

#define INVALID_GPIO          -1
#define GPIO_LOW		    0
#define GPIO_HIGH		    1



/* stick_minor_display default output mode */
#define CH7026_DEFAULT_MODE		TVOUT_VGA_1024x600p_60


#define DEBUG

#ifdef DEBUG
#define JCG(frm,args...) 	printk(KERN_CRIT "----By James--[%s:%d]--<%s>--"frm,__FILE__,__LINE__,__func__,##args)
#define DBG(frm,args...) 	printk(KERN_CRIT "----By James--[%s:%d]--<%s>--"frm,__FILE__,__LINE__,__func__,##args)
#else
#define JCG(frm,args...) 	
#define DBG(format, ...)
#endif




struct stick_minor_display_monspecs {
	struct ns115_display_device	*ddev;
	unsigned int				enable;
	struct fb_videomode		*mode;
	struct list_head			modelist;
	unsigned int 				mode_set;
};

struct stick_minor_display {
	struct ns115_display_device *dev;
	struct miscdevice	miscdev;
	int video_source;
	int mode;
    struct stick_minor_display_monspecs *lcd;
};

extern struct stick_minor_display stick_minor_display;


extern int stick_minor_display_write_reg(char reg, char value);
extern int stick_minor_display_read_reg(char reg);
extern int stick_minor_display_switch_fb(const struct fb_videomode *modedb, int mode);
extern void stick_minor_display_standby(void);
extern int stick_minor_display_register_display_lcd(struct device *parent);
extern int stick_minor_display_unregister_display_lcd(struct ns115_display_device *device);
extern int lcd_set_enable(struct ns115_display_device *device, int enable);

#endif //__CH7026_H__
