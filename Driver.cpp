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
#include <getopt.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <array>
#include "Game.h"
#include "gui.h"

static bool valid_command(char cmd);

using namespace std;

int main(int argc, char * argv[]) {

  int board_size = 0;
  
  static struct option longopts[] = {
    {"size",   required_argument,   nullptr, 's'},
    {"help",   no_argument,         nullptr, 'h'}
  };
  
  opterr = false;
  
  int idx = 0;
  int c;
  
  while ((c = getopt_long(argc, argv, "s:h", longopts, &idx)) != -1) {
    switch (c) {
      case 's': {
        board_size = atoi(optarg);
        break;
      }
      case 'h': {
        cout << "Usage...\n"
        << "./<executable> -s/--size <size or board>\n" << flush;
        exit(0);
        break;
      }
      default: {
        cout << "Error! Unknown command! Use -h/--help for usage\n" << flush;
        exit(1);
      }
    }
  }

  if (board_size == 0) {
    board_size = 4;
  }
  
  // Initialize rand with time
  srand((unsigned int)time(nullptr));
  
  Game Game_board((unsigned int)board_size);

  char command = ' ';
  bool up_good = true;
  bool down_good = true;
  bool left_good = true;
  bool right_good = true;
  bool made_move = false;
  bool quit = false;;

  Game_board.add_tile();
  //Game_board.print_board();

  gui game_gui(800, 800, board_size);
  SDL_Event e;

  game_gui.render(Game_board.get_board());

  while (!Game_board.is_full()
         || (up_good || down_good || left_good || right_good)) {
    while(SDL_PollEvent( &e ) != 0) {
      //User requests quit
      if(e.type == SDL_QUIT) {
        quit = true;
      }
      //User presses a key
      else if(e.type == SDL_KEYDOWN) {
        //Select surfaces based on key press
        switch(e.key.keysym.sym) {
          case SDLK_UP: {
            if (up_good) {
              Game_board.move_up();
              Game_board.add_tile();
              //Game_board.print_board();
              made_move = true;
            }
            break;
          }
          case SDLK_DOWN: {
            if (down_good) {
              Game_board.move_down();
              Game_board.add_tile();
              //Game_board.print_board();
              made_move = true;
            }
            break;
          }
          case SDLK_LEFT: {
            if (left_good) {
              Game_board.move_left();
              Game_board.add_tile();
              //Game_board.print_board();
              made_move = true;
            }
            break;
          }
          case SDLK_RIGHT: {
            if (right_good) {
              Game_board.move_right();
              Game_board.add_tile();
              //Game_board.print_board();
              made_move = true;
            }
            break;
          }
          case SDLK_q: {
            cout << "GoodBye!\nYour Score: "
                << Game_board.get_score() << "\n";
            return 0;
            break;
          }
          case SDLK_ESCAPE: {
            cout << "GoodBye!\nYour Score: "
                << Game_board.get_score() << "\n";
            return 0;
            break;
          }
          default: {
          //  r.x = SCREEN_WIDTH/2  - s_width/2;
          //  r.y = SCREEN_HEIGHT/2 - s_height/2;
            break;
          }
        }
      }
    }
      if (made_move) {
        command = ' ';
        up_good = Game_board.check_up_moves();
        down_good = Game_board.check_down_moves();
        left_good = Game_board.check_left_moves();
        right_good = Game_board.check_right_moves();
        made_move = false;
        game_gui.render(Game_board.get_board());
      }
      if (quit) {
        break;
      }
      SDL_Delay(100); 
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
