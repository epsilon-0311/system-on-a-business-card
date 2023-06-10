/*
 * Copyright (c) 2018 PHYTEC Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT solomon_ssd1312fb

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ssd1312, CONFIG_DISPLAY_LOG_LEVEL);

#include <string.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

#include "ssd1312_regs.h"
#include <zephyr/display/cfb.h>

#if DT_INST_PROP(0, segment_remap) == 1
#define SSD1312_PANEL_SEGMENT_REMAP	true
#else
#define SSD1312_PANEL_SEGMENT_REMAP	false
#endif

#if DT_INST_PROP(0, com_invdir) == 1
#define SSD1312_PANEL_COM_INVDIR	true
#else
#define SSD1312_PANEL_COM_INVDIR	false
#endif

#if DT_INST_PROP(0, com_sequential) == 1
#define SSD1312_COM_PINS_HW_CONFIG	SSD1312_SET_PADS_HW_SEQUENTIAL
#else
#define SSD1312_COM_PINS_HW_CONFIG	SSD1312_SET_PADS_HW_ALTERNATIVE
#endif

#define SSD1312_PANEL_NUMOF_PAGES	(DT_INST_PROP(0, height) / 8)
#define SSD1312_CLOCK_DIV_RATIO		0x0
#define SSD1312_CLOCK_FREQUENCY		0x8
#define SSD1312_PANEL_VCOM_DESEL_LEVEL	0x40
#define SSD1312_PANEL_PUMP_VOLTAGE	SSD1312_SET_PUMP_VOLTAGE_90

#ifndef SSD1312_ADDRESSING_MODE
#define SSD1312_ADDRESSING_MODE		(SSD1312_SET_MEM_ADDRESSING_HORIZONTAL)
#endif

struct ssd1312_config {
#if DT_INST_ON_BUS(0, i2c)
	struct i2c_dt_spec bus;
#elif DT_INST_ON_BUS(0, spi)
	struct spi_dt_spec bus;
	struct gpio_dt_spec data_cmd;
#endif
	struct gpio_dt_spec reset;
};

struct ssd1312_data {
	uint8_t contrast;
	uint8_t scan_mode;
};

#if DT_INST_ON_BUS(0, i2c)

static inline bool ssd1312_bus_ready(const struct device *dev)
{
	const struct ssd1312_config *config = dev->config;

	return device_is_ready(config->bus.bus);
}

static inline int ssd1312_write_bus(const struct device *dev,
				    uint8_t *buf, size_t len, bool command)
{
	const struct ssd1312_config *config = dev->config;

	return i2c_burst_write_dt(&config->bus,
				  command ? SSD1312_CONTROL_ALL_BYTES_CMD :
				  SSD1312_CONTROL_ALL_BYTES_DATA,
				  buf, len);
}

#endif

static inline int ssd1312_set_panel_orientation(const struct device *dev)
{
	uint8_t cmd_buf[] = {
		(SSD1312_PANEL_SEGMENT_REMAP ?
		 SSD1312_SET_SEGMENT_MAP_REMAPED :
		 SSD1312_SET_SEGMENT_MAP_NORMAL),
		(SSD1312_PANEL_COM_INVDIR ?
		 SSD1312_SET_COM_OUTPUT_SCAN_FLIPPED :
		 SSD1312_SET_COM_OUTPUT_SCAN_NORMAL)
	};

	return ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static inline int ssd1312_set_timing_setting(const struct device *dev)
{
	uint8_t cmd_buf[] = {
		SSD1312_SET_CLOCK_DIV_RATIO,
		(SSD1312_CLOCK_FREQUENCY << 4) | SSD1312_CLOCK_DIV_RATIO,
		SSD1312_SET_CHARGE_PERIOD,
		DT_INST_PROP(0, prechargep),
		SSD1312_SET_VCOM_DESELECT_LEVEL,
		SSD1312_PANEL_VCOM_DESEL_LEVEL
	};

	return ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static inline int ssd1312_set_hardware_config(const struct device *dev)
{
	uint8_t cmd_buf[] = {
		SSD1312_SET_START_LINE,
		SSD1312_SET_DISPLAY_OFFSET,
		DT_INST_PROP(0, display_offset),
		SSD1312_SET_PADS_HW_CONFIG,
		SSD1312_COM_PINS_HW_CONFIG,
		SSD1312_SET_MULTIPLEX_RATIO,
		DT_INST_PROP(0, multiplex_ratio)
	};

	return ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static inline int ssd1312_set_charge_pump(const struct device *dev)
{
	uint8_t cmd_buf[] = {
		SSD1312_SET_CHARGE_PUMP_ON,
		SSD1312_SET_CHARGE_PUMP_ON_ENABLED,
		SSD1312_PANEL_PUMP_VOLTAGE,
	};

	return ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static int ssd1312_resume(const struct device *dev)
{
	uint8_t cmd_buf[] = {
		SSD1312_DISPLAY_ON,
	};

	return ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static int ssd1312_suspend(const struct device *dev)
{
	uint8_t cmd_buf[] = {
		SSD1312_DISPLAY_OFF,
	};

	return ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static int ssd1312_write(const struct device *dev, const uint16_t x, const uint16_t y,
			 const struct display_buffer_descriptor *desc,
			 const void *buf)
{
	size_t buf_len;

	if (desc->pitch < desc->width) {
		LOG_ERR("Pitch is smaller then width");
		return -1;
	}

	buf_len = MIN(desc->buf_size, desc->height * desc->width / 8);
	if (buf == NULL || buf_len == 0U) {
		LOG_ERR("Display buffer is not available");
		return -1;
	}

	if (desc->pitch > desc->width) {
		LOG_ERR("Unsupported mode");
		return -1;
	}

	if ((y & 0x7) != 0U) {
		LOG_ERR("Unsupported origin");
		return -1;
	}

	LOG_DBG("x %u, y %u, pitch %u, width %u, height %u, buf_len %u",
		x, y, desc->pitch, desc->width, desc->height, buf_len);

#if defined(CONFIG_SSD1312_DEFAULT)
	uint8_t cmd_buf[] = {
		SSD1312_SET_MEM_ADDRESSING_MODE,
		SSD1312_ADDRESSING_MODE,
		SSD1312_SET_COLUMN_ADDRESS,
		x,
		(x + desc->width - 1),
		SSD1312_SET_PAGE_ADDRESS,
		y/8,
		((y + desc->height)/8 - 1)
	};

	if (ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true)) {
		LOG_ERR("Failed to write command");
		return -1;
	}

	return ssd1312_write_bus(dev, (uint8_t *)buf, buf_len, false);
#endif

	return 0;
}

static int ssd1312_read(const struct device *dev, const uint16_t x,
			const uint16_t y,
			const struct display_buffer_descriptor *desc,
			void *buf)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static void *ssd1312_get_framebuffer(const struct device *dev)
{
	LOG_ERR("Unsupported");
	return NULL;
}

static int ssd1312_set_brightness(const struct device *dev,
				  const uint8_t brightness)
{
	LOG_WRN("Unsupported");
	return -ENOTSUP;
}

static int ssd1312_set_contrast(const struct device *dev, const uint8_t contrast)
{
	uint8_t cmd_buf[] = {
		SSD1312_SET_CONTRAST_CTRL,
		contrast,
	};

	return ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static void ssd1312_get_capabilities(const struct device *dev,
				     struct display_capabilities *caps)
{
	memset(caps, 0, sizeof(struct display_capabilities));
	caps->x_resolution = DT_INST_PROP(0, width);
	caps->y_resolution = DT_INST_PROP(0, height);
	caps->supported_pixel_formats = PIXEL_FORMAT_MONO10;
	caps->current_pixel_format = PIXEL_FORMAT_MONO10;
	caps->screen_info = SCREEN_INFO_MONO_VTILED;
}

static int ssd1312_set_orientation(const struct device *dev,
				   const enum display_orientation
				   orientation)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static int ssd1312_set_pixel_format(const struct device *dev,
				    const enum display_pixel_format pf)
{
	if (pf == PIXEL_FORMAT_MONO10) {
		return 0;
	}
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static int ssd1312_init_device(const struct device *dev)
{
	const struct ssd1312_config *config = dev->config;
	
	uint8_t cmd_buf[] = {
		SSD1312_SET_ENTIRE_DISPLAY_OFF,
#ifdef CONFIG_SSD1312_REVERSE_MODE
		SSD1312_SET_REVERSE_DISPLAY,
#else
		SSD1312_SET_NORMAL_DISPLAY,
#endif
	};

	// Reset if pin connected
	if (config->reset.port) {
		k_sleep(K_MSEC(SSD1312_RESET_DELAY));
		gpio_pin_set_dt(&config->reset, 1);
		k_sleep(K_MSEC(SSD1312_RESET_DELAY));
		gpio_pin_set_dt(&config->reset, 0);
	}

	// Turn display off 
	if (ssd1312_suspend(dev)) {
		return -EIO;
	}

	if (ssd1312_set_timing_setting(dev)) {
		return -EIO;
	}

	if (ssd1312_set_hardware_config(dev)) {
		return -EIO;
	}

	if (ssd1312_set_panel_orientation(dev)) {
		return -EIO;
	}

	if (ssd1312_set_charge_pump(dev)) {
		return -EIO;
	}

	if (ssd1312_set_contrast(dev, CONFIG_SSD1312_DEFAULT_CONTRAST)) {
		return -EIO;
	}

	if (ssd1312_write_bus(dev, cmd_buf, sizeof(cmd_buf), true)) {
		return -EIO;
	}
	ssd1312_resume(dev);
	
	return 0;
}

static int ssd1312_init(const struct device *dev)
{
	const struct ssd1312_config *config = dev->config;

	LOG_DBG("");

	if (!ssd1312_bus_ready(dev)) {
		LOG_ERR("Bus device %s not ready!", config->bus.bus->name);
		return -EINVAL;
	}

	if (config->reset.port) {
		int ret;

		ret = gpio_pin_configure_dt(&config->reset,
					    GPIO_OUTPUT_INACTIVE);
		if (ret < 0) {
			return ret;
		}
	}

	if (ssd1312_init_device(dev)) {
		LOG_ERR("Failed to initialize device!");
		return -EIO;
	}

	return 0;
}

static const struct ssd1312_config ssd1312_config = {
#if DT_INST_ON_BUS(0, i2c)
	.bus = I2C_DT_SPEC_INST_GET(0),
#endif
	.reset = GPIO_DT_SPEC_INST_GET_OR(0, reset_gpios, { 0 })
};

static struct ssd1312_data ssd1312_driver;

static struct display_driver_api ssd1312_driver_api = {
	.blanking_on = ssd1312_suspend,
	.blanking_off = ssd1312_resume,
	.write = ssd1312_write,
	.read = ssd1312_read,
	.get_framebuffer = ssd1312_get_framebuffer,
	.set_brightness = ssd1312_set_brightness,
	.set_contrast = ssd1312_set_contrast,
	.get_capabilities = ssd1312_get_capabilities,
	.set_pixel_format = ssd1312_set_pixel_format,
	.set_orientation = ssd1312_set_orientation,
};

DEVICE_DT_INST_DEFINE(0, ssd1312_init, NULL,
		      &ssd1312_driver, &ssd1312_config,
		      POST_KERNEL, CONFIG_DISPLAY_INIT_PRIORITY,
		      &ssd1312_driver_api);
