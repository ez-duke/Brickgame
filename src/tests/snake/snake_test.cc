#include "snake_test.h"

TEST(test_snake, Start) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.setSignal(Start);
  Snake.start();
  Snake.spawn();
  EXPECT_EQ(MOVING, prms.state);
  prms.state = EXIT_STATE;
  Snake.fsm();
}

TEST(test_snake, Initialize) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.start();

  EXPECT_EQ(1, prms.stats.field[9][5]);
  EXPECT_EQ(1, prms.stats.field[10][5]);
  EXPECT_EQ(1, prms.stats.field[11][5]);
  EXPECT_EQ(1, prms.stats.field[12][5]);
  EXPECT_EQ(0, prms.stats.score);
  EXPECT_EQ(PLAYING, prms.stats.pause);
  EXPECT_EQ(1, prms.stats.level);
  EXPECT_EQ(1, prms.stats.speed);
  EXPECT_NE(nullptr, prms.stats.field);
  EXPECT_EQ(nullptr, prms.stats.next);
  prms.state = EXIT_STATE;
  Snake.fsm();
}

TEST(test_snake, MOVING) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.start();

  Snake.setSignal(Left);
  prms.state = MOVING;
  Snake.fsm();
  prms.state = SHIFTING;
  Snake.fsm();
  EXPECT_EQ(1, prms.stats.field[9][4]);

  Snake.setSignal(Right);
  prms.state = MOVING;
  Snake.fsm();
  prms.state = SHIFTING;
  Snake.fsm();
  EXPECT_EQ(1, prms.stats.field[8][4]);
  prms.state = EXIT_STATE;
  Snake.fsm();
}

TEST(test_snake, SHIFTING) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.start();
  prms.state = SHIFTING;
  Snake.fsm();
  EXPECT_EQ(1, prms.stats.field[8][5]);
  EXPECT_EQ(0, prms.stats.field[12][5]);
  prms.state = EXIT_STATE;
  Snake.fsm();
}

TEST(test_snake, PAUSE) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.start();

  Snake.setSignal(Pause);
  prms.state = MOVING;
  Snake.fsm();
  EXPECT_EQ(PAUSE, prms.state);
  EXPECT_EQ(PAUSED, prms.stats.pause);
  prms.state = EXIT_STATE;
  Snake.fsm();
}

TEST(test_snake, EatApple) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.start();

  prms.state = SHIFTING;
  prms.apple.x = 5;
  prms.apple.y = 8;
  Snake.fsm();
  EXPECT_EQ(SPAWN, prms.state);
  prms.state = EXIT_STATE;
  Snake.fsm();
}

TEST(test_snake, GAMEOVER) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.start();

  prms.state = GAMEOVER;
  Snake.fsm();
  EXPECT_EQ(START, prms.state);
  EXPECT_EQ(GAMELOST, prms.stats.pause);
  prms.state = EXIT_STATE;
  Snake.fsm();
}

TEST(test_snake, GAMEOVERWON) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.start();

  prms.state = GAMEOVERWON;
  Snake.fsm();
  EXPECT_EQ(START, prms.state);
  EXPECT_EQ(GAMEWON, prms.stats.pause);
  prms.state = EXIT_STATE;
  Snake.fsm();
}

TEST(test_snake, EXIT_STATE) {
  s21::SnakeBody body{};
  s21::Params_t prms{body};
  s21::SnakeModel Snake{prms};

  Snake.setSignal(Start);
  Snake.pause();
  Snake.start();

  prms.state = EXIT_STATE;
  Snake.fsm();
  EXPECT_EQ(GAMEEXIT, prms.stats.pause);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
