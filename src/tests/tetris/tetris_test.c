#include "tetris_test.h"

START_TEST(test0) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  start(&prms);
  ck_assert_int_eq(SPAWN, prms.state);
  mem_free(&prms.stats);
}
END_TEST

START_TEST(test1) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      ck_assert_int_eq(0, prms.stats.field[i][j]);
    }
  }
  ck_assert_int_eq(0, prms.stats.score);
  ck_assert_int_eq(PLAYING, prms.stats.pause);
  ck_assert_int_eq(1, prms.stats.level);
  ck_assert_int_eq(1, prms.stats.speed);
  mem_free(&prms.stats);
}
END_TEST

START_TEST(test2) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  ck_assert_ptr_nonnull(prms.stats.field);
  ck_assert_ptr_nonnull(prms.stats.next);
  mem_free(&prms.stats);
}
END_TEST

START_TEST(test3) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  spawn(&prms);
  prms.state = SHIFTING;
  fsm(&prms);
  ck_assert_int_eq(BRICKSTART_Y + 1, prms.brick.y);
  mem_free(&prms.stats);
}
END_TEST

START_TEST(test4) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  for (int i = 0; i < 7; i++) {
    generate_brick(i, &prms);
  }
  rotate_brick(&prms);
  rotate_backwards(&prms);
  fsm(&prms);
  ck_assert_int_eq(MOVING, prms.state);
  mem_free(&prms.stats);
}
END_TEST

START_TEST(test5) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  for (int j = 0; j < 10; j++) prms.stats.field[0][j] = 1;
  ck_assert_int_eq(1, check_game_over(&prms));
  mem_free(&prms.stats);
}
END_TEST

START_TEST(test6) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  spawn_brick(&prms);
  ck_assert_int_eq(-1, prms.brick.y);
  ck_assert_int_eq(3, prms.brick.x);
  mem_free(&prms.stats);
}

START_TEST(test7) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  prms.stats.pause = PAUSED;
  prms.state = START;
  fsm(&prms);
  ck_assert_int_eq(PAUSED, prms.stats.pause);
  mem_free(&prms.stats);
}

START_TEST(test8) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  spawn(&prms);
  prms.signal = Down;
  fsm(&prms);
  ck_assert_int_eq(ATTACHING, prms.state);
  fsm(&prms);
  fsm(&prms);
  prms.signal = Terminate;
  fsm(&prms);
  ck_assert_int_eq(EXIT_STATE, prms.state);
  mem_free(&prms.stats);
}

START_TEST(test9) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  spawn_brick(&prms);
  moveright(&prms);
  ck_assert_int_eq(BRICKSTART_X + 1, prms.brick.x);
  mem_free(&prms.stats);
}

START_TEST(test10) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  spawn_brick(&prms);
  moveleft(&prms);
  ck_assert_int_eq(BRICKSTART_X - 1, prms.brick.x);
  mem_free(&prms.stats);
}

START_TEST(test11) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  spawn(&prms);
  fsm(&prms);
  fsm(&prms);
  prms.signal = Up;
  userInput(prms.signal, false);
  prms.ticks = 300000;
  fsm(&prms);
  ck_assert_int_eq(SHIFTING, prms.state);
  mem_free(&prms.stats);
}

START_TEST(test12) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  prms.state = PAUSE;
  prms.signal = Start;
  fsm(&prms);
  ck_assert_int_eq(PAUSE, prms.state);
  mem_free(&prms.stats);
}

START_TEST(test13) {
  Params_t prms = {.state = PAUSE};
  mem_alloc(&prms);
  prms.state = GAMEOVER;
  fsm(&prms);
  ck_assert_int_eq(START, prms.state);
  ck_assert_int_eq(GAMELOST, prms.stats.pause);
  mem_free(&prms.stats);
}

START_TEST(test14) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  spawn_brick(&prms);
  shifting(&prms);
  ck_assert_int_eq(BRICKSTART_Y + 1, prms.brick.y);
  mem_free(&prms.stats);
}

START_TEST(test15) {
  Params_t prms = {.state = PAUSE};
  mem_alloc(&prms);
  prms.state = ATTACHING;
  fsm(&prms);
  ck_assert_int_eq(SPAWN, prms.state);
  mem_free(&prms.stats);
}

START_TEST(test16) {
  Params_t prms = {.state = PAUSE};
  prms.signal = Start;
  pause(&prms);
  for (int j = 0; j < 10; j++) prms.stats.field[19][j] = 1;
  remove_line(&prms);
  ck_assert_int_eq(100, prms.stats.score);
  mem_free(&prms.stats);
}

START_TEST(test17) {
  Params_t prms = {.state = PAUSE};
  prms.state = START;
  prms.signal = Terminate;
  mem_alloc(&prms);
  fsm(&prms);
  ck_assert_int_eq(EXIT_STATE, prms.state);
  mem_free(&prms.stats);
  ck_assert_ptr_null(prms.stats.field);
  ck_assert_ptr_null(prms.stats.next);
}

START_TEST(test18) {
  Params_t prms = {.state = PAUSE};
  prms.state = START;
  mem_alloc(&prms);
  processSignal(ENTER_KEY);
  fsm(&prms);
  fsm(&prms);
  ck_assert_int_eq(MOVING, prms.state);
  ck_assert_int_eq(BRICKSTART_Y, prms.brick.y);
  ck_assert_int_eq(BRICKSTART_X, prms.brick.x);
  mem_free(&prms.stats);
}

START_TEST(test19) {
  Params_t prms = {.state = PAUSE};
  prms.state = START;
  mem_alloc(&prms);
  processSignal('a');
  fsm(&prms);
  fsm(&prms);
  processSignal(' ');
  fsm(&prms);
  ck_assert_int_eq(MOVING, prms.state);
  mem_free(&prms.stats);
}

START_TEST(test20) {
  Params_t* prms = NULL;
  prms = get_params();
  ck_assert_ptr_nonnull(prms);
}

START_TEST(test21) {
  Params_t prms = {.state = PAUSE};
  prms.state = START;
  prms.signal = Start;
  mem_alloc(&prms);
  fsm(&prms);
  fsm(&prms);
  processSignal(KEY_RIGHT);
  fsm(&prms);
  ck_assert_int_eq(MOVING, prms.state);
  mem_free(&prms.stats);
}

START_TEST(test22) {
  Params_t prms = {.state = PAUSE};
  prms.state = START;
  prms.signal = Start;
  mem_alloc(&prms);
  fsm(&prms);
  fsm(&prms);
  processSignal(KEY_LEFT);
  fsm(&prms);
  ck_assert_int_eq(MOVING, prms.state);
  mem_free(&prms.stats);
}

START_TEST(test23) {
  Params_t prms = {.state = PAUSE};
  prms.state = START;
  prms.signal = Start;
  mem_alloc(&prms);
  fsm(&prms);
  fsm(&prms);
  prms.signal = Pause;
  fsm(&prms);
  ck_assert_int_eq(PAUSE, prms.state);
  ck_assert_int_eq(PAUSED, prms.stats.pause);
  mem_free(&prms.stats);
}

START_TEST(test24) {
  Params_t prms = {.state = PAUSE};
  prms.state = START;
  prms.signal = Start;
  mem_alloc(&prms);
  fsm(&prms);
  prms.state = PAUSE;
  prms.signal = Pause;
  fsm(&prms);
  ck_assert_int_eq(MOVING, prms.state);
  ck_assert_int_eq(PLAYING, prms.stats.pause);
  prms.stats.pause = PLAYING;
  prms.state = PAUSE;
  prms.signal = Terminate;
  fsm(&prms);
  ck_assert_int_eq(EXIT_STATE, prms.state);
  mem_free(&prms.stats);
}

int main() {
  int result;
  Suite* suite = suite_create("tetris_test");
  TCase* tcase = tcase_create("tetris_test_case");
  SRunner* srunner = srunner_create(suite);

  suite_add_tcase(suite, tcase);

  tcase_add_test(tcase, test0);
  tcase_add_test(tcase, test1);
  tcase_add_test(tcase, test2);
  tcase_add_test(tcase, test3);
  tcase_add_test(tcase, test4);
  tcase_add_test(tcase, test5);
  tcase_add_test(tcase, test6);
  tcase_add_test(tcase, test7);
  tcase_add_test(tcase, test8);
  tcase_add_test(tcase, test9);
  tcase_add_test(tcase, test10);
  tcase_add_test(tcase, test11);
  tcase_add_test(tcase, test12);
  tcase_add_test(tcase, test13);
  tcase_add_test(tcase, test14);
  tcase_add_test(tcase, test15);
  tcase_add_test(tcase, test16);
  tcase_add_test(tcase, test17);
  tcase_add_test(tcase, test18);
  tcase_add_test(tcase, test19);
  tcase_add_test(tcase, test20);
  tcase_add_test(tcase, test21);
  tcase_add_test(tcase, test22);
  tcase_add_test(tcase, test23);
  tcase_add_test(tcase, test24);

  srunner_set_fork_status(srunner, CK_NOFORK);
  srunner_run_all(srunner, CK_NORMAL);
  result = srunner_ntests_failed(srunner);
  srunner_free(srunner);
  return result == 0 ? 0 : 1;
}
