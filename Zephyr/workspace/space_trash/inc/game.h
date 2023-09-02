
#ifndef __SPACE_TRASH_GAME_H__
#define __SPACE_TRASH_GAME_H__

#include "controls.h"

#ifndef SCORE_BOARD_ENTRIES
    #define SCORE_BOARD_ENTRIES 3
#endif

#ifndef SCORE_BOARD_MAX_NAME_LENGTH
    #define SCORE_BOARD_MAX_NAME_LENGTH 10
#endif

#ifndef MISSILE_COOLDOWN
    #define MISSILE_COOLDOWN 10
#endif

#ifndef MAX_MISSILE_COUNTER
    #define MAX_MISSILE_COUNTER 10
#endif

#ifndef MAX_TRASH_COUNTER
    #define MAX_TRASH_COUNTER 20
#endif

#ifndef MAX_TRASH_COOLDOWN
    #define MAX_TRASH_COOLDOWN 75
#endif

#ifndef MIN_TRASH_COOLDOWN
    #define MIN_TRASH_COOLDOWN 5
#endif

#ifndef ITERATIONS_PER_LEVEL
    #define ITERATIONS_PER_LEVEL 250
#endif

#ifndef LEVEL_COOLDOWN_REDUCTION
    #define LEVEL_COOLDOWN_REDUCTION 5
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
    GAME_STATE_PRE_SCORE_BOARD,
    GAME_STATE_SCORE_BOARD,
} game_state_t;

void game_init(void);

void game_step(controls_btn_states_t *control_inputs);


#endif