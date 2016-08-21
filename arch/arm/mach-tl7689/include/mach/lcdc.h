#ifndef _MACH_TL7689_LCDC_H_
#define _MACH_TL7689_LCDC_H_
/*extend this if needed*/
#if 0
struct lcdc_platform_ops
{
	int (*init)(void);
	int (*check_var)(struct fb_var_screeninfo * var);
	int (*set_var)(struct fb_var_screeninfo * var);
	int (*set_polarity)(unsigned int polarity);
	int (*disable)(void);
	int (*enable)(void);
};
#endif
struct lcdc_platform_data
{
	char name[16];			/* identification string eg "TT Builtin" */
	int id;
	int open;
	int cmap_len;
	int ddc_adapter;
	char *option;
	char *option_32;
	unsigned int height;                   /* height of picture in mm    */
	unsigned int width;                    /* width of picture in mm     */
	char *clk_name;

/*	struct lcdc_platform_pos * ops;*/
};
#endif

