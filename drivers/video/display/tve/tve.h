#ifndef __NS115_TVE_H__
#define __NS115_TVE_H__


#include <mach/gpio.h>
#include <linux/ns115_display-sys.h>
#include <linux/miscdevice.h>


#define TVE_MAGIC						'f'
#define TVE_DISABLE						_IOW(TVE_MAGIC, 0x01, unsigned int)
#define TVE_ENABLE						_IOW(TVE_MAGIC, 0x02, unsigned int)
#define TVE_SET_RESOLUTION_PAL			_IOW(TVE_MAGIC, 0x03, unsigned int)
#define TVE_SET_RESOLUTION_NTSC			_IOW(TVE_MAGIC, 0x04, unsigned int)
#define TVE_SET_RESOLUTION_800_600_60	_IOW(TVE_MAGIC, 0x05, unsigned int)
#define TVE_SET_RESOLUTION_1024_768_60	_IOW(TVE_MAGIC, 0x06, unsigned int)
#define TVE_STAT						_IOW(TVE_MAGIC, 0x10, unsigned int)

enum {
	STAT_CONNECT = 0,
	STAT_DISCONNECT,
	STAT_UNKNOW,
};


enum {
	TVOUT_CVBS_NTSC = 1,
	TVOUT_CVBS_PAL = 2,
	TVOUT_VGA_640x480p_60,
	TVOUT_VGA_800x600p_60,
	TVOUT_VGA_1024x768p_60,
	TVOUT_VGA_1024x600p_60,
//	TVOUT_YPbPr_720x480p_60,
//	TVOUT_YPbPr_720x576p_50,
//	TVOUT_YPbPr_1280x720p_50,
//	TVOUT_YPbPr_1280x720p_60,
};


#endif //__NS115_TVE_H__
