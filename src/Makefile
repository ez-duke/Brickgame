#!/bin/bash
CC=gcc -Wall -Werror -Wextra -std=c11 -g
CC2=g++ -Wall -Werror -Wextra -std=c++17 -g
SRC=brick_game/tetris/*.c
SRC2=brick_game/snake/*.cc
GUI=gui/cli/*.c
GUI2=gui/desktop/*.cc
TSRC=tests/tetris/*.c
TSRC2=tests/snake/*.cc
DIST=build
NAME=s21_tetris
NAME2=s21_snake
TNAME=$(NAME)_tests
TNAME2=$(NAME2)_tests
TGZ=brickgame.tar.gz
UNAME=$(shell uname -s)
HEADERS=common.h brick_game/tetris/*.h gui/cli/*.h tests/tetris/*.h
HEADERS2=common.h brick_game/snake/*.h gui/desktop/*.h tests/snake/*.h

ifeq ($(UNAME),Linux)
	LIBS=-lrt -lpthread -lcheck -lsubunit -lm
	LIBS2=-lrt -lpthread -lgtest -lgtest_main -lsubunit -lm
endif

ifeq ($(UNAME),Darwin)
	LIBS=-lpthread -lcheck
	LIBS2=-lpthread -lgtest -lgtest_main
endif

all: install

install:
	$(CC) $(SRC) $(GUI) -o $(NAME) -lncurses
	$(CC2) $(SRC2) $(GUI) -o $(NAME2) -lncurses

	cmake -S brick_game/tetris -B build/tetris
	cmake --build build/tetris

	cmake -S brick_game/snake -B build/snake
	cmake --build build/snake

uninstall: clean
	@rm -rf $(NAME) $(NAME2) $(TGZ) *.app

clean:
	@rm -rf $(DIST)/* *.dSYM

dvi:
	doxygen Doxyfile
ifeq ($(shell uname), Darwin)
	open ./doxygen/html/index.html
endif

dist:
	@tar -czf $(TGZ) ./*

tests: clean $(TSRC) $(SRC)
	$(CC) $(TSRC) $(SRC) gui/cli/cli_controller.c -o $(DIST)/$(TNAME) $(LIBS)
	$(CC2) $(TSRC2) $(SRC2) gui/cli/cli_controller.c -o $(DIST)/$(TNAME2) $(LIBS2)
	@$(DIST)/$(TNAME)
	@$(DIST)/$(TNAME2)

cf:
	clang-format --style=Google -i $(SRC) $(SRC2) $(TSRC) $(TSRC2) $(HEADERS) $(HEADERS2) $(GUI) $(GUI2)

check:
	clang-format --style=Google -n $(SRC) $(SRC2) $(TSRC) $(TSRC2) $(HEADERS) $(HEADERS2) $(GUI) $(GUI2)

cppc:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction $(SRC) $(TSRC) $(HEADERS)
	cppcheck --language=c++ --enable=all --suppress=missingIncludeSystem --suppress=unusedStructMember --suppress=unusedFunction $(SRC2) $(HEADERS2)
