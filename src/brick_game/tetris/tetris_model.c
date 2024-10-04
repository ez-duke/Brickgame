#include "tetris_model.h"

/// @file
/**
 * @brief Get params
 *
 * Declares a static params struct one time and returns it
 * every time this function is called.
 *
 * @return Params structure
 */
Params_t *get_params() {
  static Params_t prms = {.state = PAUSE, .signal = Up};

  return &prms;
}

/**
 * @brief Update current state
 *
 * Updates game state and clears the signal.
 *
 * @return Game info structure
 */
GameInfo_t updateCurrentState() {
  Params_t *prms = get_params();

  fsm(prms);
  prms->signal = Up;

  return prms->stats;
}

/**
 * @brief Finite state machine
 *
 * FSM logic is implemented here. For every state there is a
 * corresponding function.
 *
 * @param prms Params structure
 */
void fsm(Params_t *prms) {
  switch (prms->state) {
    case START:
      start(prms);
      break;

    case SPAWN:
      spawn(prms);
      break;

    case MOVING:
      moving(prms);
      break;

    case SHIFTING:
      shifting(prms);
      break;

    case ATTACHING:
      attaching(prms);
      break;

    case PAUSE:
      pause(prms);
      break;

    case GAMEOVER:
      gameover(prms);
      break;

    case EXIT_STATE:
      exit_state(prms);
      break;

    default:
      break;
  }
}

/**
 * @brief Start state
 *
 * Based on the signal starts the game with stats initializaion
 * and changes the state to SPAWN or changes the state to EXIT_STATE.
 *
 * @param prms Params structure
 */
void start(Params_t *prms) {
  switch (prms->signal) {
    case Start:
      stats_init(prms);
      prms->state = SPAWN;
      break;

    case Terminate:
      prms->state = EXIT_STATE;
      break;

    default:
      break;
  }
}

/**
 * @brief Field alloc
 *
 * Allocates memory of the game field.
 *
 * @param prms Params structure
 *
 * @return Memory allocation status
 */
int field_alloc(Params_t *prms) {
  int error = 0;
  prms->stats.field = calloc(FIELD_HEIGHT, sizeof(int *));
  if (prms->stats.field == NULL) error = 1;

  for (int i = 0; !error && i < FIELD_HEIGHT; i++) {
    prms->stats.field[i] = calloc(FIELD_WIDTH, sizeof(int));
    if (prms->stats.field[i] == NULL) {
      error = 1;
      for (int j = 0; j < i; j++) {
        free(prms->stats.field[j]);
      }
      free(prms->stats.field);
    }
  }
  return error;
}

/**
 * @brief Brick alloc
 *
 * Allocates memory of the next figure.
 *
 * @param prms Params structure
 *
 * @return Memory allocation status
 */
int brick_alloc(Params_t *prms) {
  int error = 0;
  prms->stats.next = calloc(BRICK_SIDE, sizeof(int *));
  if (prms->stats.next == NULL) error = 1;

  for (int i = 0; !error && i < BRICK_SIDE; i++) {
    prms->stats.next[i] = calloc(BRICK_SIDE, sizeof(int));
    if (prms->stats.next[i] == NULL) {
      error = 1;
      for (int j = 0; j < i; j++) {
        free(prms->stats.next[j]);
      }
      free(prms->stats.next);
    }
  }
  return error;
}

/**
 * @brief Stats init
 *
 * Initializes score, level, speed, ticks, clears the field on new game,
 * sets rand(), generates next brick, reads highscore from file.
 *
 * @param prms Params structure
 */
void stats_init(Params_t *prms) {
  prms->stats.score = 0;
  prms->stats.level = 1;
  prms->stats.speed = 1;
  prms->ticks = 0;

  if (prms->stats.pause == GAMELOST) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      for (int j = 0; j < FIELD_WIDTH; j++) {
        prms->stats.field[i][j] = 0;
      }
    }

    for (int i = 0; i < BRICK_SIDE; i++) {
      for (int j = 0; j < BRICK_SIDE; j++) {
        prms->stats.next[i][j] = 0;
      }
    }

    prms->stats.pause = PLAYING;
  } else {
    srand(time(NULL));
  }
  generate_brick(rand() % 7, prms);

  FILE *fp = fopen("brick_game/tetris/high_score.txt", "r");
  if (!fp) {
    fp = fopen("../../../brick_game/tetris/high_score.txt", "r");
  }

  if (fp) {
    fscanf(fp, "%d", &prms->stats.high_score);
    fclose(fp);
  } else {
    prms->stats.high_score = 0;
  }
}

/**
 * @brief Generate brick
 *
 * Generates a new figure based on its randomly generated id.
 *
 * @param id Brick id
 * @param prms Params structure
 */
void generate_brick(int id, Params_t *prms) {
  prms->brick.next_brick = id;
  switch (id) {
    case I_PIECE:
      prms->stats.next[1][0] = prms->stats.next[1][1] = prms->stats.next[1][2] =
          prms->stats.next[1][3] = 1;
      break;

    case J_PIECE:
      prms->stats.next[1][0] = prms->stats.next[1][1] = prms->stats.next[1][2] =
          prms->stats.next[2][2] = 1;
      break;

    case L_PIECE:
      prms->stats.next[1][0] = prms->stats.next[1][1] = prms->stats.next[1][2] =
          prms->stats.next[2][0] = 1;
      break;

    case O_PIECE:
      prms->stats.next[1][1] = prms->stats.next[1][2] = prms->stats.next[2][1] =
          prms->stats.next[2][2] = 1;
      break;

    case S_PIECE:
      prms->stats.next[2][0] = prms->stats.next[2][1] = prms->stats.next[1][1] =
          prms->stats.next[1][2] = 1;
      break;

    case T_PIECE:
      prms->stats.next[1][0] = prms->stats.next[1][1] = prms->stats.next[1][2] =
          prms->stats.next[2][1] = 1;
      break;

    case Z_PIECE:
      prms->stats.next[1][0] = prms->stats.next[1][1] = prms->stats.next[2][1] =
          prms->stats.next[2][2] = 1;
  }
}

/**
 * @brief Spawn state
 *
 * Spawns a figure and generates a new one, switches the state to MOVING.
 *
 * @param prms Params structure
 */
void spawn(Params_t *prms) {
  spawn_brick(prms);

  generate_brick(rand() % 7, prms);
  prms->state = MOVING;
}

/**
 * @brief Spawn brick
 *
 * Spawns a figure.
 *
 * @param prms Params structure
 */
void spawn_brick(Params_t *prms) {
  for (int i = 0; i < BRICK_SIDE; i++) {
    for (int j = 0; j < BRICK_SIDE; j++) {
      prms->brick.matrix[i][j] = prms->stats.next[i][j];
      prms->stats.next[i][j] = 0;
    }
  }
  prms->brick.x = BRICKSTART_X;
  prms->brick.y = BRICKSTART_Y;
  prms->brick.piece = prms->brick.next_brick;
}

/**
 * @brief Moving state
 *
 * In this state the figure can be moved and rotated. If action button is
 * pressed, rotates the figure. The figure can also be moved left and right, and
 * dropped down. A pause button can be pressed to pause the game. Or a terminate
 * button can be pressed to exit the game. By default increases game ticks
 * and switches the state to SHIFTING if enough ticks were passed.
 *
 * @param prms Params structure
 */
void moving(Params_t *prms) {
  clear_brick(prms);
  switch (prms->signal) {
    case Action:
      if (prms->brick.piece != O_PIECE) {
        rotate_brick(prms);
        if (check_collision(prms)) rotate_backwards(prms);
      }
      break;

    case Left:
      moveleft(prms);
      break;

    case Right:
      moveright(prms);
      break;

    case Down:
      movedown(prms);
      break;

    case Pause:
      prms->state = PAUSE;
      prms->stats.pause = PAUSED;
      break;

    case Terminate:
      prms->state = EXIT_STATE;
      break;

    default:
      prms->ticks++;
      if (prms->ticks >= INITIAL_TIMEOUT / (prms->stats.speed * 0.1)) {
        prms->state = SHIFTING;
        prms->ticks = 0;
      }
  }
  place_brick(prms);
}

/**
 * @brief Clear brick
 *
 * Clears the figure from the field before moving.
 *
 * @param prms Params structure
 */
void clear_brick(Params_t *prms) {
  for (int i = 0; i < BRICK_SIDE; i++) {
    for (int j = 0; j < BRICK_SIDE; j++) {
      if (prms->brick.matrix[i][j] == 1) {
        prms->stats.field[i + prms->brick.y][j + prms->brick.x] = 0;
      }
    }
  }
}

/**
 * @brief Place brick
 *
 * Places the figure to the field after moving.
 *
 * @param prms Params structure
 */
void place_brick(Params_t *prms) {
  for (int i = 0; i < BRICK_SIDE; i++) {
    for (int j = 0; j < BRICK_SIDE; j++) {
      if (prms->brick.matrix[i][j] == 1) {
        prms->stats.field[i + prms->brick.y][j + prms->brick.x] = 1;
      }
    }
  }
}

/**
 * @brief Rotate brick
 *
 * Rotates the figure clockwise.
 *
 * @param prms Params structure
 */
void rotate_brick(Params_t *prms) {
  Brick_t temp = {};
  int adjustment;

  if (prms->brick.piece == I_PIECE)
    adjustment = 0;
  else
    adjustment = 1;

  for (int i = 0; i < BRICK_SIDE - adjustment; i++) {
    for (int j = 0; j < BRICK_SIDE - adjustment; j++) {
      temp.matrix[i][BRICK_SIDE - j - 1 - adjustment] =
          prms->brick.matrix[j][i];
    }
  }

  for (int i = 0; i < BRICK_SIDE - adjustment; i++) {
    for (int j = 0; j < BRICK_SIDE - adjustment; j++) {
      prms->brick.matrix[i][j] = temp.matrix[i][j];
    }
  }
}

/**
 * @brief Rotate brick backwards
 *
 * Rotates the figure counterclockwise.
 *
 * @param prms Params structure
 */
void rotate_backwards(Params_t *prms) {
  Brick_t temp = {};
  int adjustment;

  if (prms->brick.piece == I_PIECE)
    adjustment = 0;
  else
    adjustment = 1;

  for (int i = 0; i < BRICK_SIDE - adjustment; i++) {
    for (int j = 0; j < BRICK_SIDE - adjustment; j++) {
      temp.matrix[j][i] =
          prms->brick.matrix[i][BRICK_SIDE - j - 1 - adjustment];
    }
  }

  for (int i = 0; i < BRICK_SIDE - adjustment; i++) {
    for (int j = 0; j < BRICK_SIDE - adjustment; j++) {
      prms->brick.matrix[i][j] = temp.matrix[i][j];
    }
  }
}

/**
 * @brief Move left
 *
 * Moves the figure to the left.
 *
 * @param prms Params structure
 */
void moveleft(Params_t *prms) {
  prms->brick.x--;
  if (check_collision(prms)) prms->brick.x++;
}

/**
 * @brief Move right
 *
 * Moves the figure to the right.
 *
 * @param prms Params structure
 */
void moveright(Params_t *prms) {
  prms->brick.x++;
  if (check_collision(prms)) prms->brick.x--;
}

/**
 * @brief Move down
 *
 * Drops the figure down.
 *
 * @param prms Params structure
 */
void movedown(Params_t *prms) {
  int collision = 0;

  while (!collision) {
    prms->brick.y++;
    collision = check_collision(prms);
    if (collision) prms->brick.y--;
  }

  prms->state = ATTACHING;
}

/**
 * @brief Shifting state
 *
 * In this state the figure moves down the field and places to the field
 * if it encountered a solid surface. In this case switches the
 * game state to ATTACHING. Otherwise returns to MOVING state.
 *
 * @param prms Params structure
 */
void shifting(Params_t *prms) {
  clear_brick(prms);
  prms->brick.y++;

  if (check_collision(prms)) {
    prms->brick.y--;
    prms->state = ATTACHING;
  } else {
    prms->state = MOVING;
    place_brick(prms);
  }
}

/**
 * @brief Check collision
 *
 * Defines if a collision with a solid ground has occured.
 *
 * @param prms Params structure
 *
 * @return Collision status
 */
int check_collision(Params_t *prms) {
  int collision = 0;

  for (int i = 0; !collision && i < BRICK_SIDE; i++) {
    for (int j = 0; j < BRICK_SIDE; j++) {
      if (prms->brick.matrix[i][j] == 1 &&
          ((i + prms->brick.y >= FIELD_HEIGHT || i + prms->brick.y < 0 ||
            j + prms->brick.x >= FIELD_WIDTH || j + prms->brick.x < 0) ||
           (prms->stats.field[i + prms->brick.y][prms->brick.x + j] == 1)))

        collision = 1;
    }
  }
  return collision;
}

/**
 * @brief Attaching state
 *
 * Attaches the figure to the field and spawns a new one, setting the state
 * to SPAWN. If a new figure can't be spawned, sets the state to GAMEOVER.
 *
 * On a brick placement checks if any lines can be removed.
 *
 * @param prms Params structure
 */
void attaching(Params_t *prms) {
  place_brick(prms);
  remove_line(prms);

  if (check_game_over(prms))
    prms->state = GAMEOVER;
  else {
    prms->state = SPAWN;
  }
}

/**
 * @brief Remove line
 *
 * Removes a complete line, moves field down and increases the score.
 *
 * @param prms Params structure
 */
void remove_line(Params_t *prms) {
  int sum = 1;
  for (int i = FIELD_HEIGHT - 1; sum > 0 && i > 0; i--) {
    sum = prms->stats.field[i][0] + prms->stats.field[i][1] +
          prms->stats.field[i][2] + prms->stats.field[i][3] +
          prms->stats.field[i][4] + prms->stats.field[i][5] +
          prms->stats.field[i][6] + prms->stats.field[i][7] +
          prms->stats.field[i][8] + prms->stats.field[i][9];

    if (sum == FIELD_WIDTH) {
      prms->lines_at_once += 1;
      move_field_down(prms, i);
      i++;
    }
  }
  if (prms->lines_at_once > 0) increase_score(prms);
  prms->lines_at_once = 0;
}

/**
 * @brief Move field down
 *
 * Moves field down for every complete line.
 *
 * @param prms Params structure
 */
void move_field_down(Params_t *prms, int line) {
  for (int i = line; i > 0; i--) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      prms->stats.field[i][j] = prms->stats.field[i - 1][j];
    }
  }
}

/**
 * @brief Increase score
 *
 * Increases the game score depending on complete lines at once.
 *
 * Also increases level and updates highscore.
 *
 * @param prms Params structure
 */
void increase_score(Params_t *prms) {
  switch (prms->lines_at_once) {
    case 1:
      prms->stats.score += 100;
      break;

    case 2:
      prms->stats.score += 300;
      break;

    case 3:
      prms->stats.score += 700;
      break;

    case 4:
      prms->stats.score += 1500;
  }

  if (prms->stats.score > prms->stats.high_score) {
    prms->stats.high_score = prms->stats.score;
  }

  if (prms->stats.level < 10) increase_level(prms);
}

/**
 * @brief Increase level
 *
 * Increases the game level depending on the score.
 *
 * Every 600 points grant 1 level.
 *
 * @param prms Params structure
 */
void increase_level(Params_t *prms) {
  prms->stats.level = 1 + (prms->stats.score / 600);
  if (prms->stats.level > 10) prms->stats.level = 10;
  prms->stats.speed = prms->stats.level;
}

/**
 * @brief Check game over
 *
 * Defines if any new figures can be spawned. Looks at the top line
 * of the field.
 *
 * @param prms Params structure
 */
int check_game_over(Params_t *prms) {
  int game_over = 0;
  for (int i = 0; i < FIELD_WIDTH; i++) {
    game_over += prms->stats.field[0][i];
  }
  return game_over > 0;
}

/**
 * @brief Pause state
 *
 * Pauses and unpauses the game.
 *
 * The game starts with this state, the memory is also allocated here.
 *
 * @param prms Params structure
 */
void pause(Params_t *prms) {
  if (prms->stats.field == NULL && prms->stats.pause == STARTING) {
    if (mem_alloc(prms)) {
      prms->state = EXIT_STATE;
    }
  }

  switch (prms->signal) {
    case Start:
      if (prms->stats.pause == STARTING) {
        prms->state = START;
        prms->stats.pause = PLAYING;
        fsm(prms);
      }
      break;

    case Pause:
      prms->state = MOVING;
      prms->stats.pause = PLAYING;
      break;

    case Terminate:
      prms->state = EXIT_STATE;
      break;

    default:
      break;
  }
}

/**
 * @brief Allocate memory
 *
 * Allocates memory for the field and figure.
 *
 * @param prms Params structure
 *
 * @return Allocation status
 */
int mem_alloc(Params_t *prms) {
  int error;
  error = field_alloc(prms);
  if (!error) error = brick_alloc(prms);
  return error;
}

/**
 * @brief Gameover state
 *
 * Happens when the game is lost. Sets the state to START and pauses the game.
 *
 * @param prms Params structure
 */
void gameover(Params_t *prms) {
  prms->state = START;
  prms->stats.pause = GAMELOST;
  saveHighScore(prms);
}

/**
 * @brief Save highscore
 *
 * Opens a file and saves current game's score if it's exceeded highscore.
 *
 * @param prms Params structure
 */
void saveHighScore(Params_t *prms) {
  if (prms->stats.high_score == prms->stats.score && prms->stats.score > 0) {
    FILE *fp = fopen("brick_game/tetris/high_score.txt", "w");
    if (!fp) {
      fp = fopen("../../../brick_game/tetris/high_score.txt", "w");
    }

    if (fp) {
      fprintf(fp, "%d", prms->stats.high_score);
      fclose(fp);
    }
  }
}

/**
 * @brief Exit state
 *
 * This state is the last state before exitin the game.
 *
 * Memory deallocation happens here and the state is set to GAMEEXIT.
 *
 * @param prms Params structure
 */
void exit_state(Params_t *prms) {
  prms->stats.pause = GAMEEXIT;
  memFree();
}

/**
 * @brief Memory free
 *
 * Frees allocated memory. No argument needed.
 */
void memFree() {
  GameInfo_t stats = getStats();

  mem_free(&stats);
}

/**
 * @brief Free memory
 *
 * Frees allocated memory from an argument.
 *
 * @param stats Game info structure
 */
void mem_free(GameInfo_t *stats) {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    free(stats->field[i]);
  }
  free(stats->field);
  stats->field = NULL;

  for (int i = 0; i < BRICK_SIDE; i++) {
    free(stats->next[i]);
  }
  free(stats->next);
  stats->next = NULL;
}

/**
 * @brief User input
 *
 * Accepts user's proccessed input into action.
 *
 * Updates current signal with new action.
 *
 * @param action User action enum
 * @param hold Is button held or not
 */
void userInput(UserAction_t action, bool hold) {
  (void)hold;
  Params_t *prms = get_params();
  prms->signal = action;
}

/**
 * @brief Get stats
 *
 * Returns updated Game info struct.
 *
 * @return Game info struct
 */
GameInfo_t getStats() {
  Params_t *prms = get_params();

  return prms->stats;
}
