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

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(graphics, LOG_LEVEL_DBG);

void graphics_init(void)
{	
	const struct device *display_dev;
	lv_obj_t *text_label;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return;
	}

	if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
		text_label = lv_label_create(hello_world_button);
	} else {
		text_label = lv_label_create(lv_scr_act());
	}

	// Create Label on the left side with the text
	lv_label_set_text(text_label, "Hello, World!");
	lv_obj_align(text_label, LV_ALIGN_TOP_LEFT, 0, 0);

	lv_task_handler();
	display_blanking_off(display_dev);

}
