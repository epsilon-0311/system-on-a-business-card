
#ifndef __SPACE_TRASH_GAME_H__
#define __SPACE_TRASH_GAME_H__

#include "controls.h"

#ifndef SCORE_BOARD_ENTRIES
    #define SCORE_BOARD_ENTRIES 10
#endif

#ifndef SCORE_BOARD_MAX_NAME_LENGTH
    #define SCORE_BOARD_MAX_NAME_LENGTH 5
#endif

#ifndef MISSILE_COOLDOWN
    #define MISSILE_COOLDOWN 10
#endif

#ifndef MAX_MISSILE_COUNTER
    #define MAX_MISSILE_COUNTER 10
#endif

#ifndef MAX_TRASH_COUNTER
    #define MAX_TRASH_COUNTER 10
#endif

#ifndef MAX_TRASH_COOLDOWN
    #define MAX_TRASH_COOLDOWN 100
#endif

// forward increment of pixels per iteration
#ifndef PLAYER_SPEED
    #define PLAYER_SPEED 1
#endif

// forward increment of pixels per iteration
#ifndef MISSILE_SPEED
    #define MISSILE_SPEED PLAYER_SPEED+1
#endif

// forward increment of pixels per iteration
#ifndef TRASH_SPEED
    #define TRASH_SPEED 1
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