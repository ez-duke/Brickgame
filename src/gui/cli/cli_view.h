#ifndef CLI_VIEW_H
#define CLI_VIEW_H

#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <unistd.h>

#include "cli_controller.h"

#define MVPRINTW(y, x, ...) \
  mvprintw(FIELDS_BEGIN + (y), FIELDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(FIELDS_BEGIN + (y), FIELDS_BEGIN + (x), c)

#define FIELDS_BEGIN 2
#define HUD_WIDTH 12

void initwin();
void game_loop();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_overlay(GameInfo_t *stats);
void print_stats(GameInfo_t *stats);
void printAll(GameInfo_t *stats);
void print_field(GameInfo_t *stats);
void print_start();
void print_pause();
void print_gameover();
void print_gameoverwon();
void print_controls();

#endif
