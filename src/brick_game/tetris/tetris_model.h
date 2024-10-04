#ifndef TETRIS_MODEL_H
#define TETRIS_MODEL_H

/// @file
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../common.h"

#define BRICKSTART_X 3
#define BRICKSTART_Y -1

/**
 * @brief Brick piece enum
 *
 * Enumeration of the possible forms of figures.
 */
typedef enum {
  I_PIECE = 0,
  J_PIECE,
  L_PIECE,
  O_PIECE,
  S_PIECE,
  T_PIECE,
  Z_PIECE
} BrickPiece_t;

/**
 * @brief Brick struct
 *
 * A structure which holds brick coordinates (x,y), matrix of the brick,
 * enumerated type of the current and next bricks.
 */
typedef struct {
  BrickPiece_t piece;
  BrickPiece_t next_brick;
  int x;
  int y;
  int matrix[4][4];
} Brick_t;

/**
 * @brief Params struct
 *
 * The main structure which holds everything needed in the game.
 *
 * Contains game ticks, complete lines at once, brick struct,
 * game info struct, game state enum and user action enum.
 */
typedef struct {
  int ticks;
  int lines_at_once;
  Brick_t brick;
  GameInfo_t stats;
  GameState_t state;
  UserAction_t signal;
} Params_t;

Params_t *get_params();
void fsm(Params_t *prms);

void start(Params_t *prms);
int field_alloc(Params_t *prms);
int brick_alloc(Params_t *prms);
void stats_init(Params_t *prms);
void generate_brick(int id, Params_t *prms);

void spawn(Params_t *prms);
void spawn_brick(Params_t *prms);

void moving(Params_t *prms);
void clear_brick(Params_t *prms);
void place_brick(Params_t *prms);
void rotate_brick(Params_t *prms);
void rotate_backwards(Params_t *prms);
void moveright(Params_t *prms);
void moveleft(Params_t *prms);
void movedown(Params_t *prms);

void shifting(Params_t *prms);
int check_collision(Params_t *prms);

void attaching(Params_t *prms);
void remove_line(Params_t *prms);
void increase_score(Params_t *prms);
void increase_level(Params_t *prms);
int check_game_over(Params_t *prms);
void move_field_down(Params_t *prms, int line);

void pause(Params_t *prms);
int mem_alloc(Params_t *prms);

void gameover(Params_t *prms);
void saveHighScore(Params_t *prms);

void exit_state(Params_t *prms);
void mem_free(GameInfo_t *stats);

#endif
