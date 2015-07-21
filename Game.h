//
//  Game.h
//  2048_game
//
//  Created by Nicholas Metcalf on 7/11/15.
//

#ifndef Game_H
#define Game_H

#include <vector>
#include <unordered_map>
#include <sstream>
#include <array>

struct Tile {
public:
  int val;
  int num;
  Tile() : val(0), num(-1) {};
};

class Game {
#ifdef DEBUG
public:
#else
private:
#endif
  std::vector<Tile> all_tiles;
  std::vector<std::vector<Tile *>> board_rows;
  std::vector<std::vector<Tile *>> board_cols;
  std::vector<int> filled_rows;
  std::vector<int> filled_cols;
  std::vector<int> largest_nums;
  std::unordered_map<int, Tile&> free;
  mutable std::stringstream oss;
  int size;
  long score;
  void get_largest();
public:
  Game(int size_in);
  void print_board();
  void move_up();
  void move_down();
  void move_left();
  void move_right();
  void add_tile();
  bool check_up_moves() const;
  bool check_down_moves() const;
  bool check_left_moves() const;
  bool check_right_moves() const;
  bool is_full() const;
  long get_score();
  Game& operator= (Game &rhs);
};

#endif /* Game_H */
