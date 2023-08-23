#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/display.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#include <time.h>
#include <stdlib.h>
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
static void handle_trash(void);
static void handle_missiles(uint8_t fire_shot);
static int handle_colisions(void);

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

static uint8_t missiles[MAX_MISSILE_COUNTER];
static uint8_t trash[MAX_TRASH_COUNTER];

static uint8_t current_level;
static uint32_t current_score;
static uint32_t high_score;
static uint32_t score_board_scores[SCORE_BOARD_ENTRIES];
static char score_board_names[SCORE_BOARD_ENTRIES][SCORE_BOARD_MAX_NAME_LENGTH];

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

    // initialize missile array
    for(uint8_t i=0; i<MAX_MISSILE_COUNTER; i++)
    {
        missiles[i]=0xFF;
    }
    
    // initialize trash array
    for(uint8_t i=0; i<MAX_TRASH_COUNTER; i++)
    {
        trash[i]=0xFF;
    }

    // initialize scoreboard array
    for(uint8_t i=0; i<SCORE_BOARD_ENTRIES; i++)
    {
        score_board_scores[i]=0x0;
        score_board_names[i][0]='\0';
    }
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
            if(control_inputs->btn_start == 1)
            {
                graphics_delete_all_objects();
                show_game_screen();

                // initialize missile array
                for(uint8_t i=0; i<MAX_MISSILE_COUNTER; i++)
                {
                    missiles[i]=0xFF;
                }

                // initialize trash array
                for(uint8_t i=0; i<MAX_TRASH_COUNTER; i++)
                {
                    trash[i]=0xFF;
                }
                
                current_score=0;
                current_level=0;
                state = GAME_STATE_RUN;
            }
            break;
        case GAME_STATE_RUN:
            // return value is negative if player died
            if(handle_game_run(control_inputs) != 0)
            {
                graphics_delete_all_objects();
                if(current_score > score_board_scores[SCORE_BOARD_ENTRIES-1])
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
                state = GAME_STATE_SCORE_BOARD;
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
            move_y=-PLAYER_SPEED;
            current_player_postion_y-=PLAYER_SPEED;
        }
    }
    else if(control_inputs->btn_down==1)
    {
        if(current_player_postion_y < player_postion_limit_y_bottom)
        {
            move_y=PLAYER_SPEED;
            current_player_postion_y+=PLAYER_SPEED;
        }
    }

    if(control_inputs->btn_left==1)
    {
        if(current_player_postion_x > player_postion_limit_x_left)
        {
            move_x=-PLAYER_SPEED;
            current_player_postion_x-=PLAYER_SPEED;
        }
    }
    else if(control_inputs->btn_right==1)
    {
        if(current_player_postion_x < player_postion_limit_x_right)
        {
            move_x=PLAYER_SPEED;
            current_player_postion_x+=PLAYER_SPEED;
        }
    }

    handle_missiles(control_inputs->btn_a );
    handle_trash();

    graphics_move_object(player_id, move_x, move_y, false);

    int ret = handle_colisions();
    sprintf( score_text_ptr,"Score: %5u", current_score);
    graphics_set_text(score_label_id, score_text_ptr);
    return ret;
}

static void handle_missiles(uint8_t fire_shot)
{
    static uint8_t fire_counter=0;
    
    uint8_t fresh_missile=0xff;

    if(fire_shot != 0 && fire_counter ==0)
    {
        // find free slot for missile 
        for(uint8_t i=0; i<MAX_MISSILE_COUNTER; i++)
        {
            if(missiles[i]==0xFF)
            {
                missiles[i] = graphics_draw_image(&st_bitmap_missile, current_player_postion_x+7, current_player_postion_y);
                // check if missile was created
                if(missiles[i] != 0xFF)
                {
                    fresh_missile=i;
                    // reset fire counter
                    fire_counter = MISSILE_COOLDOWN;
                }
                break;
            }
        }
    }
    else if(fire_counter > 0)
    {
        fire_counter--;
    }

    // iterate over all missiles and move 
    for(uint8_t i=0; i<MAX_MISSILE_COUNTER; i++)
    {
        if(missiles[i]!=0xFF && fresh_missile!=i)
        {
            int16_t x,y;
            int ret=0;
            
            ret = graphics_move_object(missiles[i], MISSILE_SPEED, 0, false);
            if(ret == 0)
            {
                // the existance of the graphics object was already estiblished, hence the return values can be ignored
                (void)graphics_get_object_position(missiles[i],&x, &y);
                // missile went out of screen
                if(x >=game_area_x_right)
                {
                    (void)graphics_delete_object(missiles[i]);
                    missiles[i] = 0xFF;
                }
            }
            // graphics object doesnt seem to exist, clear it from missile array
            else
            {
                missiles[i] = 0xFF;
            }
        }
    }
}

static void handle_trash(void)
{
    static uint8_t trash_cooldown=0;
    uint8_t fresh_trash = 0xff;

    if(trash_cooldown ==0)
    {
        // find free slot for trash 
        for(uint8_t i=0; i<MAX_TRASH_COUNTER; i++)
        {
            if(trash[i]==0xFF)
            {
                uint8_t trash_type = rand() % 3;

                if(trash_type ==0)
                {
                    int16_t y_pos = game_area_y_top + (rand() % (game_area_y_bottom-game_area_y_top-5));
                    trash[i] = graphics_draw_image(&st_bitmap_trash_5x5_1, game_area_x_right, y_pos);
                }
                else if(trash_type==1)
                {
                    int16_t y_pos = game_area_y_top + (rand() % (game_area_y_bottom-game_area_y_top-5));
                    trash[i] = graphics_draw_image(&st_bitmap_trash_5x5_2, game_area_x_right, y_pos);    
                }
                else
                {
                    int16_t y_pos = game_area_y_top + (rand() % (game_area_y_bottom-game_area_y_top-7));
                    trash[i] = graphics_draw_image(&st_bitmap_trash_7x7, game_area_x_right, y_pos);
                }

                // check if trash was created
                if(trash[i] != 0xFF)
                {
                    fresh_trash=i;
                    // reset trash cooldown, reduced by current level
                    trash_cooldown = MAX_TRASH_COOLDOWN-current_level;
                }
                break;
            }
        }
    } 
    else
    {
        trash_cooldown--;
    }
    
    // iterate over all missiles and move 
    for(uint8_t i=0; i<MAX_TRASH_COUNTER; i++)
    {
        if(trash[i]!=0xFF && fresh_trash!=i)
        {
            int16_t x,y;
            int ret=0;
            
            ret = graphics_move_object(trash[i], -TRASH_SPEED, 0, false);
            if(ret == 0)
            {
                // the existance of the graphics object was already estiblished, hence the return values can be ignored
                (void)graphics_get_object_position(trash[i],&x, &y);
                // trash went out of screen
                if(x <=game_area_x_left)
                {
                    (void)graphics_delete_object(trash[i]);
                    trash[i] = 0xFF;
                }
            }
            // graphics object doesnt seem to exist, clear it from trash array
            else
            {
                trash[i] = 0xFF;
            }
        }
    }
}

static int handle_colisions(void)
{
    int16_t current_player_postion_x_right,current_player_postion_y_bottom;
    int16_t player_height, player_width;

    (void)graphics_get_object_dimensions(player_id,&player_height, &player_width);
    
    current_player_postion_x_right = current_player_postion_x+player_width;
    current_player_postion_y_bottom = current_player_postion_y+player_height;

    for(uint8_t trash_id=0; trash_id<MAX_TRASH_COUNTER; trash_id++)
    {
        if(trash[trash_id] != 0xFF)
        {
            int16_t x_trash_left,y_trash_top, x_trash_right,y_trash_bottom;
            int16_t trash_height, trash_width;
            bool colision_detected = false;

            (void)graphics_get_object_position(trash[trash_id],&x_trash_left, &y_trash_top);
            (void)graphics_get_object_dimensions(trash[trash_id],&trash_height, &trash_width);

            x_trash_right = x_trash_left + trash_width;
            y_trash_bottom = y_trash_top + trash_height;
            
            // check if overarching rectangels of the trash and player overlay 
            if( x_trash_left < current_player_postion_x_right && 
                x_trash_right > current_player_postion_x &&
                y_trash_top < current_player_postion_y_bottom &&
                y_trash_bottom > current_player_postion_y
                )
            {
                // colision with player detected, end game
                return -1;
            }

            for(uint8_t missile_id=0; missile_id < MAX_MISSILE_COUNTER; missile_id++)
            {
                if(missiles[missile_id] != 0xFF)
                {
                    int16_t x_missile_left,y_missile_top, x_missile_right,y_missile_bottom;
                    int16_t missile_height, missile_width;
                    (void)graphics_get_object_position(missiles[missile_id],&x_missile_left, &y_missile_top);
                    (void)graphics_get_object_dimensions(missiles[missile_id],&missile_height, &missile_width);

                    x_missile_right = x_missile_left + missile_width;
                    y_missile_bottom = y_missile_top + missile_height;

                    // check if overarching rectangels of the objects overlay 
                    if( x_trash_left < x_missile_right && 
                        x_trash_right > x_missile_left &&
                        y_trash_top < y_missile_bottom &&
                        y_trash_bottom > y_missile_top
                      )
                    {
                        // colision detected
                        (void)graphics_delete_object(trash[trash_id]);
                        (void)graphics_delete_object(missiles[missile_id]);
                        trash[trash_id] = 0xFF;
                        missiles[missile_id] = 0xFF;
                        colision_detected = true;
                        
                        // size of trash == score to eliminate trash
                        current_score+=trash_height;

                        break;
                    }
                }
            }

            if(colision_detected == true)
            {
                break;
            }
        }
    }

    return 0;
}

static void input_name(controls_btn_states_t *control_inputs)
{
    uint8_t start=graphics_draw_text("Enter Name:", 0, 0, 16);
    
    return;
}

static void show_score_board(controls_btn_states_t *control_inputs)
{
    return;
}
