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

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(buttons, LOG_LEVEL_DBG);

#define ZEPHYR_DISPLAY	DT_CHOSEN(zephry_display)
#if !DT_HAS_CHOSEN(ZEPHYR_DISPLAY)
	#error "Unsupported board: zephyr,display devicetree chosen is not defined"
#endif

#if ! DT_NODE_HAS_PROP(ZEPHYR_DISPLAY, width) || !DT_NODE_HAS_PROP(ZEPHYR_DISPLAY, height) 
    #error "Unsupported board: the used display is not supported"
#endif

#define DISPLAY_WIDTH  DT_PROP(ZEPHYR_DISPLAY, width)
#define DISPLAY_HEIGHT DT_PROP(ZEPHYR_DISPLAY, height)

static lv_obj_t *canvas;

static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(DISPLAY_WIDTH, DISPLAY_HEIGHT)]

static lv_color_t black;
static lv_color_t white;





void init_display(void)
{	
	const struct device *display_dev;
	lv_obj_t *text_label;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return -ENODEV;
	}

    canvas = lv_canvas_create(lv_scr_act(), NULL);

    lv_canvas_set_buffer(canvas, cbuf, DISPLAY_WIDTH, DISPLAY_HEIGHT, LV_IMG_CF_INDEXED_1BIT);
    lv_canvas_set_palette(canvas, 0, LV_COLOR_BLACK);
    lv_canvas_set_palette(canvas, 1, LV_COLOR_WHITE);
    /*Create colors with the indices of the palette*/
    black.full = 0;
    white.full = 1;
    
    // fill background
    lv_canvas_fill_bg(canvas, black, LV_OPA_TRANSP);

    /*
	if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
		text_label = lv_label_create(hello_world_button);
	} else {
		text_label = lv_label_create(lv_scr_act());
	}
    */

	// Create Label on the left side with the text
	//lv_label_set_text(text_label, "Hello, World!");
	//lv_obj_align(text_label, LV_ALIGN_TOP_LEFT, 0, 0);

	lv_task_handler();
	display_blanking_off(display_dev);

}