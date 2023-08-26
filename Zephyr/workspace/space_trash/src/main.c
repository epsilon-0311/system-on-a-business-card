/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/display.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#include <lvgl.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "graphics.h"
#include "controls.h"
#include "game.h"
#include "nvm.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#include "app_version.h"
void main(void)
{
	printk("Zephyr Example Application %s\n", APP_VERSION_STR);

	controls_init(false);
	graphics_init();
	nvm_init();
	//graphics_demo();
	game_init();
	controls_btn_states_t control_inputs;
	while (1)
	{
		controls_get_button_states(&control_inputs);
		game_step(&control_inputs);
		graphics_update_screen();

		k_sleep(K_MSEC(25));
		//printk("Hello, world!\n");
	}
}
