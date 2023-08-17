
#ifndef __SPACE_TRASH_GAME_H__
#define __SPACE_TRASH_GAME_H__

#include "controls.h"

#ifndef SCORE_BOARD_ENTRIES
    #define SCORE_BOARD_ENTRIES 10
#endif

typedef enum{
    GAME_STATE_INIT,
    GAME_STATE_START,
    GAME_STATE_PRE_RUN,
    GAME_STATE_RUN,
    GAME_STATE_POST_RUN,
    GAME_STATE_SCORE_BOARD,
} game_state_t;

void game_init(void);

void game_step(controls_btn_states_t *control_inputs);


#endif