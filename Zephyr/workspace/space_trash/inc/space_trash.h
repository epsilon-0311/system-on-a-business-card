#ifndef __SPACE_TRASH_H__
#define __SPACE_TRASH_H__

#define ST_FP 4

#define ST_DRAW_NONE 0
#define ST_DRAW_BBOX 1
#define ST_DRAW_TRASH1 2
#define ST_DRAW_PLAYER1 3
#define ST_DRAW_TRASH2 4
#define ST_DRAW_PLAYER2 5
#define ST_DRAW_PLAYER3 6
#define ST_DRAW_GADGET 7
#define ST_DRAW_BACKSLASH 8
#define ST_DRAW_SLASH 9
#define ST_DRAW_BIG_TRASH 10

#define ST_MOVE_NONE 0
#define ST_MOVE_X_SLOW 1
#define ST_MOVE_PX_NORMAL 2
#define ST_MOVE_PX_FAST 3
#define ST_MOVE_PLAYER 4
#define ST_MOVE_PY 5
#define ST_MOVE_NY 6
#define ST_MOVE_IMPLODE 7
#define ST_MOVE_X_FAST 8
#define ST_MOVE_WALL 9
#define ST_MOVE_NXPY 10
#define ST_MOVE_NXNY 11

#define ST_IS_HIT_NONE 0
#define ST_IS_HIT_BBOX 1
#define ST_IS_HIT_WALL 2

#define ST_DESTROY_NONE 0
#define ST_DESTROY_DISAPPEAR 1
#define ST_DESTROY_TO_DUST 2
#define ST_DESTROY_GADGET 3
#define ST_DESTROY_PLAYER 4
#define ST_DESTROY_PLAYER_GADGETS 5
#define ST_DESTROY_BIG_TRASH 6

#define ST_FIRE_NONE 0
#define ST_FIRE_PLAYER1 1
#define ST_FIRE_PLAYER2 2
#define ST_FIRE_PLAYER3 3

#define ST_OT_WALL_SOLID 1
#define ST_OT_BIG_TRASH 2
#define ST_OT_MISSLE 3
#define ST_OT_TRASH1 4
#define ST_OT_PLAYER 5
#define ST_OT_DUST_PY 6
#define ST_OT_DUST_NY 7
#define ST_OT_TRASH_IMPLODE 8
#define ST_OT_TRASH2 9
#define ST_OT_PLAYER2 10
#define ST_OT_PLAYER3 11
#define ST_OT_GADGET 12
#define ST_OT_GADGET_IMPLODE 13
#define ST_OT_DUST_NXPY 14
#define ST_OT_DUST_NXNY 15

#ifndef ST_OBJ_CNT
    #define ST_OBJ_CNT 25
#endif

#ifndef ST_POINTS_PER_LEVEL
    #define ST_POINTS_PER_LEVEL 25
#endif 


/*================================================================*/
/* overall game state */
/*================================================================*/

#define ST_STATE_PREPARE 0
#define ST_STATE_IPREPARE 1
#define ST_STATE_GAME 2
#define ST_STATE_END 3
#define ST_STATE_IEND 4

/*================================================================*/
/* game difficulty */
/*================================================================*/
#ifndef ST_DIFF_VIS_LEN
    #define ST_DIFF_VIS_LEN 30
#endif 

#ifndef ST_DIFF_FP
    #define ST_DIFF_FP 5
#endif

#endif