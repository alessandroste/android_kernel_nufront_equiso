/*
 * Basic window sizes.  These probably belong somewhere more globally
 * useful.
 */

#define gc0328_ID           0x9d

#define VGA_WIDTH           640
#define VGA_HEIGHT          480
#define QVGA_WIDTH          320
#define QVGA_HEIGHT         240
#define QCIF_WIDTH          176
#define QCIF_HEIGHT         144

#define gc0328_FRAME_RATE   7 `

#define gc0328_I2C_ADDR     0x42
#define COM7_FMT_VGA        0x00
#define REG_COM7            0xfe
#define COM7_RESET          0x80
#define REG_PID             0xf0

struct gc0328_format_struct;	/* coming later */
struct gc0328_info
{
  struct gc0328_format_struct *fmt;	/* Current format */
  unsigned char sat;		/* Saturation value */
  int hue;			/* Hue value */
};

struct regval_list
{
  unsigned char reg_num;
  unsigned char value;
};

static struct regval_list WB_Auto[] = {
  {0xfe, 0x00},
  {0x77, 0x57},
  {0x78, 0x4d},
  {0x79, 0x45},
  {0x42, 0x57},
  {0xff, 0xff}
};

static struct regval_list WB_Cldy[] = {
  {0xfe, 0x00},
  {0x42, 0x55},
  {0x77, 0x8c},
  {0x78, 0x50},
  {0x79, 0x40},
  {0xff, 0xff}
};

static struct regval_list WB_ClDa[] = {
  {0xfe, 0x00},
  {0x42, 0x55},
  {0x77, 0x74},
  {0x78, 0x52},
  {0x79, 0x40},
  {0xff, 0xff}
};

static struct regval_list WB_TungstenLamp1[] = {
  {0xfe, 0x00},
  {0x42, 0x55},
  {0x77, 0x48},
  {0x78, 0x40},
  {0x79, 0x5c},
  {0xff, 0xff}

};

static struct regval_list WB_TungstenLamp2[] = {
  {0xfe, 0x00},
  {0x42, 0x55},
  {0x77, 0x40},
  {0x78, 0x42},
  {0x79, 0x50},
  {0xff, 0xff}
};

static struct regval_list *WBSeqe[] = {
  WB_Auto,
  WB_TungstenLamp1,
  WB_TungstenLamp2,
  WB_ClDa,
  WB_Cldy,
};

static struct regval_list Bt_n4[] = {
  {0xfe, 0x00},
  {0xd5, 0xe0},
  {0xff, 0xff}
};

static struct regval_list Bt_n3[] = {
  {0x41, 0x1f},
  {0xbd, 0x30},
  {0xbe, 0x09},
  {0xff, 0xff},
};

static struct regval_list Bt_n2[] = {
  {0xfe, 0x00},
  {0xd5, 0xf0},
  {0xff, 0xff}

};

static struct regval_list Bt_n1[] = {
  {0xfe, 0x00},
  {0xd5, 0xf0},
  {0xff, 0xff}
};

static struct regval_list Bt_0[] = {
  {0xfe, 0x00},
  {0xd5, 0x00},
  {0xff, 0xff}
};

static struct regval_list Bt_p1[] = {
  {0x41, 0x1f},
  {0xbd, 0x10},
  {0xbe, 0x01},
  {0xff, 0xff},
};

static struct regval_list Bt_p2[] = {
  {0xfe, 0x00},
  {0xd5, 0x20},
  {0xff, 0xff}

};

static struct regval_list Bt_p3[] = {
  {0x41, 0x1f},
  {0xbd, 0x30},
  {0xbe, 0x01},
  {0xff, 0xff},
};

static struct regval_list Bt_p4[] = {
  {0xfe, 0x00},
  {0xd5, 0x40},
  {0xff, 0xff}
};


static struct regval_list *BtSeqe[] = {
  Bt_n4,
  Bt_n3,
  Bt_n2,
  Bt_n1,
  Bt_0,
  Bt_p1,
  Bt_p2,
  Bt_p3,
  Bt_p4,

};

static struct regval_list gc0328_init_regs[] = {
  {0xfe, 0x80},
  {0xfe, 0x80},
  {0xfc, 0x16},
  {0xfc, 0x16},
  {0xfc, 0x16},
  {0xfc, 0x16},
  {0x42, 0x02},
  {0xfe, 0x01},
  {0x4c, 0x01},
  {0xfe, 0x00},

  {0xfe, 0x00},
  {0x0d, 0x01},
  {0x0e, 0xe8},
  {0x0f, 0x02},
  {0x10, 0x88},
  {0x09, 0x00},
  {0x0a, 0x00},
  {0x0b, 0x00},
  {0x0c, 0x00},
  {0x16, 0x80},
  {0x17, 0x14},
  {0x19, 0x06},
  {0x1b, 0x48},
  {0x1f, 0xC8},
  {0x20, 0x01},
  {0x21, 0x78},			//68
  {0x22, 0xb0},
  {0x24, 0x16},
  {0x26, 0x01},
  {0x50, 0x01},
  {0x70, 0x45},

  {0x27, 0x27},
  {0x28, 0x7F},
  {0x29, 0x20},
  {0x33, 0x1A},
  {0x34, 0x1A},
  {0x35, 0x1A},
  {0x36, 0x1A},
  {0x37, 0x20},
  {0x38, 0x20},
  {0x39, 0x20},
  {0x3a, 0x20},
  {0x47, 0x00},
  {0x48, 0x75},			//00

  {0x40, 0x7f},
  {0x41, 0x22},

  {0x42, 0xff},
  {0x45, 0x00},
  {0x44, 0x02},
  {0x46, 0x02},
  {0x4b, 0x01},
  {0x50, 0x01},

  {0x7e, 0x0a},
  {0x7f, 0x03},
  {0x81, 0x15},
  {0x82, 0x85},
  {0x83, 0x02},
  {0x84, 0xe5},
  {0x90, 0xac},
  {0x92, 0x02},			//5
  {0x94, 0x05},
  {0x95, 0x63},

  {0xd1, 0x38},
  {0xd2, 0x38},
  {0xdd, 0x54},
  {0xde, 0x38},
  {0xe4, 0x88},
  {0xe5, 0x40},

  {0xd7, 0x0e},
  {0xBF, 0x0E},
  {0xc0, 0x1C},
  {0xc1, 0x34},
  {0xc2, 0x48},
  {0xc3, 0x5A},
  {0xc4, 0x6B},
  {0xc5, 0x7B},
  {0xc6, 0x95},
  {0xc7, 0xAB},
  {0xc8, 0xBF},
  {0xc9, 0xCE},
  {0xcA, 0xD9},
  {0xcB, 0xE4},
  {0xcC, 0xEC},
  {0xcD, 0xF7},
  {0xcE, 0xFD},
  {0xcF, 0xFF},

  {0xfe, 0x00},
  {0x63, 0x00},
  {0x64, 0x05},
  {0x65, 0x0b},
  {0x66, 0x19},
  {0x67, 0x2e},
  {0x68, 0x40},
  {0x69, 0x54},
  {0x6a, 0x66},
  {0x6b, 0x86},
  {0x6c, 0xa7},
  {0x6d, 0xc6},
  {0x6e, 0xe4},
  {0x6f, 0xff},

  {0xfe, 0x01},
  {0x18, 0x02},
  {0xfe, 0x00},
  {0x97, 0x30},			//Y offset TH
  {0xa4, 0x10},			//ASDE auto sa slope
  {0xa8, 0x80},			//ASDE LSC SLOPE
  {0x9c, 0x60},			//auto Y offset Slope
  {0xa2, 0x23},
  {0xad, 0x01},			//ASDE ee,ddMODE
  {0x9c, 0x01},			//ABS manual K

  {0xfe, 0x01},
  {0x9c, 0x00},			//ABS manual K
  {0x08, 0xa0},
  {0x09, 0xe8},

  {0x10, 0x08},			//[7]win_mode,[6]show_mode [3]measure_point

  {0x11, 0x21},			//[7]fix target
  {0x12, 0x10},
  {0x13, 0x45},			//AEC Y target
  {0x15, 0xfc},			//high range for count
  {0x21, 0xf0},			//c0 //post_gain limit
  {0x22, 0x60},
  {0x23, 0x30},			//20 //dggain max
  {0x25, 0x00},
  {0x24, 0x14},			//Max index

  {0xfe, 0x00},
  {0x05, 0x00},
  {0x06, 0xde},			// hb
  {0x07, 0x00},
  {0x08, 0xa7},			//vb

  {0xfe, 0x01},
  {0x29, 0x00},			//anti-flicker step [11:8]
  {0x2a, 0x83},			//anti-flicker step [7:0]

  {0x2b, 0x02},			//exp level 0  20fps
  {0x2c, 0x8f},
  {0x2d, 0x03},			//exp level 14 fps
  {0x2e, 0x95},
  {0x2f, 0x05},			//exp level 2  10.00fps
  {0x30, 0x1e},
  {0x31, 0x06},			//exp level 3  8fps
  {0x32, 0x24},
  {0xfe, 0x00},

  {0xfe, 0x01},
  {0x51, 0x80},			//20
  {0x52, 0x12},			// 16 //1f
  {0x53, 0x80},			//40
  {0x54, 0x60},			//9f
  {0x55, 0x04},			//01
  {0x56, 0x0e},			//00
  {0x5b, 0x02},
  {0x61, 0xdc},			//R2G_stand0[7:0]
  {0x62, 0xca},			//B2G_stand0[7:0]

  {0x70, 0xf5},
  {0x71, 0x0a},
  {0x72, 0x18},			//y2c
  {0x73, 0x30},			//28 //20  //AWB_C_inter
  {0x74, 0x40},			//20  // AWB_C_max
  {0x7c, 0x71},			//AWB speed,AWB margin
  {0x7d, 0x00},			//10 //AWB every N
  {0x76, 0x8f},			//move mode en,Move mode TH

  {0x4f, 0x00},
  {0x4d, 0x34},
  {0x4e, 0x04},
  {0x4e, 0x00},
  {0x4d, 0x43},
  {0x4e, 0x04},
  {0x4e, 0x04},			//D50
  {0x4e, 0x02},			// d65 add
  {0x4e, 0x02},			// d65 add
  {0x4d, 0x53},
  {0x4e, 0x08},			//08
  {0x4e, 0x04},
  {0x4e, 0x02},			//D65
  {0x4e, 0x02},			//D65
  {0x4d, 0x63},
  {0x4e, 0x10},			//10
  {0x4d, 0x82},
  {0x4e, 0x20},			//A
  {0x4d, 0x92},
  {0x4e, 0x20},			//A
  {0x4d, 0xa0},
  {0x4e, 0x40},			// H
  {0x4e, 0x40},
  {0x4e, 0x40},
  {0x4f, 0x01},
  {0x50, 0x84},			//80//
  {0xfe, 0x00},

  {0xad, 0x00},

  {0xfe, 0x01},
  {0xc0, 0x11},
  {0xc1, 0x0b},
  {0xc2, 0x09},
  {0xc6, 0x0f},
  {0xc7, 0x0a},
  {0xc8, 0x07},
  {0xba, 0x2d},
  {0xbb, 0x1e},
  {0xbc, 0x1e},
  {0xb4, 0x38},
  {0xb5, 0x26},
  {0xb6, 0x23},
  {0xc3, 0x00},
  {0xc4, 0x00},
  {0xc5, 0x00},
  {0xc9, 0x00},
  {0xca, 0x00},
  {0xcb, 0x00},
  {0xbd, 0x0a},
  {0xbe, 0x00},
  {0xbf, 0x00},
  {0xb7, 0x04},
  {0xb8, 0x00},
  {0xb9, 0x00},
  {0xa8, 0x08},
  {0xa9, 0x00},
  {0xaa, 0x00},
  {0xab, 0x07},
  {0xac, 0x00},
  {0xad, 0x07},
  {0xae, 0x0e},
  {0xaf, 0x03},
  {0xb0, 0x03},
  {0xb1, 0x0d},
  {0xb2, 0x00},
  {0xb3, 0x08},
  {0xa4, 0x00},
  {0xa5, 0x00},
  {0xa6, 0x00},
  {0xa7, 0x00},
  {0xa1, 0x3c},
  {0xa2, 0x50},
  {0xfe, 0x00},

  {0xB1, 0x08},			// 40
  {0xB2, 0x02},
  {0xB3, 0x07},
  {0xB4, 0xe0},
  {0xB5, 0x00},			//
  {0xB6, 0xf0},
  {0xf1, 0x07},
  {0xf1, 0x07},
  {0xf2, 0x01},
  {0x4f, 0x01},
  {0xff, 0xff},
};

static struct regval_list gc0328_fmt_vga[] = {
  {0xc8, 0xBF},
  {0xc9, 0xCE},
  {0xcA, 0xD9},
  {0xcB, 0xE4},
  {0xcC, 0xEC},
  {0xcD, 0xF7},
  {0xcE, 0xFD},
  {0xcF, 0xFF},
  {0xff, 0xff},
};

static struct regval_list gc0328_fmt_qvga[] = {
  {0xc1, 0x34},
  {0xc2, 0x48},
  {0xc3, 0x5A},
  {0xc4, 0x6B},
  {0xc5, 0x7B},
  {0xc6, 0x95},
  {0xc7, 0xAB},
  {0xff, 0xff},
};
