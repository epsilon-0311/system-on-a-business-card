/*
 * Copyright (c) 2018 Phytec Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#ifndef __SSD1312_REGS_H__
#define __SSD1312_REGS_H__

/* All following bytes will contain commands */
#define SSD1312_CONTROL_ALL_BYTES_CMD		0x00
/* All following bytes will contain data */
#define SSD1312_CONTROL_ALL_BYTES_DATA		0x40
/* The next byte will contain a command */
#define SSD1312_CONTROL_BYTE_CMD		0x80
/* The next byte will contain data */
#define SSD1312_CONTROL_BYTE_DATA		0xc0
#define SSD1312_READ_STATUS_MASK		0xc0
#define SSD1312_READ_STATUS_BUSY		0x80
#define SSD1312_READ_STATUS_ON			0x40

/*
 * Fundamental Command Table
 */
#define SSD1312_SET_CONTRAST_CTRL		0x81 /* double byte command */

#define SSD1312_SET_ENTIRE_DISPLAY_OFF		0xa4
#define SSD1312_SET_ENTIRE_DISPLAY_ON		0xa5

#define SSD1312_SET_NORMAL_DISPLAY		0xa6
#define SSD1312_SET_REVERSE_DISPLAY		0xa7

#define SSD1312_DISPLAY_OFF			0xae
#define SSD1312_DISPLAY_ON			0xaf

/*
 * Addressing Setting Command Table
 */
#define SSD1312_SET_LOWER_COL_ADDRESS		0x00
#define SSD1312_SET_LOWER_COL_ADDRESS_MASK	0x0f

#define SSD1312_SET_HIGHER_COL_ADDRESS		0x10
#define SSD1312_SET_HIGHER_COL_ADDRESS_MASK	0x0f

#define SSD1312_SET_MEM_ADDRESSING_MODE		0x20 /* double byte command */
#define SSD1312_SET_MEM_ADDRESSING_HORIZONTAL	0x00
#define SSD1312_SET_MEM_ADDRESSING_VERTICAL	0x01
#define SSD1312_SET_MEM_ADDRESSING_PAGE		0x02

#define SSD1312_SET_COLUMN_ADDRESS		0x21 /* triple byte command */

#define SSD1312_SET_PAGE_ADDRESS		0x22 /* triple byte command */

#define SSD1312_SET_PAGE_START_ADDRESS		0xb0
#define SSD1312_SET_PAGE_START_ADDRESS_MASK	0x07


/*
 * Hardware Configuration Command Table
 */
#define SSD1312_SET_START_LINE			0x40
#define SSD1312_SET_START_LINE_MASK		0x3f

#define SSD1312_SET_SEGMENT_MAP_NORMAL		0xa0
#define SSD1312_SET_SEGMENT_MAP_REMAPED		0xa1

#define SSD1312_SET_MULTIPLEX_RATIO		0xa8 /* double byte command */

#define SSD1312_SET_COM_OUTPUT_SCAN_NORMAL	0xc0
#define SSD1312_SET_COM_OUTPUT_SCAN_FLIPPED	0xc8

#define SSD1312_SET_DISPLAY_OFFSET		0xd3 /* double byte command */

#define SSD1312_SET_PADS_HW_CONFIG		0xda /* double byte command */
#define SSD1312_SET_PADS_HW_SEQUENTIAL		0x02
#define SSD1312_SET_PADS_HW_ALTERNATIVE		0x12


#define SSD1312_SET_IREF_SOURCE         0xad
#define SSD1312_IREF_SOURCE_INTERNAL    0x40
#define SSD1312_IREF_SOURCE_EXTERNAL    0x50


/*
 * Timing and Driving Scheme Setting Command Table
 */
#define SSD1312_SET_CLOCK_DIV_RATIO		0xd5 /* double byte command */

#define SSD1312_SET_CHARGE_PERIOD		0xd9 /* double byte command */

#define SSD1312_SET_VCOM_DESELECT_LEVEL		0xdb /* double byte command */

#define SSD1312_NOP				0xe3

/*
 * Charge Pump Command Table
 */
#define SSD1312_SET_CHARGE_PUMP_ON		0x8d /* double byte command */
#define SSD1312_SET_CHARGE_PUMP_ON_DISABLED	0x10
#define SSD1312_SET_CHARGE_PUMP_ON_ENABLED	0x14

#define SSD1312_SET_PUMP_VOLTAGE_64		0x30
#define SSD1312_SET_PUMP_VOLTAGE_74		0x31
#define SSD1312_SET_PUMP_VOLTAGE_80		0x32
#define SSD1312_SET_PUMP_VOLTAGE_90		0x33

/*
 * Read modify write
 */
#define SSD1312_READ_MODIFY_WRITE_START		0xe0
#define SSD1312_READ_MODIFY_WRITE_END		0xee

/* time constants in ms */
#define SSD1312_RESET_DELAY			1

#endif
