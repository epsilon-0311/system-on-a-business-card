
#ifndef __SPACE_TRASH_GRAPHICS_H__
#define __SPACE_TRASH_GRAPHICS_H__

#include <stdio.h>
#include <inttypes.h>
#include <lvgl.h>

#ifndef MAX_GRAPHICS_OBJECTS
    #define MAX_GRAPHICS_OBJECTS 50
#endif

void graphics_init(void);

uint8_t graphics_draw_image(lv_img_dsc_t *image_src, uint8_t x, uint8_t y);

uint8_t graphics_draw_text(char *text, uint8_t x, uint8_t y);

int graphics_set_font_size(uint8_t obj_id, uint8_t size);

int graphics_move_object(uint8_t obj_id, uint8_t x, uint8_t y);

int graphics_delete_object(uint8_t obj_id);

#endif