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
};

int graphics_init(void)
{	
	const struct device *display_dev;
	
	//display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	display_dev = DEVICE_DT_GET(ZEPHYR_DISPLAY);
    int ret = device_is_ready(display_dev);
    if (ret !=0 ) {
		LOG_ERR("Device not ready, aborting test");
		return ret;
	}
	
    // initialize objects array
    for(uint8_t i=0; i<MAX_GRAPHICS_OBJECTS ;i++)
    {
        graphic_objects[i] = NULL;
    }

    lv_task_handler();
	display_blanking_off(display_dev);
    return 0;
}


void graphics_get_screen_size(uint16_t *height, uint16_t*width)
{
    *height = DISPLAY_HEIGHT;
    *width = DISPLAY_WIDTH;
}

void graphics_update_screen(void)
{
    lv_task_handler();
}

void graphics_demo(void)
{
    char text[20];
    char *text_ptr= (char *) &text;

	lv_obj_t *text_label;
	lv_obj_t *image;

    // Create Label on the left side with the text
	sprintf(text_ptr, "Hello, World!");
	uint8_t text_id = graphics_draw_text(text_ptr, 10,0,12);
        
	// Create Image 
    int16_t x=0,y=32;
	uint8_t img = graphics_draw_image(&st_bitmap_gadget, x, y);
	
    
    k_sleep(K_MSEC(1000));
    int16_t y_diff=1, x_diff=1;
    uint8_t i=0;
    while (1)
	{
        graphics_move_object(img, x_diff, y_diff, true);

        graphics_get_object_position(img,&x,&y);
        sprintf(text_ptr, "x:%i y:%i", x,y);
        graphics_set_text(text_id, text_ptr);
        
        graphics_update_screen();

        k_sleep(K_MSEC(10));
        i++;
        
        if(i==128)
        {
            y_diff = -y_diff;   
        }
        if(i==0)
        {
            x_diff = -x_diff;   
        }
	}
}

uint8_t graphics_draw_image(const lv_img_dsc_t *image, int16_t x, int16_t y)
{
    bool free_index_found;
    uint8_t index = 0xFF; 
    lv_img_dsc_t *image_src = (lv_img_dsc_t *) image;

    for(uint8_t i=last_graphics_object_index; i<MAX_GRAPHICS_OBJECTS;i++)
    {
        if(graphic_objects[i] == NULL)
        {
            graphic_objects[i] = lv_img_create(lv_scr_act());
            index=i;
            break;
        }
    }
    if(0xFF == index)
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
    
    if(0xFF != index)
    {
        lv_img_set_src(graphic_objects[index], image_src);
        lv_obj_set_pos(graphic_objects[index], x, y);
        last_graphics_object_index = index;
    }

    return index;
}

uint8_t graphics_draw_text(const char *text, int16_t x, int16_t y, uint8_t font_size)
{
    bool free_index_found;
    uint8_t index = 0xFF; 
    
    if((font_size < 8) || (font_size > 22))
    {
        return 0xFF;
    }

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

    if(0xFF == index)
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


    if(0xFF != index)
    {
        font_size -=8; // fonts start at 8
        font_size >>=1;// fonts are always multiples of 2

        lv_obj_set_style_text_font(graphic_objects[index], fonts[font_size], 0); 
        lv_label_set_text(graphic_objects[index], text);
        lv_obj_set_pos(graphic_objects[index], x, y);
        last_graphics_object_index = index;
    }
   
    return index;
}

uint8_t graphics_draw_line(const lv_point_t* points, uint8_t num_points, uint8_t line_width)
{
    bool free_index_found;
    uint8_t index = 0xFF; 

    for(uint8_t i=last_graphics_object_index; i<MAX_GRAPHICS_OBJECTS;i++)
    {
        if(graphic_objects[i] == NULL)
        {
            graphic_objects[i] = lv_line_create(lv_scr_act());
            index=i;
            last_graphics_object_index = index;
            break;
        }
    }

    if(0xFF == index)
    {
        for(uint8_t i=0; i<last_graphics_object_index ;i++)
        {
            if(graphic_objects[i] == NULL)
            {
                graphic_objects[i] = lv_line_create(lv_scr_act());
                index=i;
                last_graphics_object_index = index;
                break;
            }
        }
    }


    if(0xFF != index)
    {
        lv_line_set_points(graphic_objects[index], points, num_points);
        lv_obj_set_style_line_width(graphic_objects[index], line_width,0);
        last_graphics_object_index = index;
    }
   
    return index;
}

int graphics_set_font_size(uint8_t obj_id, uint8_t size)
{
    if(graphic_objects[obj_id] != NULL)
    {
        if(((size % 2) != 0) || (size < 8) || (size > 22) || (graphic_objects[obj_id] == NULL))
        {
            return ENOENT;
        }
        else
        {            
            size -=8; // fonts start at 8
            size >>=1;// fonts are always multiples of 2

            lv_obj_set_style_text_font(graphic_objects[obj_id], fonts[size], 0); 
        
            return 0;
        }
    }
    else
    {
        return ENOENT;
    }
}

int graphics_set_text(uint8_t obj_id, char *text)
{
    if(graphic_objects[obj_id] != NULL)
    {
        lv_label_set_text(graphic_objects[obj_id], text);
    }
    else
    {
        return ENOENT;
    }

    return 0;
}

int graphics_move_object(uint8_t obj_id, int16_t offset_x, int16_t offset_y, bool wrap_around)
{
    if(graphic_objects[obj_id] != NULL)
    {
        lv_coord_t x = lv_obj_get_x(graphic_objects[obj_id]) + offset_x;
        lv_coord_t y = lv_obj_get_y(graphic_objects[obj_id]) + offset_y;

        if(wrap_around == true)
        {
            if(x>DISPLAY_WIDTH)
            {
                x=-lv_obj_get_width(graphic_objects[obj_id])+1;
            }
            else if((x+lv_obj_get_width(graphic_objects[obj_id])) < 0)
            {
                x=DISPLAY_WIDTH-1;
            }  

            if(y>DISPLAY_HEIGHT)
            {
                y=-lv_obj_get_height(graphic_objects[obj_id])+1;
            }
            else if((y+lv_obj_get_height(graphic_objects[obj_id])) < 0)
            {
                y=DISPLAY_HEIGHT-1;
            }  
        }

        lv_obj_set_pos(graphic_objects[obj_id], x, y);
    }
    else
    {
        return ENOTSUP;
    }

    return 0;
}

int graphics_set_alignment(uint8_t obj_id, lv_align_t align)
{

     if(graphic_objects[obj_id] != NULL)
    {
        lv_obj_set_align(graphic_objects[obj_id], align);
    }
    else
    {
        return ENOTSUP;
    }

    return 0;
}

int graphics_set_object_position(uint8_t obj_id, int16_t x, int16_t y)
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

int graphics_get_object_position(uint8_t obj_id, int16_t *x, int16_t *y)
{
    if(graphic_objects[obj_id] != NULL)
    {
        (*x) = lv_obj_get_x(graphic_objects[obj_id]);
        (*y) = lv_obj_get_y(graphic_objects[obj_id]);
    }
    else
    {
        return ENOTSUP;
    }

    return 0;
}


int graphics_get_object_dimensions(uint8_t obj_id, int16_t *height, int16_t *width)
{
    if(graphic_objects[obj_id] != NULL)
    {
        (*height) = lv_obj_get_height(graphic_objects[obj_id]);
        (*width)  = lv_obj_get_width(graphic_objects[obj_id]);
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

void graphics_delete_all_objects(void)
{
    for(uint8_t i=0; i<MAX_GRAPHICS_OBJECTS; i++)
    {
        if(graphic_objects[i] != NULL)
        {
            lv_obj_del(graphic_objects[i]);
            graphic_objects[i] = NULL;
        }
    }
}

