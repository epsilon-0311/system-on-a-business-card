
#ifndef __SPACE_TRASH_GRAPHICS_H__
#define __SPACE_TRASH_GRAPHICS_H__

#include <stdio.h>
#include <inttypes.h>
#include <lvgl.h>

#ifndef MAX_GRAPHICS_OBJECTS
    #define MAX_GRAPHICS_OBJECTS 50
#endif

int graphics_init(void);

void graphics_get_screen_size(uint16_t *height, uint16_t*width);

void graphics_update_screen(void);

void graphics_demo(void);

uint8_t graphics_draw_image(const lv_img_dsc_t *image, int16_t x, int16_t y);

uint8_t graphics_draw_text(const char *text, int16_t x, int16_t y, uint8_t font_size);

uint8_t graphics_draw_line(const lv_point_t* points, uint8_t num_points, uint8_t line_width);

uint8_t graphics_create_text_area( int16_t x, int16_t y, uint16_t height, uint8_t max_length, uint8_t font_size);

uint8_t graphics_text_input_screen( int16_t x, int16_t y, uint16_t height, uint8_t ta_obj_id, void (*cb_func)(const char *));

int graphics_set_font_size(uint8_t obj_id, uint8_t size);

int graphics_set_text(uint8_t obj_id, char *text);

int graphics_move_object(uint8_t obj_id, int16_t offset_x, int16_t offset_y, bool wrap_around);

int graphics_set_alignment(uint8_t obj_id, lv_align_t align);

int graphics_set_object_position(uint8_t obj_id, int16_t x, int16_t y);

int graphics_get_object_position(uint8_t obj_id, int16_t *x, int16_t *y);

int graphics_get_object_dimensions(uint8_t obj_id, int16_t *height, int16_t *width);

int graphics_delete_object(uint8_t obj_id);

void graphics_delete_all_objects(void);

#endif