//
//  Board_test_1.cpp
//  2048_game
//
//  Created by Nicholas Metcalf on 7/11/15.
//  Copyright © 2015 Nicholas Metcalf. All rights reserved.
//

#include <stdlib.h>
#include "Game.h"
#include <cassert>
#include <iostream>


static void clear_board(Game &b);

const int NUM_TEST_BOARDS = 5;

int main(int argc, char* argv[]) {
#ifdef DEBUG
  std::cout << "This is a debug run!\n";
#endif
  srand((unsigned int)time(nullptr));
  Game test_board_0(0);
  Game test_board_1(1);
  Game test_board_2(2);
  Game test_board_3(3);
  Game test_board_4(4);
  
  Game *test_boards[] = {&test_board_0, &test_board_1,
                         &test_board_2, &test_board_3,
                         &test_board_4};
  // Tests for correct behavior from Board constructor
  for (int i = 0; i < NUM_TEST_BOARDS; ++i) {
    assert(test_boards[i]->all_tiles.size() == i*i);
    assert(test_boards[i]->size == i);
    assert(test_boards[i]->free.size() == i*i);
    for (int j = 0; j < i*i; ++j) {
      assert(test_boards[i]->board_rows[(j / i)][(j % i)]->val == 0);
      assert(test_boards[i]->board_cols[(j % i)][(j / i)]->val == 0);
      assert(test_boards[i]->free.find(j)->second.val == 0);
      assert(test_boards[i]->free.find(j)->second.num == j);
    }
  }
  
//  int i = 15;
  test_board_4.print_board();
  while (!test_board_4.free.empty()) {
    test_board_4.add_tile();
//    test_board_4.print_board();
    test_board_4.move_up();
//    test_board_4.print_board();
  }
//  test_board_4.board_rows[0][0]->val = 6;
//  test_board_4.largest_nums[0] = 1;
  test_board_4.print_board();
 
  clear_board(test_board_4);
  
  test_board_4.print_board();
  while (!test_board_4.free.empty()) {
    test_board_4.add_tile();
//    test_board_4.print_board();
    test_board_4.move_down();
//    test_board_4.print_board();
  }
  test_board_4.print_board();

  clear_board(test_board_4);
  
  test_board_4.print_board();
  while (!test_board_4.free.empty()) {
    test_board_4.add_tile();
//    test_board_4.print_board();
    test_board_4.move_right();
//    test_board_4.print_board();
  }
  test_board_4.print_board();

  clear_board(test_board_4);
  
  test_board_4.print_board();
  while (!test_board_4.free.empty()) {
    test_board_4.add_tile();
//    test_board_4.print_board();
    test_board_4.move_left();
//    test_board_4.print_board();
  }
  test_board_4.print_board();

  return 0;
}

static void clear_board(Game &b) {
  Game new_board(b.size);
  b = new_board;
}

