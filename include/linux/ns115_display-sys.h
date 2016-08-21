#ifndef _NS115_DISPLAY_H
#define _NS115_DISPLAY_H
#include <linux/device.h>
#include <linux/fb.h>
#include <linux/list.h>

struct ns115_display_device;

enum ns115_display_priority {
	DISPLAY_PRIORITY_TV = 0,
	DISPLAY_PRIORITY_YPbPr,
	DISPLAY_PRIORITY_VGA,
	DISPLAY_PRIORITY_HDMI,
	DISPLAY_PRIORITY_LCD,
};

enum ns115_display_source {
	DISPLAY_SOURCE_LCDC0 = 0,
	DISPLAY_SOURCE_LCDC1,
};

/* This structure defines all the properties of a Display. */
struct ns115_display_driver {
	void (*suspend)(struct ns115_display_device *, pm_message_t state);
	void (*resume)(struct ns115_display_device *);
	int  (*probe)(struct ns115_display_device *, void *);
	int  (*remove)(struct ns115_display_device *);
};

struct ns115_display_ops {
	int (*setenable)(struct ns115_display_device *, int enable);
	int (*getenable)(struct ns115_display_device *);
	int (*getstatus)(struct ns115_display_device *);
	int (*getmodelist)(struct ns115_display_device *, struct list_head **modelist);
	int (*setmode)(struct ns115_display_device *, struct fb_videomode *mode);
	int (*getmode)(struct ns115_display_device *, struct fb_videomode *mode);
};

struct ns115_display_device {
	struct module *owner;			/* Owner module */
	struct ns115_display_driver *driver;
	struct device *parent;			/* This is the parent */
	struct device *dev;			/* This is this display device */
	struct mutex lock;
	void *priv_data;
	char type[16];
	char *name;
	int idx;
	struct ns115_display_ops *ops;
	int priority;
	struct list_head list;
};

struct ns115_display_devicelist {
	struct list_head list;
	struct ns115_display_device *dev;
};

static struct ns115_display_platform_data 
{
	int video_source;		//display screen video source
	int io_reset_pin;        //reset control gpio
       int io_switch_pin;      //cvbs/vga output switch gpio
       int io_cvbs_det_pin;  //cvbs det pin
};

extern struct ns115_display_device *ns115_display_device_register(struct ns115_display_driver *driver,
					struct device *dev, void *devdata);
extern void ns115_display_device_unregister(struct ns115_display_device *dev);
extern void ns115_display_device_enable(struct ns115_display_device *ddev);
extern void ns115_display_device_select(int property, int priority);

#define to_ns115_display_device(obj) container_of(obj, struct ns115_display_device, class_dev)

#endif