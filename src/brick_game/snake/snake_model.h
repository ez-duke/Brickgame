#ifndef SNAKE_MODEL_H
#define SNAKE_MODEL_H

#include <cstdlib>
#include <ctime>
#include <fstream>

#include "../../common.h"

namespace s21 {

/// @file
/**
 * @brief Look direction enum
 *
 * Enumeration of the possible look directions.
 */
typedef enum { LOOKLEFT = 0, LOOKUP, LOOKRIGHT, LOOKDOWN } LookDirection_t;

/**
 * @brief Apple struct
 *
 * This structure contains coordinates of the apple.
 */
struct Apple_t {
  int x;
  int y;
};

/**
 * @brief SnakeBody class
 *
 * This class is used to create the body of the snake.
 *
 * It consists of structure Node, pointers to snake's head and tail,
 * snake's size and some functions to operate with the body.
 */
class SnakeBody {
 private:
  /**
   * @brief Node structure
   *
   * This structure contains coordinates of the node and the pointer
   * to the next node.
   *
   * A node is snake's body segment.
   */
  struct Node {
    int x;
    int y;
    Node *next = nullptr;
  };

  Node *tail = nullptr;
  Node *head = nullptr;
  int size = 0;

 public:
  SnakeBody() {}

  ~SnakeBody() {
    while (this->size > 0) {
      this->pop();
    }
  }

  /**
   * @brief Push
   *
   * Adds new node to the start of the queue
   *
   * @param x X coordinate
   * @param y Y coordinate
   */
  void push(int x, int y) {
    Node *new_node = new Node;

    if (this->size == 0) {
      this->head = new_node;
      this->tail = new_node;
    } else {
      this->head->next = new_node;
      this->head = new_node;
    }

    this->head->x = x;
    this->head->y = y;
    ++this->size;
  }

  /**
   * @brief Push back
   *
   * Adds new node to the end of the queue
   *
   * @param x X coordinate
   * @param y Y coordinate
   */
  void pushBack(int x, int y) {
    Node *new_node = new Node;

    Node *temp = this->tail;
    this->tail = new_node;
    this->tail->next = temp;

    this->tail->x = x;
    this->tail->y = y;
    ++this->size;
  }

  /**
   * @brief Pop
   *
   * Pops last node of the queue
   */
  void pop() {
    if (this->size == 1) {
      delete this->tail;
    } else {
      Node *tmp = this->tail;
      this->tail = this->tail->next;
      delete tmp;
    }

    --this->size;
  }

  /**
   * @brief Get head
   *
   * Returns head of the snake.
   *
   * @return Node struct
   */
  Node *getHead() { return this->head; }

  /**
   * @brief Get tail
   *
   * Returns tail of the snake.
   *
   * @return Node struct
   */
  Node *getTail() { return this->tail; }

  /**
   * @brief Set size
   *
   * Sets current size to a new value.
   *
   * @param num New size
   */
  void setSize(int num) { this->size = num; }
};

/**
 * @brief Params struct
 *
 * The main structure which holds everything needed in the game.
 *
 * Contains game ticks, apple struct, game info struct, game state enum,
 * snake body class, look direction enum and user action enum.
 */
struct Params_t {
  int ticks = 0;
  Apple_t apple{};
  GameInfo_t stats{};
  GameState_t state = PAUSE;
  SnakeBody *body{};
  LookDirection_t direction = LOOKUP;
  UserAction_t signal = Up;

  Params_t(){};

  explicit Params_t(SnakeBody &body) { this->body = &body; }
};

/**
 * @brief SnakeModel class
 *
 * This class is used to operate the snake.
 *
 * It consists of a private structure Params and many functions.
 */
class SnakeModel {
 public:
  void fsm();

  void start();
  void statsInit();
  void clearField();
  void getHighScore();
  void spawnSnake();

  void spawn();
  void spawnApple();
  void findEmptySpace();

  void moving();
  void turnLeft();
  void turnRight();

  void shifting();
  int moveForward();
  int checkGameWon();
  int eatApple();
  void increaseLevel();
  void clearTail();

  void pause();
  void memAlloc();

  void gameOver();
  void saveHighScore();

  void gameWon();

  void exitState();

  /**
   * @brief Get params
   *
   * Returns updated params struct.
   *
   * @return Params struct
   */
  Params_t getPrms() { return *this->prms; }

  void freeMem();

  /**
   * @brief Set signal
   *
   * Sets current signal from the argument.
   *
   * @param action User action enum
   */
  void setSignal(UserAction_t action) { this->prms->signal = action; }

  SnakeModel(){};

  explicit SnakeModel(Params_t &params) : prms(&params){};

 private:
  Params_t *prms{};
};

}  // namespace s21

#endif
