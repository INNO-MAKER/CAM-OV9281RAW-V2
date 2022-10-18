/*
 * Driver for InnoMaker Co.ltd MIPI OV9281 Sensor module - CMOS Image Sensor from Omnivision
 *
 * Copyright (C) 2019, Jack Yang <jack@inno-maker.com>
 * based on 
 *
 * IMX219 driver, Guennadi Liakhovetski <g.liakhovetski@gmx.de>
 *
 * Copyright (C) 2014, Andrew Chew <achew@nvidia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_graph.h>
#include <linux/slab.h>
#include <linux/videodev2.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-fwnode.h>
#include <media/v4l2-image-sizes.h>
#include <media/v4l2-mediabus.h>

/* OV9281 supported geometry */
#define OV9281_TABLE_END		0xFFFF

/* In dB */
#define OV9281_DIGITAL_GAIN_MIN		0x00
#define OV9281_DIGITAL_GAIN_MAX		0xFE
#define OV9281_DIGITAL_GAIN_DEFAULT	0x10

/* In n*8721 nsec */
#define OV9281_DIGITAL_EXPOSURE_MIN	0x00000010
#define OV9281_DIGITAL_EXPOSURE_MAX	0x00003750
#define OV9281_DIGITAL_EXPOSURE_DEFAULT	0x00002A90

/*Sensor work  Mode - default 8-Bit Streaming */
static int sensor_mode = 1;
module_param(sensor_mode, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(sensor_mode, "Sensor Work Mode: 0=10bit_stream 1=8bit_stream 2=10bit_ext_trig 3=8bit_ext_trig 4=720_10_stream 5=720_8_stream  6=720_10_ext_trig  7=720_8_ext_trig 8=640x400_10_stream  9=640x400_8_stream 10=640x400_10_ext_trig 11=640x400_8_ext_trig 12=320x200_8_stream 13=320x200_8_ext_trig");


//#define KERNEL_5_15 1

/* Addresses to scan */
static const unsigned short normal_i2c[] = { 0x60, 0x60 , I2C_CLIENT_END };

static const s64 link_freq_menu_items[] = {
	800000000,
};

struct ov9281_reg {
	u16 addr;
	u8 val;
};

struct ov9281_mode {
	u32 sensor_mode;
	u32 sensor_depth;
	u32 sensor_ext_trig;
	u32 width;
	u32 height;
	u32 max_fps;
	u32 hts_def;
	u32 vts_def;
	const struct ov9281_reg *reg_list;
};



/*
 * Xclk 24Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 800Mbps
 */
static const struct ov9281_reg ov9281_init_tab_1280_800_120fps[] = {

	{OV9281_TABLE_END, 0x00},
};

//ext trigger mode
static const struct ov9281_reg ov9281_init_tab_1280_800_ext[] = {
	
	{OV9281_TABLE_END, 0x00},
};

//120fps raw8
static const struct ov9281_reg ov9281_init_tab_8_1280_800_120fps[] = {
	
	{OV9281_TABLE_END, 0x00},
};

//ext trigger mode raw8
static const struct ov9281_reg ov9281_init_tab_8_1280_800_ext[] = {
	
	{OV9281_TABLE_END, 0x00},
};

static const struct ov9281_reg ov9281_init_tab_1280_720_130fps[] = {
	
	{OV9281_TABLE_END, 0x00},
};

static const struct ov9281_reg ov9281_init_tab_8_1280_720_130fps[] = {
	
	{OV9281_TABLE_END, 0x00},
};

static const struct ov9281_reg ov9281_init_tab_1280_720_ext[] = {
	
	{OV9281_TABLE_END, 0x00},
};


static const struct ov9281_reg ov9281_init_tab_8_1280_720_ext[] = {
	
	{OV9281_TABLE_END, 0x00},
};

static const struct ov9281_reg ov9281_init_tab_640_400_210fps[] = {
	
	{0x4507,0x03},
	{0x4509,0x80},
	{OV9281_TABLE_END, 0x00},
};

static const struct ov9281_reg ov9281_init_tab_8_640_400_210fps[] = {
	
	{0x4507,0x03},
	{0x4509,0x80},
	{OV9281_TABLE_END, 0x00},
};

static const struct ov9281_reg ov9281_init_tab_640_400_ext[] = {
	
	{0x4507,0x03},
	{0x4509,0x80},
	{OV9281_TABLE_END, 0x00},
};


static const struct ov9281_reg ov9281_init_tab_8_640_400_ext[] = {
	
	{0x4507,0x03},
	{0x4509,0x80},
	{OV9281_TABLE_END, 0x00},
};


static const struct ov9281_reg ov9281_init_tab_8_320_200_480fps[] = {
	
	{OV9281_TABLE_END, 0x00},
};


static const struct ov9281_reg start[] = {
	{0x0100, 0x01},		/* mode select streaming on */
	{OV9281_TABLE_END, 0x00}
};

static const struct ov9281_reg stop[] = {
	{0x0100, 0x00},		/* mode select streaming off */
	{OV9281_TABLE_END, 0x00}
};


#define SIZEOF_I2C_TRANSBUF 32

struct inno_rom_table {
	char magic[12];
	char manuf[32];
	u16 manuf_id;
	char sen_manuf[8];
	char sen_type[16];
	u16 mod_id;
	u16 mod_rev;
	char regs[56];
	u16 nr_modes;
	u16 bytes_per_mode;
	char mode1[16];
	char mode2[16];
};

struct ov9281 {
	struct v4l2_subdev subdev;
	struct media_pad pad;
	struct v4l2_ctrl_handler ctrl_handler;
	struct clk *clk;
	int hflip;
	int vflip;
	u16 digital_gain;
	u32 exposure_time;
	struct v4l2_ctrl *pixel_rate;
	const struct ov9281_mode *cur_mode;
	struct i2c_client *rom;
	struct inno_rom_table rom_table;
};

static const struct ov9281_mode supported_modes[] = {
	{
		.sensor_mode = 0,
		.sensor_ext_trig = 0,
		.sensor_depth = 10,
		.width = 1280,
		.height = 800,
		.max_fps = 120,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_1280_800_120fps,
	},
	{
		.sensor_mode = 1,
		.sensor_ext_trig = 0,
		.sensor_depth = 8,
		.width = 1280,
		.height = 800,
		.max_fps = 120,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_8_1280_800_120fps,
	},
	{
		.sensor_mode = 2,
		.sensor_ext_trig = 1,
		.sensor_depth = 10,
		.width = 1280,
		.height = 800,
		.max_fps = 120,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_1280_800_ext,
	},
	{
		.sensor_mode = 3,
		.sensor_ext_trig = 1,
		.sensor_depth = 8,
		.width = 1280,
		.height = 800,
		.max_fps = 120,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_8_1280_800_ext,
	},
	{
		.sensor_mode = 4,
		.sensor_ext_trig = 0,
		.sensor_depth = 10,
		.width = 1280,
		.height = 720,
		.max_fps = 130,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_1280_720_130fps,
	},
		{
		.sensor_mode = 5,
		.sensor_ext_trig = 0,
		.sensor_depth = 8,
		.width = 1280,
		.height = 720,
		.max_fps = 130,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_8_1280_720_130fps,
	},
	   {
		.sensor_mode = 6,
		.sensor_ext_trig = 1,
		.sensor_depth = 10,
		.width = 1280,
		.height = 720,
		.max_fps = 130,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_1280_720_ext,
	},
		{
		.sensor_mode = 7,
		.sensor_ext_trig = 1,
		.sensor_depth = 8,
		.width = 1280,
		.height = 720,
		.max_fps = 130,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_8_1280_720_ext,
	},
	{
		.sensor_mode = 8,
		.sensor_ext_trig = 0,
		.sensor_depth = 10,
		.width = 640,
		.height = 400,
		.max_fps = 210,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_640_400_210fps,
	},
		{
		.sensor_mode = 9,
		.sensor_ext_trig = 0,
		.sensor_depth = 8,
		.width = 640,
		.height = 400,
		.max_fps = 210,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_8_640_400_210fps,
	},
	   {
		.sensor_mode = 10,
		.sensor_ext_trig = 1,
		.sensor_depth = 10,
		.width = 640,
		.height = 400,
		.max_fps = 210,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_640_400_ext,
	},
		{
		.sensor_mode = 11,
		.sensor_ext_trig = 1,
		.sensor_depth = 8,
		.width = 640,
		.height = 400,
		.max_fps = 210,
		.hts_def = 1355,
		.vts_def = 805,
		.reg_list = ov9281_init_tab_8_640_400_ext,
	},
		{
		.sensor_mode = 12,
		.sensor_ext_trig = 0,
		.sensor_depth = 8,
		.width = 320,
		.height = 200,
		.max_fps = 480,
		.hts_def = 728,
		.vts_def = 288,
		.reg_list = ov9281_init_tab_8_320_200_480fps,
	},

		{
		.sensor_mode = 13,
		.sensor_ext_trig = 1,
		.sensor_depth = 8,
		.width = 320,
		.height = 200,
		.max_fps = 480,
		.hts_def = 728,
		.vts_def = 288,
		.reg_list = ov9281_init_tab_8_320_200_480fps,
	},
}; 

static struct ov9281 *to_ov9281(const struct i2c_client *client)
{
	return container_of(i2c_get_clientdata(client), struct ov9281, subdev);
}

static int reg_write(struct i2c_client *client, const u16 addr, const u8 data)
{
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;
	u8 tx[3];
	int ret;

	msg.addr = client->addr;
	msg.buf = tx;
	msg.len = 3;
	msg.flags = 0;
	tx[0] = addr >> 8;
	tx[1] = addr & 0xff;
	tx[2] = data;
	ret = i2c_transfer(adap, &msg, 1);
	mdelay(2);

	return ret == 1 ? 0 : -EIO;
}

static int rom_write(struct i2c_client *client, const u8 addr, const u8 data)
{
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;
	u8 tx[2];
	int ret;

	msg.addr = client->addr;
	msg.buf = tx;
	msg.len = 2;
	msg.flags = 0;
	tx[0] = addr;	
	tx[1] = data;
	ret = i2c_transfer(adap, &msg, 1);
	mdelay(2);

	return ret == 1 ? 0 : -EIO;
}

static int reg_read(struct i2c_client *client, const u16 addr)
{
	u8 buf[2] = {addr >> 8, addr & 0xff};
	int ret;
	struct i2c_msg msgs[] = {
		{
			.addr  = client->addr,
			.flags = 0,
			.len   = 2,
			.buf   = buf,
		}, {
			.addr  = client->addr,
			.flags = I2C_M_RD,
			.len   = 1,
			.buf   = buf,
		},
	};

	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret < 0) {
		dev_warn(&client->dev, "Reading register %x from %x failed\n",
			 addr, client->addr);
		return ret;
	}

	return buf[0];
}

static int rom_read(struct i2c_client *client, const u8 addr)
{
	u8 buf[1]={ addr }; 
	int ret;
	struct i2c_msg msgs[] = {
		{
			.addr  = client->addr,
			.flags = 0,
			.len   = 1,
			.buf   = buf,
		}, {
			.addr  = client->addr,
			.flags = I2C_M_RD,
			.len   = 1,
			.buf   = buf,
		},
	};

	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret < 0) {
		dev_warn(&client->dev, "Reading register %x from %x failed\n",
			 addr, client->addr);
		return ret;
	}

	return buf[0];
}

static int reg_write_table(struct i2c_client *client,
			   const struct ov9281_reg table[])
{
	const struct ov9281_reg *reg;
	int ret;

	for (reg = table; reg->addr != OV9281_TABLE_END; reg++) {
		ret = reg_write(client, reg->addr, reg->val);
		if (ret < 0)
			return ret;
	}

	return 0;
}

/* V4L2 subdev video operations */
static int ov9281_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct ov9281 *priv = to_ov9281(client);
	int ret;
	int addr;
	int data;

	if (!enable)
		return reg_write_table(client, stop);

	ret = reg_write_table(client, priv->cur_mode->reg_list);
	if (ret)
		return ret;
	
	if(priv->cur_mode->sensor_ext_trig)
	{
		addr = 208; // ext trig enable
	        data =   1; // external trigger enable 
	        ret = rom_write(priv->rom, addr, data);
		
                mdelay(10);
		return reg_write_table(client, stop);
	}	
	else
	{
		addr = 208; // ext trig enable
	       	data =   0; // external trigger disable 
	        ret = rom_write(priv->rom, addr, data);
		
		return reg_write_table(client, start);
	}
}

/* V4L2 subdev core operations */
static int ov9281_s_power(struct v4l2_subdev *sd, int on)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct ov9281 *priv = to_ov9281(client);

	if (on)	{
		dev_dbg(&client->dev, "ov9281 power on\n");
		clk_prepare_enable(priv->clk);
	} else if (!on) {
		dev_dbg(&client->dev, "ov9281 power off\n");
		clk_disable_unprepare(priv->clk);
	}

	return 0;
}

static int ov9281_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct ov9281 *priv =
	    container_of(ctrl->handler, struct ov9281, ctrl_handler);
	struct i2c_client *client = v4l2_get_subdevdata(&priv->subdev);
	u8 reg;
	int ret;
	u16 gain = 0;
	u32 exposure = 0;

	switch (ctrl->id) {
	case V4L2_CID_HFLIP:
		priv->hflip = ctrl->val;
		reg = reg_read(client, 0x3821);
		if(ctrl->val)		 
		 ret=reg_write(client, 0x3821, reg|0x04);
	    else
		 ret=reg_write(client, 0x3821,  reg&0xFB);		
               return ret;    
	case V4L2_CID_VFLIP:
		priv->vflip = ctrl->val;
		reg = reg_read(client, 0x3820);
		if(ctrl->val)		 
		 ret=reg_write(client, 0x3820, reg|0x04);
	        else
		 ret=reg_write(client, 0x3820,  reg&0xFB);		
               return ret;
	case V4L2_CID_GAIN:

		gain = ctrl->val;

		if (gain < OV9281_DIGITAL_GAIN_MIN) 
			gain = OV9281_DIGITAL_GAIN_MIN;
		if (gain > OV9281_DIGITAL_GAIN_MAX)
			gain = OV9281_DIGITAL_GAIN_MAX;
		
		priv->digital_gain = gain;
		dev_info(&client->dev, "GAIN = %d \n",gain);

		ret  = reg_write(client, 0x3507, 0x03);
		ret |= reg_write(client, 0x3509, (priv->digital_gain) & 0xfe);

		return ret;

	case V4L2_CID_EXPOSURE:

		// 
		// 8721 ns <==> 16
		//
		exposure = (ctrl->val / 8721) * 16; // value in ns - 4 bit shift
		
		if (exposure < OV9281_DIGITAL_EXPOSURE_MIN)
			exposure = OV9281_DIGITAL_EXPOSURE_MIN;
		if (exposure > OV9281_DIGITAL_EXPOSURE_MAX)
			exposure = OV9281_DIGITAL_EXPOSURE_MAX;
		
		priv->exposure_time = (exposure / 16) * 8721;

		dev_info(&client->dev, "EXPOSURE = %d \n",exposure);

		ret  = reg_write(client, 0x3500, (exposure >> 16) & 0x0f);
		ret |= reg_write(client, 0x3501, (exposure >>  8) & 0xff);
		ret |= reg_write(client, 0x3502,  exposure        & 0xff);

		return ret;

	default:
		return -EINVAL;
	}

	/* If enabled, apply settings immediately */
	reg = reg_read(client, 0x100);
	if ((reg & 0x1f) == 0x01)
		ov9281_s_stream(&priv->subdev, 1);

	return 0;
}

static int ov9281_enum_mbus_code(struct v4l2_subdev *sd,
#ifdef KERNEL_5_15
				 struct v4l2_subdev_state *sd_state,
#else
				 struct v4l2_subdev_pad_config *cfg,
#endif			 
				 struct v4l2_subdev_mbus_code_enum *code)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct ov9281 *priv = to_ov9281(client);
	const struct ov9281_mode *mode = priv->cur_mode;

	if (code->index !=0)
		return -EINVAL;
	
	if(mode->sensor_depth==8)
		code->code = MEDIA_BUS_FMT_Y8_1X8;

	if(mode->sensor_depth==10)
		code->code = MEDIA_BUS_FMT_Y10_1X10;

	return 0;
}

#if 0
static int ov9281_get_reso_dist(const struct ov9281_mode *mode,
				struct v4l2_mbus_framefmt *framefmt)
{
	return abs(mode->width - framefmt->width) +
	       abs(mode->height - framefmt->height);
}
#endif

static const struct ov9281_mode *ov9281_find_best_fit(
					struct v4l2_subdev_format *fmt)
{
#if 0
	struct v4l2_mbus_framefmt *framefmt = &fmt->format;
	int dist;
	int cur_best_fit = 0;
	int cur_best_fit_dist = -1;
	int i;

	for (i = 0; i < ARRAY_SIZE(supported_modes); i++) {
		dist = ov9281_get_reso_dist(&supported_modes[i], framefmt);
		if (cur_best_fit_dist == -1 || dist < cur_best_fit_dist) {
			cur_best_fit_dist = dist;
			cur_best_fit = i;
		}
	}

	return &supported_modes[cur_best_fit];
#else
	return &supported_modes[sensor_mode];
#endif
}

static int ov9281_set_fmt(struct v4l2_subdev *sd,
#ifdef KERNEL_5_15
			  struct v4l2_subdev_state *sd_state,
#else
			  struct v4l2_subdev_pad_config *cfg,
#endif		  
			  struct v4l2_subdev_format *fmt)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct ov9281 *priv = to_ov9281(client);
	const struct ov9281_mode *mode;

	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY)
		return 0;

	mode = ov9281_find_best_fit(fmt);
	if(mode->sensor_depth==8)
		fmt->format.code = MEDIA_BUS_FMT_Y8_1X8;
	if(mode->sensor_depth==10)
		fmt->format.code = MEDIA_BUS_FMT_Y10_1X10;
	fmt->format.width = mode->width;
	fmt->format.height = mode->height;
	fmt->format.field = V4L2_FIELD_NONE;
	priv->cur_mode = mode;

	return 0;
}

static int ov9281_get_fmt(struct v4l2_subdev *sd,
#ifdef KERNEL_5_15
			  struct v4l2_subdev_state *sd_state,
#else
			  struct v4l2_subdev_pad_config *cfg,
#endif		  
			  struct v4l2_subdev_format *fmt)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct ov9281 *priv = to_ov9281(client);
	const struct ov9281_mode *mode = priv->cur_mode;
	s64 pixel_rate;

	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY)
		return 0;

	fmt->format.width = mode->width;
	fmt->format.height = mode->height;
	if(mode->sensor_depth==8)
		fmt->format.code = MEDIA_BUS_FMT_Y8_1X8;
	if(mode->sensor_depth==10)
		fmt->format.code = MEDIA_BUS_FMT_Y10_1X10;
	fmt->format.field = V4L2_FIELD_NONE;

	pixel_rate = mode->vts_def * mode->hts_def * mode->max_fps;
	__v4l2_ctrl_modify_range(priv->pixel_rate, pixel_rate,
					pixel_rate, 1, pixel_rate);

	return 0;
}

/* Various V4L2 operations tables */
static struct v4l2_subdev_video_ops ov9281_subdev_video_ops = {
	.s_stream = ov9281_s_stream,
};

static struct v4l2_subdev_core_ops ov9281_subdev_core_ops = {
	.s_power = ov9281_s_power,
};

static const struct v4l2_subdev_pad_ops ov9281_subdev_pad_ops = {
	.enum_mbus_code = ov9281_enum_mbus_code,
	.set_fmt = ov9281_set_fmt,
	.get_fmt = ov9281_get_fmt,
};

static struct v4l2_subdev_ops ov9281_subdev_ops = {
	.core = &ov9281_subdev_core_ops,
	.video = &ov9281_subdev_video_ops,
	.pad = &ov9281_subdev_pad_ops,
};

static const struct v4l2_ctrl_ops ov9281_ctrl_ops = {
	.s_ctrl = ov9281_s_ctrl,
};

static int ov9281_video_probe(struct i2c_client *client)
{
	struct v4l2_subdev *subdev = i2c_get_clientdata(client);
	u16 model_id;
	u32 lot_id=0;
	u16 chip_id=0;
	int ret;

	ret = ov9281_s_power(subdev, 1);
	if (ret < 0)
		return ret;

	/* Check and show model, lot, and chip ID. */
	ret = reg_read(client, 0x300A);
	if (ret < 0) {
		dev_err(&client->dev, "Failure to read Model ID (high byte)\n");
		goto done;
	}
	model_id = ret << 8; 

	ret = reg_read(client, 0x300B);
	if (ret < 0) {
		dev_err(&client->dev, "Failure to read Model ID (low byte)\n");
		goto done;
	}
	model_id |= ret;
	
	ret = reg_read(client, 0x300C);
	if (ret < 0) {
		dev_err(&client->dev, "Failure to read Lot ID (low byte)\n");
		goto done;
	}
	lot_id = ret;

	dev_info(&client->dev,
		 "Model ID 0x%04x, Lot ID 0x%06x, Chip ID 0x%04x\n",
		 model_id, lot_id, chip_id);
done:
	ov9281_s_power(subdev, 0);
	return ret;
}

static int ov9281_ctrls_init(struct v4l2_subdev *sd)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct ov9281 *priv = to_ov9281(client);
	const struct ov9281_mode *mode = priv->cur_mode;
	s64 pixel_rate;
	int ret;

	v4l2_ctrl_handler_init(&priv->ctrl_handler, 10);
	
	v4l2_ctrl_new_std(&priv->ctrl_handler, &ov9281_ctrl_ops,
			  V4L2_CID_HFLIP,0,1,1,0);
	v4l2_ctrl_new_std(&priv->ctrl_handler, &ov9281_ctrl_ops,
			  V4L2_CID_VFLIP,0,1,1,0);

	v4l2_ctrl_new_std(&priv->ctrl_handler, &ov9281_ctrl_ops,
			  V4L2_CID_GAIN,
			  OV9281_DIGITAL_GAIN_MIN,
			  OV9281_DIGITAL_GAIN_MAX, 1,
			  OV9281_DIGITAL_GAIN_DEFAULT);

	v4l2_ctrl_new_std(&priv->ctrl_handler, &ov9281_ctrl_ops,
			  V4L2_CID_EXPOSURE,
			  (OV9281_DIGITAL_EXPOSURE_MIN/16)     * 8721,
			  (OV9281_DIGITAL_EXPOSURE_MAX/16)     * 8721, 1,
			  (OV9281_DIGITAL_EXPOSURE_DEFAULT/16) * 8721);
			  

	/* freq */
	v4l2_ctrl_new_int_menu(&priv->ctrl_handler, NULL, V4L2_CID_LINK_FREQ,
			       0, 0, link_freq_menu_items);
	pixel_rate = mode->vts_def * mode->hts_def * mode->max_fps;
	priv->pixel_rate = v4l2_ctrl_new_std(&priv->ctrl_handler, NULL, V4L2_CID_PIXEL_RATE,
			  0, pixel_rate, 1, pixel_rate);


	priv->subdev.ctrl_handler = &priv->ctrl_handler;
	if (priv->ctrl_handler.error) {
		dev_err(&client->dev, "Error %d adding controls\n",
			priv->ctrl_handler.error);
		ret = priv->ctrl_handler.error;
		goto error;
	}

	ret = v4l2_ctrl_handler_setup(&priv->ctrl_handler);
	if (ret < 0) {
		dev_err(&client->dev, "Error %d setting default controls\n",
			ret);
		goto error;
	}

	return 0;
error:
	v4l2_ctrl_handler_free(&priv->ctrl_handler);
	return ret;
}

/* Return 0 if detection is successful, -ENODEV otherwise */
static int ov9281_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	//struct i2c_adapter *adapter = client->adapter;

	strlcpy(info->type, "ov9281", I2C_NAME_SIZE);

	return 0;
}

static int ov9281_probe(struct i2c_client *client,
			const struct i2c_device_id *did)
{
	struct ov9281 *priv;
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	int ret;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_warn(&adapter->dev,
			 "I2C-Adapter doesn't support I2C_FUNC_SMBUS_BYTE\n");
		return -EIO;
	}

	priv = devm_kzalloc(&client->dev, sizeof(struct ov9281), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

 	
 	priv->rom = i2c_new_dummy_device(adapter,0x10);
 	if ( priv->rom )
 	 {	
		static int i=1;
		int addr,reg,data;
		dev_info(&client->dev, "InnoMaker Camera controller found!\n");
#if 1
		for (addr=0; addr<sizeof(priv->rom_table); addr++)
		{
	          reg = rom_read(priv->rom, addr);
		  *((char *)(&(priv->rom_table))+addr)=(char)reg;
		  dev_dbg(&client->dev, "addr=0x%04x reg=0x%02x\n",addr,reg);
		}

		dev_info(&client->dev, "[ MAGIC  ] [ %s ]\n",
				priv->rom_table.magic);

		dev_info(&client->dev, "[ MANUF. ] [ %s ] [ MID=0x%04x ]\n",
				priv->rom_table.manuf,
				priv->rom_table.manuf_id);

		dev_info(&client->dev, "[ SENSOR ] [ %s %s ]\n",
				priv->rom_table.sen_manuf,
				priv->rom_table.sen_type);

		dev_info(&client->dev, "[ MODULE ] [ ID=0x%04x ] [ REV=0x%04x ]\n",
				priv->rom_table.mod_id,
				priv->rom_table.mod_rev);

		dev_info(&client->dev, "[ MODES  ] [ NR=0x%04x ] [ BPM=0x%04x ]\n",
				priv->rom_table.nr_modes,
				priv->rom_table.bytes_per_mode);
#endif
		addr = 200; // reset
	       	data =   2; // powerdown sensor 
	        reg = rom_write(priv->rom, addr, data);
		
		addr = 202; // mode
	       	data = sensor_mode; // default 8-bit streaming
	        reg = rom_write(priv->rom, addr, data);
		
		//addr = 200; // reset
	       	//data =   0; // powerup sensor
	        //reg = reg_write(priv->rom, addr, data);
		
		while(1)
		{
			mdelay(100); // wait 100ms 
			
			addr = 201; // status
			reg = rom_read(priv->rom, addr);
			
			if(reg & 0x80)
			       	break;
			
			if(reg & 0x01) 	
				dev_err(&client->dev, "!!! ERROR !!! setting  Sensor MODE=%d STATUS=0x%02x i=%d\n",sensor_mode,reg,i);
			
			if(i++ >  4)
				break;
		}

		dev_info(&client->dev, " Sensor MODE=%d PowerOn STATUS=0x%02x i=%d\n",sensor_mode,reg,i);

	}
	else
	{
		
		dev_err(&client->dev, "NOTE !!!  External Camera controller  not found !!!\n");
		dev_info(&client->dev, "Sensor MODE=%d \n",sensor_mode);
		return -EIO;
	}


	/* 1280 * 800 by default */
	priv->cur_mode = &supported_modes[sensor_mode];

	v4l2_i2c_subdev_init(&priv->subdev, client, &ov9281_subdev_ops);
	ret = ov9281_ctrls_init(&priv->subdev);
	if (ret < 0)
		return ret;
	ret = ov9281_video_probe(client);
	if (ret < 0)
		return ret;

	priv->subdev.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	priv->pad.flags = MEDIA_PAD_FL_SOURCE;

	priv->subdev.entity.function = MEDIA_ENT_F_CAM_SENSOR;
	ret = media_entity_pads_init(&priv->subdev.entity, 1, &priv->pad);
	if (ret < 0)
		return ret;

	ret = v4l2_async_register_subdev(&priv->subdev);
	if (ret < 0)
		return ret;

	return ret;
}

static int ov9281_remove(struct i2c_client *client)
{
	struct ov9281 *priv = to_ov9281(client);

	if(priv->rom)
		i2c_unregister_device(priv->rom);
	v4l2_async_unregister_subdev(&priv->subdev);
	media_entity_cleanup(&priv->subdev.entity);
	v4l2_ctrl_handler_free(&priv->ctrl_handler);

	return 0;
}

static const struct i2c_device_id ov9281_id[] = {
	{"inno_mipi_ov9281", 0},
	{}
};

static const struct of_device_id ov9281_of_match[] = {
	{ .compatible = "omnivision,inno_mipi_ov9281" },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, ov9281_of_match);

MODULE_DEVICE_TABLE(i2c, ov9281_id);
static struct i2c_driver ov9281_i2c_driver = {
	.driver = {
		.of_match_table = of_match_ptr(ov9281_of_match),
		.name = "inno_mipi_ov9281",
	},
	.probe = ov9281_probe,
	.remove = ov9281_remove,
	.id_table = ov9281_id,
	.class		= I2C_CLASS_HWMON,
	.detect		= ov9281_detect,
	.address_list	= normal_i2c,
};

module_i2c_driver(ov9281_i2c_driver);
MODULE_VERSION("0.0.3");
MODULE_DESCRIPTION("Inno-maker - MIPI OV9281 driver for Raspberry pi");
MODULE_AUTHOR("Jack Yang, Inno-maker  <support@inno-maker.com>");
MODULE_LICENSE("GPL v2");
