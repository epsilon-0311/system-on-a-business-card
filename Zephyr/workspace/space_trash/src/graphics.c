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
#include "images.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(graphics, LOG_LEVEL_DBG);

/*
#define ZEPHYR_DISPLAY	DT_CHOSEN(zephry_display)
#if !DT_HAS_CHOSEN(ZEPHYR_DISPLAY)
	#error "Unsupported board: zephyr,display devicetree chosen is not defined"
#endif
*/

#define ZEPHYR_DISPLAY	DT_ALIAS(display0)
#if !DT_NODE_HAS_STATUS(ZEPHYR_DISPLAY, okay)
	#error "Unsupported board: zephyr,display devicetree chosen is not defined"
#endif

#if ! DT_NODE_HAS_PROP(ZEPHYR_DISPLAY, width) || !DT_NODE_HAS_PROP(ZEPHYR_DISPLAY, height) 
    #error "Unsupported board: the used display is not supported"
#endif

#define DISPLAY_WIDTH  DT_PROP(ZEPHYR_DISPLAY, width)
#define DISPLAY_HEIGHT DT_PROP(ZEPHYR_DISPLAY, height)

static lv_obj_t *canvas;

static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(DISPLAY_WIDTH, DISPLAY_HEIGHT)];

static lv_color_t black;
static lv_color_t white;



void graphics_init(void)
{	
	const struct device *display_dev;
	char text[20];
	lv_obj_t *text_label;
	lv_obj_t *image;
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return;
	}
	/*
	if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
		text_label = lv_label_create(hello_world_button);
	} else {
	}
	*/
	
	// Create Label on the left side with the text
	text_label = lv_label_create(lv_scr_act());
	sprintf(&text, "Data Size %u", st_bitmap_gadget.data_size);
	lv_label_set_text(text_label, &text);
	lv_obj_align(text_label, LV_ALIGN_TOP_LEFT, 10, 0);
	

	//text_area = lv_textarea_create(lv_scr_act());
	//lv_textarea_set_text(text_area, "Hello, World!");

	image = lv_img_create(lv_scr_act());
	lv_img_set_src(image, &st_bitmap_gadget);
	//lv_img_set_offset_x(image, 20);
	//lv_img_set_offset_y(image, 20);
	
	lv_task_handler();
	display_blanking_off(display_dev);

    while (1)
	{
		for(uint8_t i=0; i<128; i++)
        {
            lv_obj_align(image, LV_ALIGN_TOP_LEFT, i, 32);
            lv_task_handler();
    		k_sleep(K_MSEC(100));
        }
	}
    

	/*
	lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.radius = 10;
    rect_dsc.bg_opa = LV_OPA_COVER;
    rect_dsc.bg_grad.dir = LV_GRAD_DIR_VER;
    rect_dsc.bg_grad.stops[0].color = lv_palette_main(LV_PALETTE_RED);
    rect_dsc.bg_grad.stops[0].opa = LV_OPA_100;
    rect_dsc.bg_grad.stops[1].color = lv_palette_main(LV_PALETTE_BLUE);
    rect_dsc.bg_grad.stops[1].opa = LV_OPA_50;
    rect_dsc.border_width = 2;
    rect_dsc.border_opa = LV_OPA_90;
    rect_dsc.border_color = lv_color_white();
    rect_dsc.shadow_width = 5;
    rect_dsc.shadow_ofs_x = 5;
    rect_dsc.shadow_ofs_y = 5;
	*/

	/*
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_palette_main(LV_PALETTE_GREY);

    lv_obj_t * canvas = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(canvas, cbuf, DISPLAY_WIDTH, DISPLAY_HEIGHT, LV_IMG_CF_INDEXED_1BIT);
    lv_canvas_set_palette(canvas, 0, lv_color_black());
    lv_canvas_set_palette(canvas, 1, lv_color_white());
    //Create colors with the indices of the palette
    black.full = 0;
    white.full = 1;
    
    // fill background
    lv_obj_center(canvas);
    lv_canvas_fill_bg(canvas, black, LV_OPA_TRANSP);
	*/

	/*
    lv_area_t coords_rect = {30, 20, 100, 70};
    lv_draw_rect(&layer, &rect_dsc, &coords_rect);
	*/
	
	/*
    lv_area_t coords_text = {40, 80, 100, 120};
	lv_canvas_draw_text(canvas, 40, 20, 100, &label_dsc, "Hello, World!");
	*/

    /*Test the rotation. It requires another buffer where the original image is stored.
     *So copy the current image to buffer and rotate it to the canvas*/
    /*
	static uint8_t cbuf_tmp[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
    lv_memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));
    lv_img_dsc_t img;
    img.data = (void *)cbuf_tmp;
    img.header.cf = LV_COLOR_FORMAT_NATIVE;
    img.header.w = CANVAS_WIDTH;
    img.header.h = CANVAS_HEIGHT;

    lv_canvas_finish_layer(canvas, &layer);

    lv_canvas_fill_bg(canvas, lv_palette_lighten(LV_PALETTE_GREY, 1), LV_OPA_COVER);

    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    img_dsc.angle = 120;
    img_dsc.src = &img;
    img_dsc.pivot.x = CANVAS_WIDTH / 2;
    img_dsc.pivot.y = CANVAS_HEIGHT / 2;

    lv_area_t coords_img = {0, 0, CANVAS_WIDTH - 1, CANVAS_HEIGHT - 1};
    lv_draw_img(&layer, &img_dsc, &coords_img);

    lv_canvas_finish_layer(canvas, &layer);
	*/
}
