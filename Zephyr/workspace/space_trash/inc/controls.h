
#ifndef __SPACE_TRASH_CONTROLS_H__
#define __SPACE_TRASH_CONTROLS_H__

#include <inttypes.h>

typedef union  btn_states
{
    struct 
    {
        /* data */
        uint16_t btn_a:1;
        uint16_t btn_b:1;
        uint16_t btn_x:1;
        uint16_t btn_y:1;
        uint16_t btn_up:1;
        uint16_t btn_left:1;
        uint16_t btn_down:1;
        uint16_t btn_right:1;
        uint16_t btn_select:1;
        uint16_t btn_start:1;
    };
    uint16_t bf;
} controls_btn_states_t;

int controls_init(bool enable_interupts);

void controls_get_button_states(controls_btn_states_t* states);

#endif