#include "cli_view.h"

/// @file
/**
 * @brief Entry point
 *
 * Execution of the program
 * starts here.
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 *
 * @return Program exit status
 */
int main() {
  initwin();
  game_loop();
  endwin();

  return 0;
}

/**
 * @brief initwin
 *
 * Initialization of ncurses window.
 */
void initwin() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
}

/**
 * @brief Game loop
 *
 * Loops the game, updating the game, processing user inputs
 * and drawing the game each time cycle.
 */
void game_loop() {
  GameInfo_t stats = updateCurrentState();

  while (stats.pause != GAMEEXIT) {
    int signal = getch();
    processSignal(signal);

    stats = updateCurrentState();

    if (stats.pause != GAMEEXIT) {
      printAll(&stats);
    }
    usleep(5000);
  }
}

/**
 * @brief Print rectangle
 *
 * Prints rectangle using ncurses by passed coordinates in arguments.
 *
 * @param top_y Top y coordinate
 * @param bottom_y Bottom y coordinate
 * @param left_x Left x coordinate
 * @param right_x Right x coordinate
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

/**
 * @brief Print overlay
 *
 * Prints game overlay in rectangles for score, highscore, level, speed
 * and next figure (in tetris only).
 *
 * @param stats Basic game structure, passed from game model
 */
void print_overlay(GameInfo_t *stats) {
  print_rectangle(0, FIELD_HEIGHT + 1, 1, FIELD_WIDTH * 2 + 2);
  print_rectangle(0, FIELD_HEIGHT + 1, FIELD_WIDTH * 2 + 3,
                  FIELD_WIDTH * 2 + HUD_WIDTH + 2);

  print_rectangle(2, 4, FIELD_WIDTH * 2 + 4, FIELD_WIDTH * 2 + HUD_WIDTH + 1);
  print_rectangle(6, 8, FIELD_WIDTH * 2 + 4, FIELD_WIDTH * 2 + HUD_WIDTH + 1);
  print_rectangle(9, 11, FIELD_WIDTH * 2 + 4, FIELD_WIDTH * 2 + HUD_WIDTH + 1);
  print_rectangle(12, 14, FIELD_WIDTH * 2 + 4, FIELD_WIDTH * 2 + HUD_WIDTH + 1);
  if (stats->next)
    print_rectangle(16, 19, FIELD_WIDTH * 2 + 4,
                    FIELD_WIDTH * 2 + HUD_WIDTH + 1);

  MVPRINTW(1, FIELD_WIDTH * 2 + 6, "SCORE");
  MVPRINTW(5, FIELD_WIDTH * 2 + 5, "HI-SCORE");
  MVPRINTW(10, FIELD_WIDTH * 2 + 5, "LEVEL");
  MVPRINTW(13, FIELD_WIDTH * 2 + 5, "SPEED");
  if (stats->next) MVPRINTW(15, FIELD_WIDTH * 2 + 7, "NEXT");
}

/**
 * @brief Print stats
 *
 * Prints game stats, including score, highscore, level, speed
 * and next figure (in tetris only).
 *
 * @param stats Basic game structure, passed from game model
 */
void print_stats(GameInfo_t *stats) {
  MVPRINTW(3, FIELD_WIDTH * 2 + 7, "%d", stats->score);
  MVPRINTW(7, FIELD_WIDTH * 2 + 7, "%d", stats->high_score);
  MVPRINTW(10, FIELD_WIDTH * 2 + 11, "%d", stats->level);
  MVPRINTW(13, FIELD_WIDTH * 2 + 11, "%d", stats->speed);
  for (int i = 0; i < BRICK_SIDE; i++) {
    for (int j = 0; j < BRICK_SIDE; j++) {
      if (stats->next && stats->next[i][j])
        MVPRINTW(i + HUD_WIDTH + 4, 2 * j + HUD_WIDTH + 13, "%s", "[]");
    }
  }
}

/**
 * @brief Print everything
 *
 * Prints game overlay, stats, field, states (such as pause and gameover),
 * and controls.
 *
 * @param stats Basic game structure, passed from game model
 */
void printAll(GameInfo_t *stats) {
  erase();
  print_overlay(stats);
  print_stats(stats);
  print_field(stats);
  if (stats->pause == STARTING) {
    print_start();
  } else if (stats->pause == PAUSED) {
    print_pause();
  } else if (stats->pause == GAMELOST) {
    print_gameover();
  } else if (stats->pause == GAMEWON) {
    print_gameoverwon();
  }
  print_controls();
  refresh();
}

/**
 * @brief Print field
 *
 * Prints game field, cycling through every coordinate, printing "[]"
 * if a cell of the field exists.
 *
 * @param stats Basic game structure, passed from game model
 */
void print_field(GameInfo_t *stats) {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (stats->field[i][j]) MVPRINTW(i + 1, 2 * j + 2, "%s", "[]");
    }
  }
}

/**
 * @brief Print start
 *
 * Prints starting screen which waits for user input to start the game.
 */
void print_start() {
  mvaddstr(9, 4, "--------------------");
  mvaddstr(10, 4, "     Start game     ");
  mvaddstr(11, 4, "--------------------");
  mvaddstr(12, 4, "  'ENTER' to start  ");
  mvaddstr(13, 4, "--------------------");
  mvaddstr(14, 4, "   Or 'Q' to quit   ");
  mvaddstr(15, 4, "--------------------");
}

/**
 * @brief Print pause
 *
 * Prints pause screen which waits for user input to resume the game.
 */
void print_pause() {
  mvaddstr(9, 4, "--------------------");
  mvaddstr(10, 4, "        Pause       ");
  mvaddstr(11, 4, "--------------------");
  mvaddstr(12, 4, " Press 'P' to resume");
  mvaddstr(13, 4, "--------------------");
  mvaddstr(14, 4, "   Or 'Q' to quit   ");
  mvaddstr(15, 4, "--------------------");
}

/**
 * @brief Print gameover
 *
 * Prints gameover screen which waits for user input to restart the game.
 */
void print_gameover() {
  mvaddstr(9, 4, "--------------------");
  mvaddstr(10, 4, "      GAME OVER     ");
  mvaddstr(11, 4, "--------------------");
  mvaddstr(12, 4, "'ENTER' to try again");
  mvaddstr(13, 4, "--------------------");
  mvaddstr(14, 4, "   Or 'Q' to quit   ");
  mvaddstr(15, 4, "--------------------");
}

/**
 * @brief Print game won
 *
 * Prints game won screen which waits for user input to restart the game.
 */
void print_gameoverwon() {
  mvaddstr(9, 4, "--------------------");
  mvaddstr(10, 4, "       YOU WON!     ");
  mvaddstr(11, 4, "--------------------");
  mvaddstr(12, 4, "'ENTER' to try again");
  mvaddstr(13, 4, "--------------------");
  mvaddstr(14, 4, "   Or 'Q' to quit   ");
  mvaddstr(15, 4, "--------------------");
}

/**
 * @brief Print controls
 *
 * Prints game controls.
 */
void print_controls() {
  mvaddstr(3, 38, "CONTROLS:");
  mvaddstr(4, 38, "Space Rotate/Accelerate");
  mvaddstr(5, 38, "<- Move left");
  mvaddstr(6, 38, "-> Move right");
  mvaddstr(7, 38, "DownArrow Drop down/None");
  mvaddstr(8, 38, "P Pause");
  mvaddstr(9, 38, "Q Quit");
}
