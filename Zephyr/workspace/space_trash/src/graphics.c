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

static lv_obj_t *graphic_objects[MAX_GRAPHICS_OBJECTS];
static uint8_t last_graphics_object_index=0;

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
	
	// Create Label on the left side with the text
	text_label = lv_label_create(lv_scr_act());
	sprintf(&text, "Data Size %u", st_bitmap_gadget.data_size);
	lv_label_set_text(text_label, &text);
	lv_obj_align(text_label, LV_ALIGN_TOP_LEFT, 10, 0);
	

	// Create Image on the left side
	image = lv_img_create(lv_scr_act());
	lv_img_set_src(image, &st_bitmap_gadget);
	
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
}

uint8_t graphics_draw_image(lv_img_dsc_t *image_src, uint8_t x, uint8_t y)
{
    bool free_index_found;
    uint8_t index = MAX_GRAPHICS_OBJECTS; 

    for(uint8_t i=last_graphics_object_index; i<MAX_GRAPHICS_OBJECTS;i++)
    {
        if(graphic_objects[i] == NULL)
        {
            graphic_objects[i] = lv_img_create(lv_scr_act());
            index=i;
            break;
        }
    }
    if(MAX_GRAPHICS_OBJECTS == index)
    {
        for(uint8_t i=0; i<last_graphics_object_index ;i++)
        {
            if(graphic_objects[i] == NULL)
            {
                graphic_objects[i] = lv_img_create(lv_scr_act());
                index=i;
                break;
            }
        }
    }
    last_graphics_object_index = index;
    
	lv_img_set_src(graphic_objects[index], image_src);
    lv_obj_set_pos(graphic_objects[index], x, y);
    return index;
}

uint8_t graphics_draw_text(char *text, uint8_t x, uint8_t y)
{
    bool free_index_found;
    uint8_t index = MAX_GRAPHICS_OBJECTS; 

    for(uint8_t i=last_graphics_object_index; i<MAX_GRAPHICS_OBJECTS;i++)
    {
        if(graphic_objects[i] == NULL)
        {
            graphic_objects[i] = lv_label_create(lv_scr_act());
            index=i;
            last_graphics_object_index = index;
            break;
        }
    }
    if(MAX_GRAPHICS_OBJECTS == index)
    {
        for(uint8_t i=0; i<last_graphics_object_index ;i++)
        {
            if(graphic_objects[i] == NULL)
            {
                graphic_objects[i] = lv_label_create(lv_scr_act());
                index=i;
                last_graphics_object_index = index;
                break;
            }
        }
    }
    
	lv_label_set_text(graphic_objects[index], &text);
    lv_obj_set_pos(graphic_objects[index], x, y);
    return index;
}

void graphics_move_object(uint8_t obj_id, uint8_t x, uint8_t y)
{
    if(graphic_objects[obj_id] != NULL)
    {
        lv_obj_set_pos(graphic_objects[obj_id], x, y);
    }
}

void graphics_delete_object(uint8_t obj_id)
{
    if(graphic_objects[obj_id] != NULL)
    {
        lv_obj_del(graphic_objects[obj_id]);
        graphic_objects[obj_id] = NULL;
    }
}