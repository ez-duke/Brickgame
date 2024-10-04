#include "snake_model.h"

/// @file

static s21::SnakeBody snake_body;
static s21::Params_t params(snake_body);
static s21::SnakeModel Snake(params);

/**
 * @brief Update current state
 *
 * Updates game state and clears the signal.
 *
 * @return Game info structure
 */
GameInfo_t updateCurrentState() {
  Snake.fsm();
  Snake.setSignal(Up);

  return Snake.getPrms().stats;
}

/**
 * @brief Finite state machine
 *
 * FSM logic is implemented here. For every state there is a
 * corresponding function.
 */
void s21::SnakeModel::fsm() {
  switch (this->prms->state) {
    case START:
      start();
      break;

    case SPAWN:
      spawn();
      break;

    case MOVING:
      moving();
      break;

    case SHIFTING:
      shifting();
      break;

    case PAUSE:
      pause();
      break;

    case GAMEOVER:
      gameOver();
      break;

    case GAMEOVERWON:
      gameWon();
      break;

    case EXIT_STATE:
      exitState();
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
 */
void s21::SnakeModel::start() {
  switch (this->prms->signal) {
    case Start:
      statsInit();
      this->prms->state = SPAWN;
      break;

    case Terminate:
      this->prms->state = EXIT_STATE;
      break;

    default:
      break;
  }
}

/**
 * @brief Stats init
 *
 * Initializes score, level, speed, ticks, clears the field on new game,
 * sets rand(), reads highscore from file and spawns snake.
 */
void s21::SnakeModel::statsInit() {
  this->prms->stats.score = 0;
  this->prms->stats.level = 1;
  this->prms->stats.speed = 1;
  this->prms->ticks = 0;

  if (this->prms->stats.pause == GAMELOST ||
      this->prms->stats.pause == GAMEWON) {
    clearField();
  } else {
    srand(time(NULL));
  }
  this->prms->stats.pause = PLAYING;

  getHighScore();
  spawnSnake();
}

/**
 * @brief Clear field
 *
 * Clears the game field.
 */
void s21::SnakeModel::clearField() {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      this->prms->stats.field[i][j] = 0;
    }
  }
}

/**
 * @brief Get highscore
 *
 * Opens a file and reads highscore.
 */
void s21::SnakeModel::getHighScore() {
  std::ifstream fp;
  fp.open("brick_game/snake/high_score.txt");
  if (!fp.is_open()) {
    fp.open("../../../brick_game/snake/high_score.txt");
  }

  if (fp.is_open()) {
    fp >> this->prms->stats.high_score;
    fp.close();
  } else {
    this->prms->stats.high_score = 0;
  }
}

/**
 * @brief Spawn snake
 *
 * Spawns a snake.
 */
void s21::SnakeModel::spawnSnake() {
  this->prms->direction = LOOKUP;
  this->prms->body->setSize(0);

  this->prms->stats.field[9][5] = 1;
  this->prms->stats.field[10][5] = 1;
  this->prms->stats.field[11][5] = 1;
  this->prms->stats.field[12][5] = 1;

  this->prms->body->push(5, 12);
  this->prms->body->push(5, 11);
  this->prms->body->push(5, 10);
  this->prms->body->push(5, 9);
}

/**
 * @brief Spawn state
 *
 * Spawns an apple and switches the state to MOVING.
 */
void s21::SnakeModel::spawn() {
  spawnApple();
  this->prms->state = MOVING;
}

/**
 * @brief Spawn apple
 *
 * Spawns an apple.
 */
void s21::SnakeModel::spawnApple() { findEmptySpace(); }

/**
 * @brief Find empty space
 *
 * Finds an unoccupied cell to spawn an apple into.
 */
void s21::SnakeModel::findEmptySpace() {
  this->prms->apple.x = rand() % 10;
  this->prms->apple.y = rand() % 20;

  while (this->prms->stats.field[prms->apple.y][prms->apple.x] == 1) {
    this->prms->apple.x = rand() % 10;
    this->prms->apple.y = rand() % 20;
  }

  this->prms->stats.field[prms->apple.y][prms->apple.x] = 2;
}

/**
 * @brief Moving state
 *
 * In this state the snake can be moved and accelerated. If action button is
 * pressed, accelerates the snake. The snake can also be moved left and right.
 * A pause button can be pressed to pause the game. Or a terminate
 * button can be pressed to exit the game. By default increases game ticks
 * and switches the state to SHIFTING if enough ticks were passed.
 */
void s21::SnakeModel::moving() {
  switch (this->prms->signal) {
    case Action:
      this->prms->state = SHIFTING;
      this->prms->ticks = 0;
      break;
    case Left:
      turnLeft();
      break;

    case Right:
      turnRight();
      break;
    case Pause:
      this->prms->state = PAUSE;
      this->prms->stats.pause = PAUSED;
      break;

    case Terminate:
      this->prms->state = EXIT_STATE;
      break;

    default:
      this->prms->ticks++;
      if (prms->ticks >= INITIAL_TIMEOUT * 0.5 / (prms->stats.speed * 0.1)) {
        this->prms->state = SHIFTING;
        this->prms->ticks = 0;
      }
  }
}

/**
 * @brief Turn left
 *
 * Switches the snake's look direction to a lefter one.
 */
void s21::SnakeModel::turnLeft() {
  switch (this->prms->direction) {
    case LOOKLEFT:
      this->prms->direction = LOOKDOWN;
      break;

    case LOOKDOWN:
      this->prms->direction = LOOKRIGHT;
      break;

    case LOOKRIGHT:
      this->prms->direction = LOOKUP;
      break;

    case LOOKUP:
      this->prms->direction = LOOKLEFT;
  }
}

/**
 * @brief Turn left
 *
 * Switches the snake's look direction to a righter one.
 */
void s21::SnakeModel::turnRight() {
  switch (this->prms->direction) {
    case LOOKLEFT:
      this->prms->direction = LOOKUP;
      break;

    case LOOKUP:
      this->prms->direction = LOOKRIGHT;
      break;

    case LOOKRIGHT:
      this->prms->direction = LOOKDOWN;
      break;

    case LOOKDOWN:
      this->prms->direction = LOOKLEFT;
  }
}

/**
 * @brief Shifting state
 *
 * In this state the snake moves on the field and eats an apple
 * on an encounter. In this case switches the game state to SPAWN.
 * If the snake encountered a solid surface, the game ends and
 * game state GAMEOVER is set. If the player eats 200 apples, sets
 * the game state to GAMEOVERWON. Otherwise MOVING state is set.
 */
void s21::SnakeModel::shifting() {
  int temp_x = this->prms->body->getTail()->x;
  int temp_y = this->prms->body->getTail()->y;
  clearTail();

  if (moveForward()) {
    this->prms->body->pushBack(temp_x, temp_y);
    this->prms->stats.field[temp_y][temp_x] = 1;

    this->prms->state = GAMEOVER;
  } else {
    if (eatApple()) {
      this->prms->body->pushBack(temp_x, temp_y);
      this->prms->stats.field[temp_y][temp_x] = 1;

      this->prms->state = SPAWN;
    } else {
      this->prms->state = MOVING;
    }

    if (checkGameWon()) this->prms->state = GAMEOVERWON;
  }
}

/**
 * @brief Move forward
 *
 * Moves the snake forward depending on the direction it's looking.
 * A new snake body node is pushed to the snake's head and
 * a node is popped from the snake's tail.
 *
 * If the snake encounters a wall or its own body, the game ends.
 *
 * @return Crash status
 */
int s21::SnakeModel::moveForward() {
  switch (this->prms->direction) {
    case LOOKLEFT:
      this->prms->body->push(this->prms->body->getHead()->x - 1,
                             this->prms->body->getHead()->y);
      break;

    case LOOKUP:
      this->prms->body->push(this->prms->body->getHead()->x,
                             this->prms->body->getHead()->y - 1);
      break;

    case LOOKRIGHT:
      this->prms->body->push(this->prms->body->getHead()->x + 1,
                             this->prms->body->getHead()->y);
      break;

    case LOOKDOWN:
      this->prms->body->push(this->prms->body->getHead()->x,
                             this->prms->body->getHead()->y + 1);
  }

  int game_over = 0;
  if (this->prms->body->getHead()->x < 0 ||
      this->prms->body->getHead()->x >= FIELD_WIDTH ||
      this->prms->body->getHead()->y < 0 ||
      this->prms->body->getHead()->y >= FIELD_HEIGHT ||
      this->prms->stats
              .field[prms->body->getHead()->y][prms->body->getHead()->x] == 1)
    game_over = 1;
  else
    this->prms->stats
        .field[prms->body->getHead()->y][prms->body->getHead()->x] = 1;

  return game_over;
}

/**
 * @brief Check game won
 *
 * Defines if the snake has eaten 200 apples or not. If it has,
 * the game won status is returned.
 *
 * @return Game won status
 */
int s21::SnakeModel::checkGameWon() {
  int game_won = 0;
  if (this->prms->stats.score == 200) game_won = 1;

  return game_won;
}

/**
 * @brief Eat apple
 *
 * Defines if the snake's head coordinates are mathing the apple's coordinates,
 * and if so, the apple is eaten, the score and level are increased,
 * and snake's body is not decreased.
 *
 * @return Apple eaten status
 */
int s21::SnakeModel::eatApple() {
  int ate_apple = 0;
  if (this->prms->body->getHead()->x == this->prms->apple.x &&
      this->prms->body->getHead()->y == this->prms->apple.y) {
    this->prms->stats.score += 1;

    if (this->prms->stats.score > this->prms->stats.high_score) {
      this->prms->stats.high_score = this->prms->stats.score;
    }
    increaseLevel();
    ate_apple = 1;
  }
  return ate_apple;
}

/**
 * @brief Increase level
 *
 * Increases the game level depending on the score.
 *
 * Every 5 points grant 1 level.
 */
void s21::SnakeModel::increaseLevel() {
  this->prms->stats.level = 1 + (this->prms->stats.score / 5);
  if (this->prms->stats.level > 10) this->prms->stats.level = 10;
  this->prms->stats.speed = this->prms->stats.level;
}

/**
 * @brief Clear tail
 *
 * Pops the snake's tail and changes the tail's coordinates.
 */
void s21::SnakeModel::clearTail() {
  this->prms->stats
      .field[this->prms->body->getTail()->y][this->prms->body->getTail()->x] =
      0;
  this->prms->body->pop();
}

/**
 * @brief Pause state
 *
 * Pauses and unpauses the game.
 *
 * The game starts with this state, the memory is also allocated here.
 */
void s21::SnakeModel::pause() {
  if (this->prms->stats.field == NULL && this->prms->stats.pause == STARTING) {
    memAlloc();
  }

  switch (this->prms->signal) {
    case Start:
      if (this->prms->stats.pause == STARTING) {
        this->prms->state = START;
        this->prms->stats.pause = PLAYING;
        Snake.fsm();
      }
      break;

    case Pause:
      this->prms->state = MOVING;
      this->prms->stats.pause = PLAYING;
      break;

    case Terminate:
      this->prms->state = EXIT_STATE;
      break;

    default:
      break;
  }
}

/**
 * @brief Allocate memory
 *
 * Allocates memory for the field.
 */
void s21::SnakeModel::memAlloc() {
  this->prms->stats.field = new int *[FIELD_HEIGHT]();
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    this->prms->stats.field[i] = new int[FIELD_WIDTH]();
  }
}

/**
 * @brief Gameover state
 *
 * Happens when the game is lost. Sets the state to START and pauses the game.
 */
void s21::SnakeModel::gameOver() {
  this->prms->state = START;
  this->prms->stats.pause = GAMELOST;
  saveHighScore();
}

/**
 * @brief Save highscore
 *
 * Opens a file and saves current game's score if it's exceeded highscore.
 */
void s21::SnakeModel::saveHighScore() {
  if (this->prms->stats.high_score == this->prms->stats.score &&
      this->prms->stats.score > 0) {
    std::ofstream fp;
    fp.open("brick_game/snake/high_score.txt");
    if (!fp.is_open()) {
      fp.open("../../../brick_game/snake/high_score.txt");
    }

    if (fp.is_open()) {
      fp << this->prms->stats.high_score;
      fp.close();
    }
  }
}

/**
 * @brief Game won state
 *
 * Happens when the game is won. Sets the state to START and pauses the game.
 */
void s21::SnakeModel::gameWon() {
  this->prms->state = START;
  this->prms->stats.pause = GAMEWON;
  saveHighScore();
}

/**
 * @brief Exit state
 *
 * This state is the last state before exitin the game.
 *
 * Memory deallocation happens here and the state is set to GAMEEXIT.
 */
void s21::SnakeModel::exitState() {
  this->prms->stats.pause = GAMEEXIT;
  freeMem();
}

/**
 * @brief Free memory
 *
 * Frees allocated memory from current object.
 */
void s21::SnakeModel::freeMem() {
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    delete[] this->prms->stats.field[i];
    this->prms->stats.field[i] = nullptr;
  }

  delete[] this->prms->stats.field;
  this->prms->stats.field = nullptr;
}

/**
 * @brief Memory free
 *
 * Frees allocated memory. No object needed.
 */
void memFree() {
  GameInfo_t stats = getStats();

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    delete[] stats.field[i];
    stats.field[i] = nullptr;
  }

  delete[] stats.field;
  stats.field = nullptr;
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
  Snake.setSignal(action);
}

/**
 * @brief Get stats
 *
 * Returns updated Game info struct.
 *
 * @return Game info struct
 */
GameInfo_t getStats() { return Snake.getPrms().stats; }
