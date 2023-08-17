#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/display.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>


#include <stdio.h>
#include <string.h>
#include <math.h>

#include <lvgl.h>

#include "graphics.h"
#include "images.h"
#include "game.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(game, LOG_LEVEL_DBG);


/*****************************************
 * Function definitions
 *****************************************/
static void show_start_screen(void);
static void show_game_screen(void);
static int handle_game_run(controls_btn_states_t *control_inputs);
static void input_name(controls_btn_states_t *control_inputs);
static void show_score_board(controls_btn_states_t *control_inputs);

/*****************************************
 * Variable definitions
 *****************************************/
static game_state_t state;
static uint16_t game_area_x_left;
static uint16_t game_area_x_right;
static uint16_t game_area_y_top;
static uint16_t game_area_y_bottom;

static uint16_t current_player_postion_x;
static uint16_t current_player_postion_y;
static uint16_t player_postion_limit_x_left;
static uint16_t player_postion_limit_x_right;
static uint16_t player_postion_limit_y_top;
static uint16_t player_postion_limit_y_bottom;

static uint32_t current_score;
static uint32_t high_score;
static uint32_t score_board[SCORE_BOARD_ENTRIES];

char score_text[30];
char *score_text_ptr = (char *)&score_text[0];
uint8_t score_label_id;
uint8_t player_id;

static lv_point_t top_line_points[]=
{
    {
        .x=0,
        .y=11,
    },
    {
        .x=127,
        .y=11,
    }
};

void game_init(void)
{
    state = GAME_STATE_INIT;
    current_score=0;

    // reserve top pixels for displaying text 
    game_area_y_top=12;
    game_area_x_left =0;
    graphics_get_screen_size(&game_area_y_bottom, &game_area_x_right);
}


void game_step(controls_btn_states_t *control_inputs)
{

    switch (state)
    {
        case GAME_STATE_INIT:
            state = GAME_STATE_START;
            break;
        case GAME_STATE_START:
            show_start_screen();
            state = GAME_STATE_PRE_RUN;
            break;
        case GAME_STATE_PRE_RUN:
            // show start screen until start is pressed 
            printk("control_inputs->btn_start: %u\n", control_inputs->btn_start);
            if(control_inputs->btn_start == 1)
            {
                graphics_delete_all_objects();
                show_game_screen();
                current_score=0;
                state = GAME_STATE_RUN;
            }
            break;
        case GAME_STATE_RUN:
            // return value is negative if player died
            if(handle_game_run(control_inputs) != 0)
            {
                if(current_score > score_board[SCORE_BOARD_ENTRIES-1])
                {
                    state = GAME_STATE_POST_RUN;
                }
                else
                {
                    state = GAME_STATE_SCORE_BOARD;
                }
            }
            break;
        case GAME_STATE_POST_RUN:
            // enter name for new score entry
            input_name(control_inputs);
            if(control_inputs->btn_start == 1)
            {
                state = GAME_STATE_START;
            }
            break;
        case GAME_STATE_SCORE_BOARD:
            // show scoreboard until start is pressed
            show_score_board(control_inputs);
            if(control_inputs->btn_start == 1)
            {
                state = GAME_STATE_START;
            }
            break;
        default:
            break;
    }
}

static void show_start_screen(void)
{
    char high_score_text[30];
    char *high_score_text_ptr = (char *)&high_score_text[0];

    // set title
    uint8_t title=graphics_draw_text("Space Trash", 0, 1, 20);
    // display high score
    sprintf( high_score_text_ptr,"High Score: %u", high_score);
    uint8_t high_score_label =graphics_draw_text(high_score_text_ptr, 10, 25, 10);  
    // set title
    uint8_t start=graphics_draw_text("Press Start!", 20, 40, 16);
    
    return;
}


static void show_game_screen(void)
{
    sprintf( score_text_ptr,"Score: %5u", current_score);
    score_label_id =graphics_draw_text(score_text_ptr, 0, 0, 10);
    // the reference to line object is not required by the game
    (void)graphics_draw_line(&top_line_points, 2, 2);

    current_player_postion_x=0;
    current_player_postion_y = game_area_y_top+((game_area_y_bottom-game_area_y_top)>>1);

    player_postion_limit_x_left=game_area_x_left;
    player_postion_limit_x_right=game_area_x_right-7; // initial ship is 7 pixel wide
    player_postion_limit_y_top=game_area_y_top;
    player_postion_limit_y_bottom=game_area_y_bottom-8; // initil ship is 8 pixel high

    player_id = graphics_draw_image(&st_bitmap_player1, current_player_postion_x, current_player_postion_y);
}

static int handle_game_run(controls_btn_states_t *control_inputs)
{
    int16_t move_x=0, move_y=0;
    
    if(control_inputs->btn_up==1)
    {
        if(current_player_postion_y > player_postion_limit_y_top)
        {
            move_y=-1;
            current_player_postion_y--;
        }
    }
    else if(control_inputs->btn_down==1)
    {
        if(current_player_postion_y < player_postion_limit_y_bottom)
        {
            move_y=1;
            current_player_postion_y++;
        }
    }

    if(control_inputs->btn_left==1)
    {
        if(current_player_postion_x > player_postion_limit_x_left)
        {
            move_x=-1;
            current_player_postion_x--;
        }
    }
    else if(control_inputs->btn_right==1)
    {
        if(current_player_postion_x < player_postion_limit_x_right)
        {
            move_x=1;
            current_player_postion_x++;
        }
    }

    graphics_move_object(player_id, move_x, move_y, false);

    return 0;
}

static void input_name(controls_btn_states_t *control_inputs)
{
    return;
}

static void show_score_board(controls_btn_states_t *control_inputs)
{
    return;
}
