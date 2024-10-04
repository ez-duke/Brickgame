#ifndef BRICKGAME_COMMON_H
#define BRICKGAME_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define KEY_DOWN 0402
#define KEY_UP 0403
#define KEY_LEFT 0404
#define KEY_RIGHT 0405
#define NO_INPUT -1
#define ENTER_KEY 10

#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define BRICK_SIDE 4

#ifdef __APPLE__
#define INITIAL_TIMEOUT 50
#else
#define INITIAL_TIMEOUT 350
#endif

/**
 * @brief Pause enum
 *
 * Enumeration of the possible types of game pause.
 */
typedef enum {
  STARTING = 0,
  PLAYING,
  PAUSED,
  GAMELOST,
  GAMEEXIT,
  GAMEWON
} PauseType_t;

/**
 * @brief Pause enum
 *
 * Enumeration of the possible types of game pause.
 */
typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  PAUSE,
  GAMEOVER,
  GAMEOVERWON,
  EXIT_STATE
} GameState_t;

/**
 * @brief User action enum
 *
 * Enumeration of the possible types of actions made by user.
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * @brief Game info struct
 *
 * The sub-main structure which holds the minimum needed
 * both in the game model and game view.
 *
 * Contains game field matrix, next figure matrix, score, highscore,
 * level, speed and pause.
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

GameInfo_t updateCurrentState();
void userInput(UserAction_t action, bool hold);

GameInfo_t getStats();
void memFree();

#ifdef __cplusplus
}
#endif

#endif
