/*
 * A V4L2 driver for Micron GC2035 cameras.
 *
 * Based on GC2035 camera driver.
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

#include "gc2035_dvp.h"

#define DRIVER_NAME "gc2035"

//#define DEBUG_EN
#ifdef DEBUG_EN
#define  PDBG(format,...)               printk("[%d:%s]:"format, \
        __LINE__,__func__ , ##__VA_ARGS__ )
#else
#define  PDBG(format,...)               do{}while(0)
#endif

#define V4L2_CID_CAMERA_SPECIAL_EFFECT  (V4L2_CID_PRIVATE_BASE + 124)
#define V4L2_CID_CAMERA_FACING      (V4L2_CID_PRIVATE_BASE + 125)
#define V4L2_CID_PRE_CAP        (V4L2_CID_PRIVATE_BASE + 127)


#define to_gc2035(sd)       container_of(sd, struct gc2035_priv, subdev)

enum v4l2_camera_facing {
    CAMERA_FACING_BACK  = 0,
    CAMERA_FACING_FRONT = 1,
};

enum {
    SPEC_EFF_NORMAL = 0,    // "Normal",
    SPEC_EFF_BLUE,          //"Blueish",
    SPEC_EFF_RED,           //"Redish",
    SPEC_EFF_GREEN,         //"Greenish",
    SPEC_EFF_BLWH,          //"Black and white",
    SPEC_EFF_SEPIA,         //"Sepia",
    SPEC_EFF_NEG,           //"Negative",
    SPEC_EFF_OVEX,          //"Over exposure",
    SPEC_EFF_SOL,           //"Solarize",
};

/* supported resolutions */
enum {
    GC2035_QCIF,
    GC2035_QVGA,
    GC2035_CIF,
    GC2035_VGA,
    GC2035_720P,
    GC2035_2M,
};

struct gc2035_resolution {
    unsigned int width;
    unsigned int height;
};

static struct gc2035_resolution gc2035_resolutions[] = {
    [GC2035_QCIF] = {
        .width  = 176,
        .height = 144,
    },
    [GC2035_QVGA] = {
        .width  = 320,
        .height = 240,
    },
    [GC2035_CIF] = {
        .width  = 352,
        .height = 288,
    },
    [GC2035_VGA] = {
        .width  = 640,
        .height = 480,
    },
    [GC2035_720P] = {
        .width  = 1280,
        .height = 720,
    },
    [GC2035_2M] = {
        .width  = 1600,
        .height = 1200,
    },
};

enum {
    WB_AUTO = 0,
    WB_SUNNY,
    WB_OFFICE,
    WB_CLOUDY,
    WB_HOME,
};

struct gc2035_priv {
    struct v4l2_subdev          subdev;

    int                 ident;
    u16                 model;
    u8                  revision;

    bool                afc_fw_ready;
    bool                af_result;
    bool                flag_vflip;
    bool                flag_hflip;
    int                 brightness;
    int                 contrast;
    int                 effect;
    int                 wh_bal;     //white balance
    int                 ev;
    int                 sat;        //saturation
    int                 res;
    int                 fps;

    int                 f_cap;
    int                 shutter;
    int                 gain16;
    int                 average;
};

static int gc2035_read(struct i2c_client *client, unsigned char reg,
        unsigned char *value)
{
    int ret = -EAGAIN, cnt = 3;

    while((cnt-- > 0) && (ret < 0)) {
        ret = i2c_smbus_read_byte_data(client, reg);
        if (ret >= 0) {
            *value = (unsigned char)ret;
            return 0;
        } else {
            dev_err(&client->dev, "Failed reading register 0x%02x, val:0x%02x!\n",
                    reg, *value);
        }
    }
    return ret;

}


static int gc2035_write(struct i2c_client *client, unsigned char reg,
        unsigned char value)
{
   int ret = -EAGAIN, cnt = 3;

    while((cnt-- > 0) && (ret < 0)) {
        int ret = i2c_smbus_write_byte_data(client, reg, value);
        if (ret >= 0) {
            if (reg == REG_COM7 && (value & COM7_RESET))
                msleep(3);  /* Wait for reset to run */
            return 0;
        } else {
            dev_err(&client->dev, "Failed writing register 0x%02x, val:0x%02x!\n",
                    reg, value);
        }
    }
        return ret;
}

static int gc2035_write_array(struct i2c_client *sd, struct regval_list *vals)
{
    int i = 0;
    while (vals->reg_num != 0xff || vals->value != 0xff) {
        int ret = gc2035_write(sd, vals->reg_num, vals->value);
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
static int gc2035_set_bus_param(struct soc_camera_device *icd,
                unsigned long flags)
{
    return 0;
}

/* Request bus settings on camera side */
static unsigned long gc2035_query_bus_param(struct soc_camera_device *icd)
{
    struct soc_camera_link *icl = to_soc_camera_link(icd);
	struct v4l2_mbus_config cfg = {.type = V4L2_MBUS_PARALLEL,};

	cfg.flags = V4L2_MBUS_PCLK_SAMPLE_RISING | V4L2_MBUS_MASTER |
		V4L2_MBUS_VSYNC_ACTIVE_LOW | V4L2_MBUS_HSYNC_ACTIVE_HIGH |
		V4L2_MBUS_DATA_ACTIVE_HIGH;

	return soc_camera_apply_board_flags(icl, &cfg);

}

static const struct v4l2_queryctrl gc2035_controls[] = {
    {
        .id     = V4L2_CID_VFLIP,
        .type       = V4L2_CTRL_TYPE_BOOLEAN,
        .name       = "Flip Vertically",
        .minimum    = 0,
        .maximum    = 1,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id     = V4L2_CID_HFLIP,
        .type       = V4L2_CTRL_TYPE_BOOLEAN,
        .name       = "Flip Horizontally",
        .minimum    = 0,
        .maximum    = 1,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id     = V4L2_CID_CAMERA_FACING,
        .type       = V4L2_CTRL_TYPE_INTEGER,
        .name       = "Camera Facing",
        .minimum    = 0,
        .maximum    = 1,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id     = V4L2_CID_BRIGHTNESS,
        .type       = V4L2_CTRL_TYPE_INTEGER,
        .name       = "Brightness",
        .minimum    = -4,
        .maximum    = 4,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id     = V4L2_CID_CONTRAST,
        .type       = V4L2_CTRL_TYPE_INTEGER,
        .name       = "Contrast",
        .minimum    = -3,
        .maximum    = 3,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id     = V4L2_CID_SATURATION,
        .type       = V4L2_CTRL_TYPE_INTEGER,
        .name       = "Saturation",
        .minimum    = -3,
        .maximum    = 3,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id     = V4L2_CID_CAMERA_SPECIAL_EFFECT,
        .type       = V4L2_CTRL_TYPE_MENU,
        .name       = "Special Effect",
        .minimum    = 0,
        .maximum    = 8,
        .step       = 1,
        .default_value  = 0,
    },
    {
        .id     = V4L2_CID_DO_WHITE_BALANCE,
        .type       = V4L2_CTRL_TYPE_MENU,
        .name       = "White balance(light mode)",
        .minimum    = 0,
        .maximum    = 4,
        .step       = 1,
        .default_value  = 0,
    },
};

static int gc2035_suspend(struct soc_camera_device *icd, pm_message_t state)
{
    struct v4l2_subdev *sd;
    struct gc2035_priv *priv;

    sd = soc_camera_to_subdev(icd);
    priv = container_of(sd, struct gc2035_priv, subdev);

    return 0;
}

static int gc2035_resume(struct soc_camera_device *icd)
{
    struct v4l2_subdev *sd;
    struct gc2035_priv *priv;
    struct i2c_client *client;

    sd = soc_camera_to_subdev(icd);
    priv = to_gc2035(sd);
    client = (struct i2c_client *) v4l2_get_subdevdata(sd);

    priv->fps = -1;
    priv->res = -1;
    priv->flag_vflip = 0;
    priv->flag_hflip = 0;
    priv->brightness = 0;
    priv->contrast = 0;
    priv->effect = 0;
    priv->wh_bal = 0;
    priv->ev = 0;
    priv->sat = 0;

    priv->afc_fw_ready = 0;
    priv->f_cap = 0;
    return 0;
}

static struct soc_camera_ops gc2035_ops = {
    .set_bus_param      = gc2035_set_bus_param,
    .query_bus_param    = gc2035_query_bus_param,
    .suspend        = gc2035_suspend,
    .resume         = gc2035_resume,
    .controls       = gc2035_controls,
    .num_controls       = ARRAY_SIZE(gc2035_controls),
};

static int gc2035_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
    struct gc2035_priv *priv = to_gc2035(sd);

    switch (ctrl->id) {
    case V4L2_CID_VFLIP:
        ctrl->value = priv->flag_vflip;
        break;
    case V4L2_CID_HFLIP:
        ctrl->value = priv->flag_hflip;
        break;
    case V4L2_CID_CAMERA_FACING:
                ctrl->value = CAMERA_FACING_BACK;
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

    ret = gc2035_write(client, 0xfe, 0x00);
    if(ret < 0)
        return ret;

    ret = gc2035_read(client, 0x17, &val);
    if(ret < 0)
        return ret;
    if(is_on) {
        val |= 0x02;
    } else {
        val &= ~0x02;
    }

    ret = gc2035_write(client, 0x17, val);
    if(ret < 0)
        return ret;
    return 0;
}

static int set_mirror(struct i2c_client * client, int is_on)
{
    u8 val;
    int ret = 0;

    PDBG("is_on:%d\n", is_on);
    ret = gc2035_write(client, 0xfe, 0x00);
    if(ret < 0)
        return ret;

    ret = gc2035_read(client, 0x17, &val);
    if(ret < 0)
        return ret;
    if(is_on) {
        val |= 0x01;
    } else {
        val &= ~0x01;
    }

    ret = gc2035_write(client, 0x17, val);
    if(ret < 0)
        return ret;

    return 0;
}

static int set_brightness(struct i2c_client *client, int val)
{

    int ret = 0;


    if(val < -5 || val > 5)
        return -EINVAL;

    gc2035_write(client,0xfe,0x02);
    switch(val) {
    case -4 :
        ret = gc2035_write(client, 0xd5, 0xc0);
        break;
    case -3 :
        ret = gc2035_write(client, 0xd5, 0xd0);
        break;
    case -2 :
        ret = gc2035_write(client, 0xd5, 0xe0);
        break;
    case -1 :
        ret = gc2035_write(client, 0xd5, 0xf0);
        break;
    case 0 :
        ret = gc2035_write(client, 0xd5, 0x00);
        break;
    case 1 :
        ret = gc2035_write(client, 0xd5, 0x10);
        break;
    case 2 :
        ret = gc2035_write(client, 0xd5, 0x20);
        break;
    case 3 :
        ret = gc2035_write(client, 0xd5, 0x30);
        break;
    case 4 :
        ret = gc2035_write(client, 0xd5, 0x40);
        break;
    default:
        ret = -1;
    }

    gc2035_write(client,0xfe,0x00);

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

    gc2035_write(client,0xfe,0x00);

    switch(val) {
        case SPEC_EFF_NORMAL :
            ret = gc2035_write(client,0x83,0x00);
            break;
        case SPEC_EFF_SEPIA :
            ret = gc2035_write(client,0x83,0x82);
            break;
        case SPEC_EFF_NEG :
            ret = gc2035_write(client,0x83,0x01);
            break;
        case SPEC_EFF_GREEN :
            ret = gc2035_write(client,0x83,0x52);
            break;
        case SPEC_EFF_BLUE :
            ret = gc2035_write(client,0x83,0x62);
            break;
        case SPEC_EFF_BLWH :
            ret = gc2035_write(client,0x83,0x02);
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
    gc2035_write(client,0xfe,0x00);

    switch(val) {
        case WB_AUTO :
            ret = gc2035_write(client,0xb3, 0x61);
            ret |= gc2035_write(client,0xb4, 0x40);
            ret |= gc2035_write(client,0xb5, 0x61);
            ret |= gc2035_write(client,0x82, 0xfe);
            break;
        case WB_SUNNY :
            ret = gc2035_write(client,0x82, 0xfc);
            ret |= gc2035_write(client,0xb3, 0x58);
            ret |= gc2035_write(client,0xb4, 0x40);
            ret |= gc2035_write(client,0xb5, 0x60);
            break;
        case WB_CLOUDY :
            ret = gc2035_write(client,0x82, 0xfc);
            ret |= gc2035_write(client,0xb3, 0x58);
            ret |= gc2035_write(client,0xb4, 0x40);
            ret |= gc2035_write(client,0xb5, 0x50);
            break;
        case WB_HOME :
            ret = gc2035_write(client,0x82, 0xfc);
            ret |= gc2035_write(client,0xb3, 0x50);
            ret |= gc2035_write(client,0xb4, 0x40);
            ret |= gc2035_write(client,0xb5, 0xa8);
            break;
        case WB_OFFICE :
            ret = gc2035_write(client,0x82, 0xfc);
            ret |= gc2035_write(client,0xb3, 0xa0);
            ret |= gc2035_write(client,0xb4, 0x45);
            ret |= gc2035_write(client,0xb5, 0x40);

            break;
        default:
            ret = -1;
    }

    return ret;
}

static void pre_capture(struct i2c_client *client, struct gc2035_priv *priv)
{
    priv->f_cap = 1;
}
/* Set status of additional camera capabilities */
static int gc2035_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
    struct gc2035_priv *priv = to_gc2035(sd);
    struct i2c_client *client =
            (struct i2c_client *) v4l2_get_subdevdata(sd);

    switch (ctrl->id) {
    case V4L2_CID_VFLIP:
        priv->flag_vflip = ctrl->value;
        return set_flip(client, priv->flag_vflip);
        break;
    case V4L2_CID_HFLIP:
        priv->flag_hflip = ctrl->value;
        PDBG("set H-FLIP:%d\n", priv->flag_hflip);
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
    case V4L2_CID_PRE_CAP:
        pre_capture(client, priv);
        break;
    default:
        return -EINVAL;
    }

    return 0;
}

/* Get chip identification */
static int gc2035_g_chip_ident(struct v4l2_subdev *sd,
                   struct v4l2_dbg_chip_ident *id)
{
    struct gc2035_priv *priv = to_gc2035(sd);

    id->ident = priv->ident;
    return 0;
}

static struct v4l2_subdev_core_ops gc2035_core_ops = {
    .g_ctrl         = gc2035_g_ctrl,
    .s_ctrl         = gc2035_s_ctrl,
    .g_chip_ident       = gc2035_g_chip_ident,
};

/* Start/Stop streaming from the device */
static int gc2035_s_stream(struct v4l2_subdev *sd, int enable)
{
    struct i2c_client *client = v4l2_get_subdevdata(sd);
    struct gc2035_priv *priv = to_gc2035(sd);
    int ret = 0;

    /* Program orientation register. */
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
        ret = gc2035_write(client, 0xfe, 0x00);
        if (ret)
            goto out;
    } else {
        ret = gc2035_write(client, 0xfe, 0x00);
    }

out:
    return ret;
}

static void gc2035_res_roundup(u32 *width, u32 *height)
{
    int i;

    for (i = 0; i < ARRAY_SIZE(gc2035_resolutions); i++) {
        if ((gc2035_resolutions[i].width >= *width) &&
                (gc2035_resolutions[i].height >= *height)) {
            *width = gc2035_resolutions[i].width;
            *height = gc2035_resolutions[i].height;
            return;
        }
    }
}

static int gc2035_set_res(struct i2c_client *client, u32 width)
{

    int ret;
    struct v4l2_subdev *sd = i2c_get_clientdata(client);
    struct gc2035_priv *priv = to_gc2035(sd);

    if (width == gc2035_resolutions[GC2035_VGA].width) {
        priv->res = GC2035_VGA;
        ret |= gc2035_write_array(client, fmt_vga);
    } else if (width == gc2035_resolutions[GC2035_QCIF].width) {
        priv->res = GC2035_QCIF;
        ret = gc2035_write_array(client, fmt_qcif);
    } else if (width == gc2035_resolutions[GC2035_QVGA].width) {
        priv->res = GC2035_QVGA;
        ret = gc2035_write_array(client, fmt_qvga);
    } else if (width == gc2035_resolutions[GC2035_CIF].width) {
        priv->res = GC2035_CIF;
        ret = gc2035_write_array(client, fmt_cif);
    } else if (width == gc2035_resolutions[GC2035_720P].width) {
        priv->res = GC2035_720P;
        ret = gc2035_write_array(client, fmt_720p);
    } else if (width == gc2035_resolutions[GC2035_2M].width) {
        priv->res = GC2035_2M;
        ret = gc2035_write_array(client, fmt_uxga);
    } else {
        return -EINVAL;
    }

    priv->res = -1;

    return ret;
}

/* set the format we will capture in */
static int gc2035_s_fmt(struct v4l2_subdev *sd,
        struct v4l2_mbus_framefmt *mf)
{
    struct i2c_client *client = v4l2_get_subdevdata(sd);
    struct gc2035_priv *priv = to_gc2035(sd);
    enum v4l2_colorspace cspace;
    enum v4l2_mbus_pixelcode code = mf->code;
    int ret;
    char value;
    unsigned   int pid=0,shutter,temp_reg;

    gc2035_res_roundup(&mf->width, &mf->height);

    switch (code) {
        case V4L2_MBUS_FMT_YUYV8_2X8:
            cspace = V4L2_COLORSPACE_SRGB;
            break;
        default:
            return -EINVAL;
    }

    if(priv->afc_fw_ready == 0) {
        ret = gc2035_write_array(client, gc2035_init_regs);
        if (ret < 0) {
            return ret;
        }
        priv->afc_fw_ready = 1;
    }

    ret = gc2035_set_res(client, mf->width);
    if (ret < 0)
        return ret;

    if (priv->f_cap) {
        gc2035_write(client, 0xFE, 0x00);
        gc2035_write(client, 0xB6, 0x00);
        ret = gc2035_read(client, 0x03, &value);
        pid |= (value << 8);
        gc2035_read(client, 0x04, &value);
        pid |= (value & 0xff);
        shutter=pid;
        temp_reg= shutter /2;
        if(temp_reg < 1) temp_reg = 1;
        gc2035_write(client, 0x03, ((temp_reg>>8)&0xff));
        gc2035_write(client, 0x04, (temp_reg&0xff));
        priv->f_cap = 0;
    }

    mf->code    = code;
    mf->colorspace  = cspace;

    return ret;
}

static int gc2035_try_fmt(struct v4l2_subdev *sd,
              struct v4l2_mbus_framefmt *mf)
{
    gc2035_res_roundup(&mf->width, &mf->height);

    mf->field = V4L2_FIELD_NONE;
    mf->code = V4L2_MBUS_FMT_YUYV8_2X8;
    mf->colorspace = V4L2_COLORSPACE_SRGB;

    return 0;
}

static enum v4l2_mbus_pixelcode gc2035_codes[] = {
    V4L2_MBUS_FMT_YUYV8_2X8,
};

static int gc2035_enum_fmt(struct v4l2_subdev *sd, unsigned int index,
               enum v4l2_mbus_pixelcode *code)
{
    if (index >= ARRAY_SIZE(gc2035_codes))
        return -EINVAL;

    *code = gc2035_codes[index];

    return 0;
}

static int gc2035_cropcap(struct v4l2_subdev *sd, struct v4l2_cropcap *a)
{
    a->bounds.left      = 0;
    a->bounds.top       = 0;
    a->bounds.width     = 800;
    a->bounds.height    = 600;
    a->defrect      = a->bounds;
    a->type         = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    a->pixelaspect.numerator    = 1;
    a->pixelaspect.denominator  = 1;

    return 0;
}

static int gc2035_g_crop(struct v4l2_subdev *sd, struct v4l2_crop *a)
{
    a->c.left       = 0;
    a->c.top        = 0;
    a->c.width      = 800;
    a->c.height     = 600;
    a->type         = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    return 0;
}

static struct v4l2_subdev_video_ops gc2035_video_ops = {
    .s_stream       = gc2035_s_stream,
    .s_mbus_fmt     = gc2035_s_fmt,
    .try_mbus_fmt       = gc2035_try_fmt,
    .enum_mbus_fmt      = gc2035_enum_fmt,
    .cropcap        = gc2035_cropcap,
    .g_crop         = gc2035_g_crop,
};

static struct v4l2_subdev_ops gc2035_subdev_ops = {
    .core           = &gc2035_core_ops,
    .video          = &gc2035_video_ops,
};

static int gc2035_video_probe(struct soc_camera_device *icd,
                  struct i2c_client *client)
{
    struct v4l2_subdev *sd = i2c_get_clientdata(client);
    struct gc2035_priv *priv = to_gc2035(sd);
    char id_hi = 0, id_lo = 0;
    int chip_id = 0, ret;

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

    ret = gc2035_read(client, MODEL_ID_HI, &id_hi);
    if (ret < 0)
        goto err;
    ret = gc2035_read(client, MODEL_ID_LO, &id_lo);
    if (ret < 0)
        goto err;

    chip_id = (id_hi << 8) | id_lo;

    if (chip_id != SENSOR_ID)
    {
        dev_err(&client->dev, "got id = 0x%04x, but gc2035 product ID is 0x2035\n", chip_id);
        ret = -ENODEV;
        goto err;
    }
    priv->ident = V4L2_IDENT_GC2035;

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

    ret = gc2035_read(client, dbg_addr, &val);

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

    ret = gc2035_write(client, dbg_addr, val);

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

static int gc2035_probe(struct i2c_client *client,
            const struct i2c_device_id *did)
{
    struct gc2035_priv *priv;
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

    priv = kzalloc(sizeof(struct gc2035_priv), GFP_KERNEL);
    if (!priv) {
        dev_err(&client->dev, "Failed to allocate private data!\n");
        return -ENOMEM;
    }

    v4l2_i2c_subdev_init(&priv->subdev, client, &gc2035_subdev_ops);

    icd->ops = &gc2035_ops;

    ret = gc2035_video_probe(icd, client);
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

static int gc2035_remove(struct i2c_client *client)
{
    struct gc2035_priv *priv = i2c_get_clientdata(client);

    if(!priv) {
        return 0;
    }

#ifdef DEBUG_EN
    sysfs_remove_group(&client->dev.kobj, &dev_attr_group);
#endif

    kfree(priv);

    return 0;
}

static const struct i2c_device_id gc2035_id[] = {
    { DRIVER_NAME, 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, gc2035_id);

static struct i2c_driver gc2035_i2c_driver = {
    .driver = {
        .name = DRIVER_NAME,
    },
    .probe    = gc2035_probe,
    .remove   = gc2035_remove,
    .id_table = gc2035_id,
};

static int __init gc2035_module_init(void)
{
    return i2c_add_driver(&gc2035_i2c_driver);
}

static void __exit gc2035_module_exit(void)
{
    i2c_del_driver(&gc2035_i2c_driver);
}

module_init(gc2035_module_init);
module_exit(gc2035_module_exit);

MODULE_DESCRIPTION("SoC Camera driver for GC2035");
MODULE_AUTHOR("nufront@nufront.com");
MODULE_LICENSE("GPL v2");

