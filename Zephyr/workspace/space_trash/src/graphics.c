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

static const lv_font_t *fonts[] =
{
    &lv_font_montserrat_8,
    &lv_font_montserrat_10,
    &lv_font_montserrat_12,
    &lv_font_montserrat_14,
    &lv_font_montserrat_16,
    &lv_font_montserrat_18,
    &lv_font_montserrat_20,
    &lv_font_montserrat_22,
    /*
    &lv_font_montserrat_24,
    &lv_font_montserrat_26,
    &lv_font_montserrat_28,
    &lv_font_montserrat_30,
    &lv_font_montserrat_32,
    &lv_font_montserrat_34,
    &lv_font_montserrat_36,
    &lv_font_montserrat_38,
    &lv_font_montserrat_40,
    &lv_font_montserrat_42,
    &lv_font_montserrat_44,
    &lv_font_montserrat_46,
    &lv_font_montserrat_48,
    */
};

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
	//text_label = lv_label_create(lv_scr_act());
	sprintf(&text, "Data Size %u", st_bitmap_gadget.data_size);
	//lv_label_set_text(text_label, &text);
	//lv_obj_align(text_label, LV_ALIGN_TOP_LEFT, 10, 0);

	uint8_t text_id = graphics_draw_text(&text, 10,0);
    int ret = graphics_set_font_size(text_id, 8);
    if(ret != 0)
    {
        return;
    }
	// Create Image on the left side
	//image = lv_img_create(lv_scr_act());
	//lv_img_set_src(image, &st_bitmap_gadget);
	uint8_t img = graphics_draw_image(&st_bitmap_gadget, 0, 32);
	
    lv_task_handler();
	display_blanking_off(display_dev);
    
    while (1)
	{
		for(uint8_t i=0; i<128; i++)
        {
            //lv_obj_align(image, LV_ALIGN_TOP_LEFT, i, 32);
            graphics_move_object(img, i, 32);
            lv_task_handler();
    		k_sleep(K_MSEC(10));
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
    
    if(MAX_GRAPHICS_OBJECTS != index)
    {
        lv_img_set_src(graphic_objects[index], image_src);
        lv_obj_set_pos(graphic_objects[index], x, y);
        last_graphics_object_index = index;
    }

    return index;
}

uint8_t graphics_draw_text(char *text, uint8_t x, uint8_t y)
{
    bool free_index_found;
    uint8_t index = MAX_GRAPHICS_OBJECTS; 
    index =0;

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
    
    if(MAX_GRAPHICS_OBJECTS != index)
    {
        lv_label_set_text(graphic_objects[index], text);
        lv_obj_set_pos(graphic_objects[index], x, y);
        last_graphics_object_index = index;
    }
   
    return index;
}


int graphics_set_font_size(uint8_t obj_id, uint8_t size)
{
    if(graphic_objects[obj_id] != NULL)
    {
        if(((size % 2) != 0) || (size < 8) || (size > 48) || (graphic_objects[obj_id] == NULL))
        {
            return ENOENT;
        }
        else
        {
            lv_style_t style;
            lv_style_init(&style);
            
            size -=8; // fonts start at 8
            size >>=1;// fonts are always multiples of 2

            lv_style_set_text_font(&style, &lv_font_montserrat_22); //fonts[size]);
            lv_obj_add_style(graphic_objects[obj_id], &style, 0); 
            
            return 0;
        }
    }
    else
    {
        return ENOENT;
    }
    
}

int graphics_move_object(uint8_t obj_id, uint8_t x, uint8_t y)
{
    if(graphic_objects[obj_id] != NULL)
    {
        lv_obj_set_pos(graphic_objects[obj_id], x, y);
    }
    else
    {
        return ENOTSUP;
    }

    return 0;
}

int graphics_delete_object(uint8_t obj_id)
{
    if(graphic_objects[obj_id] != NULL)
    {
        lv_obj_del(graphic_objects[obj_id]);
        graphic_objects[obj_id] = NULL;
    }
    else
    {
        return ENOTSUP;
    }

    return 0;
}