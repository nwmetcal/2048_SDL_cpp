//
//  Driver.cpp
//
//  Created by Nicholas Metcalf on 7/11/15.
//
// File containing driver for 2048 game
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Game.h"

static bool valid_command(char cmd);

using namespace std;

int main(int argc, const char * argv[]) {

  // Initialize rand with time
  srand((unsigned int)time(nullptr));
  
  Game Game_board(atoi(argv[argc-1]));

  char command = ' ';
  bool up_good = true;
  bool down_good = true;
  bool left_good = true;
  bool right_good = true;
  
  Game_board.add_tile();
  Game_board.print_board();

  while (!Game_board.is_full()
         || (up_good || down_good || left_good || right_good)) {
   
    while (!valid_command(command)) {
      cin >> command;
    }
    switch (command) {
      case 'w': {
        if (up_good) {
          Game_board.move_up();
          Game_board.add_tile();
          Game_board.print_board();
        }
        break;
      }
      case 'a': {
        if (left_good) {
          Game_board.move_left();
          Game_board.add_tile();
          Game_board.print_board();
        }
        break;
      }
      case 's': {
        if (down_good) {
          Game_board.move_down();
          Game_board.add_tile();
          Game_board.print_board();
        }
        break;
      }
      case 'd': {
        if (right_good) {
          Game_board.move_right();
          Game_board.add_tile();
          Game_board.print_board();
        }
        break;
      }
      case 'q': {
        cout << "GoodBye!\nYour Score: "
            << Game_board.get_score() << "\n";
        return 0;
        break;
      }
      default:
        break;
    }
    command = ' ';
    up_good = Game_board.check_up_moves();
    down_good = Game_board.check_down_moves();
    left_good = Game_board.check_left_moves();
    right_good = Game_board.check_right_moves();
  }
  cout << "GAME OVER :(\nYour Score: "
       << Game_board.get_score() << "\n";
  return 0;
}

static bool valid_command(char cmd) {
  switch (cmd) {
    case 'w':
    case 'a':
    case 's':
    case 'd':
    case 'q':
      return true;
      break;
      
    default:
      return false;
      break;
  }
}
