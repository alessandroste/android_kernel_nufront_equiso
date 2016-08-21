/*
 * A V4L2 driver for Micron gc0328 cameras.
 *
 * Based on gc0328 camera driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <media/v4l2-chip-ident.h>
#include <media/soc_camera.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/clk.h>

#include "gc0328_dvp.h"

#define DRIVER_NAME "gc0328"

//#define DEBUG_EN
#ifdef DEBUG_EN
#define  PDBG(format,...)               printk("[%d:%s]:"format, \
        __LINE__,__func__ , ##__VA_ARGS__ )
#else
#define  PDBG(format,...)               do{}while(0)
#endif

#define V4L2_CID_CAMERA_SPECIAL_EFFECT  (V4L2_CID_PRIVATE_BASE + 124)
#define V4L2_CID_CAMERA_FACING          (V4L2_CID_PRIVATE_BASE + 125)

#define to_gc0328(sd)       container_of(sd, struct gc0328_priv, subdev)

enum v4l2_camera_facing {
    CAMERA_FACING_BACK  = 0,
    CAMERA_FACING_FRONT = 1,
};

enum {
	SPEC_EFF_NORMAL = 0,	// "Normal",
	SPEC_EFF_BLUE,		//"Blueish",
	SPEC_EFF_RED,		//"Redish",
	SPEC_EFF_GREEN,		//"Greenish",
	SPEC_EFF_BLWH,		//"Black and white",
	SPEC_EFF_SEPIA,		//"Sepia",
	SPEC_EFF_NEG,		//"Negative",
	SPEC_EFF_OVEX,		//"Over exposure",
	SPEC_EFF_SOL,		//"Solarize",
};

enum {
    WB_AUTO = 0,
    WB_SUNNY,
    WB_OFFICE,
    WB_CLOUDY,
    WB_HOME,
};

struct gc0328_priv {
    struct v4l2_subdev          subdev;
    int                         ident;
    bool                        flag_vflip;
    bool                        flag_hflip;
    int                         brightness;
    int                         contrast;
    int                         sat;
    int                         effect;
    int                         wh_bal;     //white balance
    int                         inited;
};

static int gc0328_read(struct i2c_client *client, unsigned char reg,
        unsigned char *value)
{
    int ret;

    ret = i2c_smbus_read_byte_data(client, reg);
    if (ret >= 0) {
        *value = (unsigned char)ret;
        ret = 0;
    }
    return ret;
}


static int gc0328_write(struct i2c_client *client, unsigned char reg,
        unsigned char value)
{
    int ret = i2c_smbus_write_byte_data(client, reg, value);

    if (reg == REG_COM7 && (value & COM7_RESET))
        msleep(3);
    return ret;
}

static int gc0328_write_array(struct i2c_client *sd, struct regval_list *vals)
{
    int i = 0;
    while (vals->reg_num != 0xff || vals->value != 0xff) {
        int ret = gc0328_write(sd, vals->reg_num, vals->value);
        if (ret < 0)
            return ret;
        vals++;
        if (i == 0)
            mdelay(5);
        i++;
    }
    return 0;
}

/* Alter bus settings on camera side */
static int gc0328_set_bus_param(struct soc_camera_device *icd,
                unsigned long flags)
{
    return 0;
}

/* Request bus settings on camera side */
static unsigned long gc0328_query_bus_param(struct soc_camera_device *icd)
{
    struct soc_camera_link *icl = to_soc_camera_link(icd);
	struct v4l2_mbus_config cfg = {.type = V4L2_MBUS_PARALLEL,};

	cfg.flags = V4L2_MBUS_PCLK_SAMPLE_RISING | V4L2_MBUS_MASTER |
		V4L2_MBUS_VSYNC_ACTIVE_LOW | V4L2_MBUS_HSYNC_ACTIVE_HIGH |
		V4L2_MBUS_DATA_ACTIVE_HIGH;

	return soc_camera_apply_board_flags(icl, &cfg);

}

static const struct v4l2_queryctrl gc0328_controls[] = {
    {
        .id         = V4L2_CID_VFLIP,
        .type       = V4L2_CTRL_TYPE_BOOLEAN,
        .name       = "Flip Vertically",
        .minimum    = 0,
        .maximum    = 1,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id         = V4L2_CID_HFLIP,
        .type       = V4L2_CTRL_TYPE_BOOLEAN,
        .name       = "Flip Horizontally",
        .minimum    = 0,
        .maximum    = 1,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id         = V4L2_CID_CAMERA_FACING,
        .type       = V4L2_CTRL_TYPE_INTEGER,
        .name       = "Camera Facing",
        .minimum    = 0,
        .maximum    = 1,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id         = V4L2_CID_BRIGHTNESS,
        .type       = V4L2_CTRL_TYPE_INTEGER,
        .name       = "Brightness",
        .minimum    = -4,
        .maximum    = 4,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id         = V4L2_CID_CONTRAST,
        .type       = V4L2_CTRL_TYPE_INTEGER,
        .name       = "Contrast",
        .minimum    = -3,
        .maximum    = 3,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id         = V4L2_CID_SATURATION,
        .type       = V4L2_CTRL_TYPE_INTEGER,
        .name       = "Saturation",
        .minimum    = -3,
        .maximum    = 3,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id         = V4L2_CID_CAMERA_SPECIAL_EFFECT,
        .type       = V4L2_CTRL_TYPE_MENU,
        .name       = "Special Effect",
        .minimum    = 0,
        .maximum    = 8,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id         = V4L2_CID_DO_WHITE_BALANCE,
        .type       = V4L2_CTRL_TYPE_MENU,
        .name       = "White balance(light mode)",
        .minimum    = 0,
        .maximum    = 4,
        .step       = 1,
        .default_value  = 0,
    },
};

static int gc0328_suspend(struct soc_camera_device *icd, pm_message_t state)
{
    struct v4l2_subdev *sd;
    struct gc0328_priv *priv;

    sd = soc_camera_to_subdev(icd);
    priv = container_of(sd, struct gc0328_priv, subdev);

    return 0;
}

static int gc0328_resume(struct soc_camera_device *icd)
{
    struct v4l2_subdev *sd;
    struct gc0328_priv *priv;

    sd = soc_camera_to_subdev(icd);
    priv = container_of(sd, struct gc0328_priv, subdev);

    priv->flag_vflip = 0;
    priv->flag_hflip = 0;
    priv->brightness = 0;
    priv->contrast= 0;
    priv->sat= 0;
    priv->effect = 0;
    priv->wh_bal = 0;

    priv->inited = 0;

    return 0;
}

static struct soc_camera_ops gc0328_ops = {
    .set_bus_param      = gc0328_set_bus_param,
    .query_bus_param    = gc0328_query_bus_param,
    .suspend        = gc0328_suspend,
    .resume         = gc0328_resume,
    .controls       = gc0328_controls,
    .num_controls       = ARRAY_SIZE(gc0328_controls),
};

static int gc0328_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
    struct gc0328_priv *priv = to_gc0328(sd);

    switch (ctrl->id) {
    case V4L2_CID_VFLIP:
        ctrl->value = priv->flag_vflip;
        break;
    case V4L2_CID_HFLIP:
        ctrl->value = priv->flag_hflip;
        break;
    case V4L2_CID_CAMERA_FACING:
        ctrl->value = CAMERA_FACING_FRONT;
        break;
    case V4L2_CID_BRIGHTNESS:
        ctrl->value = priv->brightness;
        break;
    case V4L2_CID_CONTRAST:
        ctrl->value = priv->contrast;
        break;
    case V4L2_CID_SATURATION:
        ctrl->value = priv->sat;
        break;
    case V4L2_CID_CAMERA_SPECIAL_EFFECT:
        ctrl->value = priv->effect;
        break;
    case V4L2_CID_DO_WHITE_BALANCE:
        ctrl->value = priv->wh_bal;
        break;
    default:
        return -EINVAL;
    }

    return 0;
}

static int set_flip(struct i2c_client * client, int is_on)
{
    u8 val;
    int ret = 0;

    ret = gc0328_write(client, 0xfe, 0x00);
    if(ret < 0)
        return ret;

    ret = gc0328_read(client, 0x17, &val);
    if(ret < 0)
        return ret;
    if(is_on) {
        val |= 0x02;
    } else {
        val &= ~0x02;
    }

    ret = gc0328_write(client, 0x17, val);
    if(ret < 0)
        return ret;

    return 0;
}

static int set_mirror(struct i2c_client * client, int is_on)
{
    u8 val;
    int ret = 0;

    ret = gc0328_write(client, 0xfe, 0x00);
    if(ret < 0)
        return ret;

    ret = gc0328_read(client, 0x17, &val);
    if(ret < 0)
        return ret;
    if(is_on) {
        val |= 0x01;
    } else {
        val &= ~0x01;
    }

    ret = gc0328_write(client, 0x17, val);
    if(ret < 0)
        return ret;

    return 0;
}

static int set_brightness(struct i2c_client *client, int val)
{

    int ret = 0;

    if(val < -5 || val > 5)
        return -EINVAL;

    switch(val) {
    case -4 :
        ret = gc0328_write_array(client, BtSeqe[0]);
        break;
    case -3 :
        ret = gc0328_write_array(client, BtSeqe[1]);
        break;
    case -2 :
        ret = gc0328_write_array(client, BtSeqe[2]);
        break;
    case -1 :
        ret = gc0328_write_array(client, BtSeqe[3]);
        break;
    case 0 :
        ret = gc0328_write_array(client, BtSeqe[4]);
        break;
    case 1 :
        ret = gc0328_write_array(client, BtSeqe[5]);
        break;
    case 2 :
        ret = gc0328_write_array(client, BtSeqe[6]);
        break;
    case 3 :
        ret = gc0328_write_array(client, BtSeqe[7]);
        break;
    case 4 :
        ret = gc0328_write_array(client, BtSeqe[8]);
        break;
    default:
        ret = -1;
    }

    return 0;
}

static int set_contrast(struct i2c_client *client, int val)
{
    return 0;
}

static int set_sat(struct i2c_client *client, int val)
{
    return 0;
}

static int set_effect(struct i2c_client *client, int val)
{
	int ret = 0;

	if(val < 0 || val > 8)
		return -EINVAL;

	switch (val) {
	case SPEC_EFF_NORMAL:
		ret = gc0328_write(client, 0x43, 0x00);
		break;
	case SPEC_EFF_SEPIA:
		ret = gc0328_write(client, 0x43, 0x02);
		ret |= gc0328_write(client, 0xda, 0xd0);
		ret |= gc0328_write(client, 0xdb, 0x28);
		break;
	case SPEC_EFF_NEG:
		ret = gc0328_write(client, 0x43, 0x01);
		break;
	case SPEC_EFF_GREEN:
		ret = gc0328_write(client, 0x43, 0x02);
		ret |= gc0328_write(client, 0xda, 0xc0);
		ret |= gc0328_write(client, 0xdb, 0xc0);
		break;
	case SPEC_EFF_BLUE:
		ret = gc0328_write(client, 0x43, 0x02);
		ret |= gc0328_write(client, 0xda, 0x50);
		ret |= gc0328_write(client, 0xdb, 0xe0);
		break;
	case SPEC_EFF_BLWH:
		ret = gc0328_write(client, 0x43, 0x02);
		ret |= gc0328_write(client, 0xda, 0x00);
		ret |= gc0328_write(client, 0xdb, 0x00);
		break;
	default:
		ret = -1;
	}

	return ret;
}

static int set_wh_bal(struct i2c_client *client, int val)
{
    int ret = 0;

    if(val < 0 || val > 4)
        return -EINVAL;

    switch(val) {
    case WB_AUTO :
        ret = gc0328_write_array(client, WBSeqe[0]);
        break;
    case WB_SUNNY :
        ret = gc0328_write_array(client, WBSeqe[2]);
        break;
    case WB_CLOUDY :
        ret = gc0328_write_array(client, WBSeqe[3]);
        break;
    case WB_HOME :
        ret = gc0328_write_array(client, WBSeqe[1]);
        break;
    case WB_OFFICE :
        ret = gc0328_write_array(client, WBSeqe[4]);
        break;
    default:
        ret = -1;
    }

    return ret;
}

static int gc0328_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
    struct gc0328_priv *priv = to_gc0328(sd);
    struct i2c_client *client =
    (struct i2c_client *) v4l2_get_subdevdata(sd);

    switch (ctrl->id) {
    case V4L2_CID_VFLIP:
        priv->flag_vflip = ctrl->value;
        return set_flip(client, priv->flag_vflip);
        break;
    case V4L2_CID_HFLIP:
        priv->flag_hflip = ctrl->value;
        return set_mirror(client, priv->flag_hflip);
        break;
    case V4L2_CID_BRIGHTNESS:
        if (ctrl->value < -4 || ctrl->value > 4) {
            return -EINVAL;
        }
        priv->brightness = ctrl->value;
        return set_brightness(client, priv->brightness);
                break;
    case V4L2_CID_CONTRAST:
        if (ctrl->value < -3 || ctrl->value > 3) {
            return -EINVAL;
        }
        priv->contrast = ctrl->value;
        return set_contrast(client, priv->contrast);
                break;
    case V4L2_CID_SATURATION:
        if (ctrl->value < -3 || ctrl->value > 3) {
            return -EINVAL;
        }
        priv->sat = ctrl->value;
        return set_sat(client, priv->sat);
                break;
    case V4L2_CID_CAMERA_SPECIAL_EFFECT:
        if (ctrl->value < 0 || ctrl->value > 8) {
            return -EINVAL;
        }
        priv->effect = ctrl->value;
        return set_effect(client, priv->effect);
                break;
    case V4L2_CID_DO_WHITE_BALANCE:
        if (ctrl->value < 0 || ctrl->value > 4) {
            return -EINVAL;
        }
        priv->wh_bal = ctrl->value;
        return set_wh_bal(client, priv->wh_bal);
                break;
    default:
        return -EINVAL;
    }

    return 0;
}

static int gc0328_g_chip_ident(struct v4l2_subdev *sd,
                   struct v4l2_dbg_chip_ident *id)
{
    struct gc0328_priv *priv = to_gc0328(sd);

    id->ident = priv->ident;
    return 0;
}

static struct v4l2_subdev_core_ops gc0328_core_ops = {
    .g_ctrl         = gc0328_g_ctrl,
    .s_ctrl         = gc0328_s_ctrl,
    .g_chip_ident       = gc0328_g_chip_ident,
};

static int gc0328_s_stream(struct v4l2_subdev *sd, int enable)
{
    struct i2c_client *client = v4l2_get_subdevdata(sd);
    struct gc0328_priv *priv = to_gc0328(sd);
    int ret = 0;

    ret = set_flip(client, priv->flag_vflip);
    if (ret < 0)
        return ret;

    ret = set_brightness(client, priv->brightness);
    if (ret < 0)
        return ret;

    ret = set_contrast(client, priv->contrast);
    if (ret < 0)
        return ret;

    ret = set_sat(client, priv->sat);
    if (ret < 0)
        return ret;

    ret = set_effect(client, priv->effect);
    if (ret < 0)
        return ret;

    ret = set_wh_bal(client, priv->wh_bal);
    if (ret < 0)
        return ret;

    ret = set_mirror(client, priv->flag_hflip);
    if (ret < 0)
        return ret;

    if (enable) {
        ret = gc0328_write(client, 0x25, 0x0f);
        if (ret)
            goto out;
    } else {
        ret = gc0328_write(client, 0x25, 0x00);
    }

out:
    return ret;
}

static void gc0328_res_roundup(u32 *width, u32 *height)
{
    if(*width <= 320) {
        *width = 320;
        *height = 240;
    } else {
        *width = 640;
        *height = 480;
    }
}

static int gc0328_set_res(struct i2c_client *client, u32 width)
{
    int ret;

    if (width == 640) {
        ret = gc0328_write_array(client, gc0328_fmt_vga);
    } else if (width == 320) {
        ret = gc0328_write_array(client, gc0328_fmt_qvga);
    } else {
        dev_err(&client->dev, "Failed to select resolution!\n");
        return -EINVAL;
    }

    return ret;
}

static int gc0328_s_fmt(struct v4l2_subdev *sd,
            struct v4l2_mbus_framefmt *mf)
{
    struct i2c_client *client = v4l2_get_subdevdata(sd);
    struct gc0328_priv *priv = to_gc0328(sd);
    enum v4l2_colorspace cspace;
    enum v4l2_mbus_pixelcode code = mf->code;
    int ret;

    gc0328_res_roundup(&mf->width, &mf->height);
    switch (code) {
    case V4L2_MBUS_FMT_YUYV8_2X8:
        cspace = V4L2_COLORSPACE_SRGB;
        break;
    default:
        return -EINVAL;
    }

    if(priv->inited == 0) {
        ret = gc0328_write_array(client, gc0328_init_regs);
        if (ret < 0) {
            return ret;
        }
        priv->inited = 1;
    }

    ret = gc0328_set_res(client, mf->width);
    if (ret < 0)
        return ret;

    mf->code    = code;
    mf->colorspace  = cspace;

    return ret;
}

static int gc0328_try_fmt(struct v4l2_subdev *sd,
              struct v4l2_mbus_framefmt *mf)
{
    gc0328_res_roundup(&mf->width, &mf->height);

    mf->field = V4L2_FIELD_NONE;
    mf->code = V4L2_MBUS_FMT_YUYV8_2X8;
    mf->colorspace = V4L2_COLORSPACE_SRGB;

    return 0;
}

static enum v4l2_mbus_pixelcode gc0328_codes[] = {
    V4L2_MBUS_FMT_YUYV8_2X8,
};

static int gc0328_enum_fmt(struct v4l2_subdev *sd, unsigned int index,
               enum v4l2_mbus_pixelcode *code)
{
    if (index >= ARRAY_SIZE(gc0328_codes))
        return -EINVAL;

    *code = gc0328_codes[index];

    return 0;
}

static int gc0328_cropcap(struct v4l2_subdev *sd, struct v4l2_cropcap *a)
{
    a->bounds.left      = 0;
    a->bounds.top       = 0;
    a->bounds.width     = 640;
    a->bounds.height    = 480;
    a->defrect      = a->bounds;
    a->type         = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    a->pixelaspect.numerator    = 1;
    a->pixelaspect.denominator  = 1;

    return 0;
}

static int gc0328_g_crop(struct v4l2_subdev *sd, struct v4l2_crop *a)
{
    a->c.left       = 0;
    a->c.top        = 0;
    a->c.width      = 640;
    a->c.height     = 480;
    a->type         = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    return 0;
}

static struct v4l2_subdev_video_ops gc0328_video_ops = {
    .s_stream       = gc0328_s_stream,
    .s_mbus_fmt     = gc0328_s_fmt,
    .try_mbus_fmt   = gc0328_try_fmt,
    .enum_mbus_fmt  = gc0328_enum_fmt,
    .cropcap        = gc0328_cropcap,
    .g_crop         = gc0328_g_crop,
};

static struct v4l2_subdev_ops gc0328_subdev_ops = {
    .core           = &gc0328_core_ops,
    .video          = &gc0328_video_ops,
};

static int gc0328_video_probe(struct soc_camera_device *icd,
                  struct i2c_client *client)
{
    struct v4l2_subdev *sd = i2c_get_clientdata(client);
    struct gc0328_priv *priv = to_gc0328(sd);
    char id;
    int ret;

    /*
     * We must have a parent by now. And it cannot be a wrong one.
     * So this entire test is completely redundant.
     */
    if (!icd->parent ||
        to_soc_camera_host(icd->parent)->nr != icd->iface) {
        dev_err(&client->dev, "Parent missing or invalid!\n");
        ret = -ENODEV;
        goto err;
    }

    ret = gc0328_read(client, REG_PID, &id);
    if (ret < 0)
        goto err;
    if (id != 0x9d)
        goto err;

    priv->ident = V4L2_IDENT_GC2035;
    printk("gc0328 product ID 0x%02x\n", id);
err:
    return ret;
}

#ifdef DEBUG_EN
static u8 dbg_addr = 0;

static ssize_t show_addr(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "0x%02X\n", dbg_addr);
}

static ssize_t store_addr(struct device *dev, struct device_attribute *attr,
                const char *buf, size_t n)
{
    dbg_addr = simple_strtol(buf, NULL, 16);
    return n;
}

static ssize_t show_val(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct i2c_client *client = to_i2c_client(dev);
    u8 val;
    int ret = 0;

    ret = gc0328_read(client, dbg_addr, &val);

    if(ret < 0){
        dev_err(&client->dev, "show_val: reg_read failed\n");
        return ret;
    }

    return sprintf(buf, "0x%02X\n", val);
}

static ssize_t store_val(struct device *dev, struct device_attribute *attr,
                    const char *buf, size_t n)
{
    struct i2c_client *client = to_i2c_client(dev);
    u8 val;
    int ret = 0;

    val = simple_strtol(buf, NULL, 16);

    ret = gc0328_write(client, dbg_addr, val);

    if(ret < 0){
        dev_err(&client->dev, " store_val: reg_write failed\n");
        return ret;
    }

    return n;
}

static ssize_t store_test(struct device *dev, struct device_attribute *attr,
                const char *buf, size_t n)
{
    return n;
}

static DEVICE_ATTR(addr, S_IRUGO | S_IWUGO, show_addr, store_addr);
static DEVICE_ATTR(val, S_IRUGO | S_IWUGO, show_val, store_val);
static DEVICE_ATTR(test, S_IWUGO, NULL, store_test);

static struct attribute *dev_attributes[] = {
    &dev_attr_addr.attr,
    &dev_attr_val.attr,
    &dev_attr_test.attr,
    NULL
};

static struct attribute_group dev_attr_group = {
    .attrs = dev_attributes,
};
#endif

/*
 * i2c_driver function
 */
static int gc0328_probe(struct i2c_client *client,
            const struct i2c_device_id *did)
{
    struct gc0328_priv *priv;
    struct soc_camera_device *icd   = client->dev.platform_data;
    struct soc_camera_link *icl;
    int ret;

    if (!icd) {
        dev_err(&client->dev, "Missing soc-camera data!\n");
        return -EINVAL;
    }

    icl = to_soc_camera_link(icd);
    if (!icl) {
        dev_err(&client->dev, "Missing platform_data for driver\n");
        return -EINVAL;
    }

    priv = kzalloc(sizeof(struct gc0328_priv), GFP_KERNEL);
    if (!priv) {
        dev_err(&client->dev, "Failed to allocate private data!\n");
        return -ENOMEM;
    }

    v4l2_i2c_subdev_init(&priv->subdev, client, &gc0328_subdev_ops);
    icd->ops = &gc0328_ops;
    ret = gc0328_video_probe(icd, client);
    if (ret < 0) {
        goto err_probe;
    }

#ifdef DEBUG_EN
    sysfs_create_group(&client->dev.kobj, &dev_attr_group);
#endif

    return 0;

err_probe:
    icd->ops = NULL;
    kfree(priv);

    return ret;
}

static int gc0328_remove(struct i2c_client *client)
{
    struct gc0328_priv *priv = i2c_get_clientdata(client);

    if(!priv) {
        return 0;
    }

#ifdef DEBUG_EN
    sysfs_remove_group(&client->dev.kobj, &dev_attr_group);
#endif

    kfree(priv);

    return 0;
}

static const struct i2c_device_id gc0328_id[] = {
    { DRIVER_NAME, 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, gc0328_id);

static struct i2c_driver gc0328_i2c_driver = {
    .driver = {
        .name = DRIVER_NAME,
    },
    .probe    = gc0328_probe,
    .remove   = gc0328_remove,
    .id_table = gc0328_id,
};

static int __init gc0328_module_init(void)
{
    return i2c_add_driver(&gc0328_i2c_driver);
}

static void __exit gc0328_module_exit(void)
{
    i2c_del_driver(&gc0328_i2c_driver);
}

module_init(gc0328_module_init);
module_exit(gc0328_module_exit);

MODULE_DESCRIPTION("SoC Camera driver for gc0328");
MODULE_AUTHOR("nufront@nusmart.com");
MODULE_LICENSE("GPL v2");


