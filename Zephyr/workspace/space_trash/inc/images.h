
#ifndef __SPACE_TRASH_IMAGES_H__
#define __SPACE_TRASH_IMAGES_H__


#include <lvgl.h>

const uint8_t bitmap_player1[] = 
{ 
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0: */
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 1: */
  /* 01100000 */ 0x060,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 01100000 */ 0x060
};

const lv_img_dsc_t st_bitmap_player1 = {
  .header.reserved = 0,
  .header.always_zero = 0,
  .header.w = 8,
  .header.h = 5,
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .data_size = 13,
  .data = bitmap_player1,
};

const uint8_t bitmap_player2[] = 
{ 
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0: */
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 1: */
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060,
  /* 11100000 */ 0x0e0,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 01100000 */ 0x060
};

const lv_img_dsc_t st_bitmap_player2 = {
  .header.reserved = 0,
  .header.always_zero = 0,
  .header.w = 8,
  .header.h = 8,
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .data_size = 16,
  .data = bitmap_player2,
};

const uint8_t bitmap_player3[] = 
{ 
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0: */
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 1: */
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060,
  /* 11100000 */ 0x0e0,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 11100000 */ 0x0e0,
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060
};

const lv_img_dsc_t st_bitmap_player3 = {
  .header.reserved = 0,
  .header.always_zero = 0,
  .header.w = 8,
  .header.h = 11,
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .data_size = 17,
  .data = bitmap_player3,
};

const uint8_t bitmap_trash_5x5_1[] = 
{ 
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0: */
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 1: */
  /* 01110000 */ 0x070,
  /* 11110000 */ 0x0f0,
  /* 11111000 */ 0x0f8,
  /* 01111000 */ 0x078,
  /* 00110000 */ 0x030,
};

const lv_img_dsc_t st_bitmap_trash_5x5_1 = {
  .header.reserved = 0,
  .header.always_zero = 0,
  .header.w = 5,
  .header.h = 5,
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .data_size = 13,
  .data = bitmap_trash_5x5_1,
};

const uint8_t bitmap_trash_5x5_2[] = 
{ 
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0: */
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 1: */
  /* 00110000 */ 0x030,
  /* 11111000 */ 0x0f8,
  /* 11111000 */ 0x0f8,
  /* 11110000 */ 0x0f0,
  /* 01110000 */ 0x070,
};

const lv_img_dsc_t st_bitmap_trash_5x5_2 = {
  .header.reserved = 0,
  .header.always_zero = 0,
  .header.w = 5,
  .header.h = 5,
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .data_size = 13,
  .data = bitmap_trash_5x5_2,
};

const uint8_t bitmap_trash_7x7[] = 
{ 
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0: */
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 1: */
  /* 00111000 */ 0x038,
  /* 01111100 */ 0x07c,
  /* 11111100 */ 0x0fc,
  /* 11111110 */ 0x0fe,
  /* 11111110 */ 0x0fe,
  /* 01111110 */ 0x07e,
  /* 01111000 */ 0x078,
};

const lv_img_dsc_t st_bitmap_trash_7x7 = {
  .header.reserved = 0,
  .header.always_zero = 0,
  .header.w = 7,
  .header.h = 7,
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .data_size = 15,
  .data = bitmap_trash_7x7,
};

const uint8_t bitmap_gadget[] = 
{ 
  0x00, 0x00, 0x00, 0x00, 	/*Color of index 0: */
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 1: */
  /* 01110000 */ 0x070,
  /* 11011000 */ 0x0d8,
  /* 10001000 */ 0x088,
  /* 11011000 */ 0x0d8,
  /* 01110000 */ 0x070,
};

const lv_img_dsc_t st_bitmap_gadget = {
  .header.reserved = 0,
  .header.always_zero = 0,
  .header.w = 5,
  .header.h = 5,
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .data_size = 13,
  .data = bitmap_gadget,
};

#endif