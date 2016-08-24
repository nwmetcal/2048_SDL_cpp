//
//  Game.cpp
//
// File containing implementation of Game class
//
//  Created by Nicholas Metcalf on 7/11/15.
//

#include "Game.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <math.h>


static void swap_tiles(Tile *lhs, Tile *rhs);

Game::Game(int size_in) : oss(), size(size_in), score(0) {
  this->all_tiles.resize((int)pow(this->size,2));
  this->board_rows.resize(this->size);
  this->board_cols.resize(this->size);
  this->filled_rows.resize(this->size);
  this->filled_cols.resize(this->size);
  this->largest_nums.resize(this->size);
  
  for (int i = 0; i < this->size; ++i) {
    this->board_rows[i].resize(this->size);
    this->board_cols[i].resize(this->size);
    this->filled_rows[i] = 0;
    this->filled_cols[i] = 0;
    this->largest_nums[i] = 0;
  }
  for (int i = 0; i < (this->size * this->size); ++i) {
    this->all_tiles[i].num = i;
    this->board_rows[(i / this->size)][(i % this->size)] = &this->all_tiles[i];
    this->board_cols[(i % this->size)][(i / this->size)] = &this->all_tiles[i];
    this->free.emplace(i, this->all_tiles[i]);
  }
}

void Game::add_tile() {
  for (int i = 0; i < (int)log2(this->size-1); ++i) {
    double val_prob = (double)(rand())/(double)(RAND_MAX);
    double pos_prob = (double)(rand())/(double)(RAND_MAX);
    
    int id = (int)(pos_prob*(double)(this->free.size()));
    auto position = std::next(std::begin(this->free), id);
    
    ++filled_cols[position->second.num % this->size];
    ++filled_rows[position->second.num / this->size];
    
    if (val_prob < .9) {
      position->second.val = 1;
    }
    else {
      position->second.val = 2;
      this->score += (int)(1<<position->second.val);
    }
    this->free.erase(position);
  }
}

void Game::print_board() {
  this->get_largest();
  std::string header = "+";
  for (int i = 0; i < this->size; ++i) {
    for (int k = 0; k < (this->largest_nums[i] + 3); ++k) {
      header.append("-");
    }
    header.append("+");
  }
  this->oss << "Score: " << this->score << "\n";
  for (int j = 0; j < this->size; ++j) {
    this->oss << header << "\n";
    this->oss << "| ";
    for (int i = 0; i < this->size; ++i) {
      for (int k = 0;
           k < (this->largest_nums[i]
                - (int)log10(1<<this->board_rows[j][i]->val));
           ++k) {
        this->oss << " ";
      }
      if (this->board_rows[j][i]->val) {
        this->oss << (1<<this->board_rows[j][i]->val) << " | ";
      }
      else {
        this->oss << "  | ";
      }
    }
    this->oss << "\n";
  }
  this->oss << header << "\n";
  std::cout << this->oss.str();
  this->oss.str("");
}

void Game::move_up() {
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_cols[i]) {
      std::vector<int> removed;
      int offset = 1;
      for (int j = 0; j < (this->size - offset); ++j) {
        offset = 1;
        while ((j < (this->size - offset))
               && this->board_cols[i][j]->val) {
          if (this->board_cols[i][j]->val == this->board_cols[i][j + offset]->val) {
            ++this->board_cols[i][j]->val;
            this->score += (int)(1<<this->board_cols[i][j]->val);
            this->board_cols[i][j + offset]->val = 0;
            if ((int)log10(1<<this->board_cols[i][j]->val) > this->largest_nums[i]) {
              this->largest_nums[i] = (int)log10(1<<this->board_cols[i][j]->val);
            }
            --this->filled_rows[j + offset];
            --this->filled_cols[i];
            j += offset;
            break;
          }
          if (this->board_cols[i][j + offset]->val) {
            break;
          }
          ++offset;
        }
      }
      //move all cells to topmost spots
      int top_free = (this->size + 1);
      int top_taken = (this->size + 1);
    
      while (top_taken > this->size) {
        int j = 0;
        while ((j < this->size) && ((top_free > this->size) || (top_taken > this->size))) {
          if (j && this->board_cols[i][j]->val
              && !this->board_cols[i][j-1]->val) {
            top_taken = j;
          }
          if (!this->board_cols[i][j]->val && (j < top_free)) {
            top_free = j;
          }
          ++j;
        }
        if (top_taken < this->size) {
          swap_tiles(this->board_cols[i][top_free], this->board_cols[i][top_taken]);
          ++filled_rows[top_free];
          --filled_rows[top_taken];
          removed.push_back(this->board_cols[i][top_free]->num);
          top_free = top_taken;
          top_taken = (this->size + 1);
        }
        else {
          top_taken = -1;
        }
      }
      
      //add new empty tiles to this->free
      int j = this->size-1;
      while (!this->board_cols[i][j]->val) {
        auto pos = this->free.find(this->board_cols[i][j]->num);
        if (pos == this->free.end()) {
          this->free.emplace(this->board_cols[i][j]->num,
                             this->all_tiles[i+(j*this->size)]);
        }
        --j;
      }
      for (auto it = removed.begin(); it != removed.end(); ++it) {
        auto pos = this->free.find(*it);
        if (pos != this->free.end()) {
          this->free.erase(pos);
        }
      }
    }
  }
}

void Game::move_down() {
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_cols[i]) {
      std::vector<int> removed;
      int offset = 1;
      for (int j = (this->size - 1); !((j - offset) < 0); --j) {
        offset = 1;
        while (!((j-offset) < 0) && this->board_cols[i][j]->val) {
          if (this->board_cols[i][j]->val == this->board_cols[i][j - offset]->val) {
            ++this->board_cols[i][j]->val;
            this->score += (int)(1<<this->board_cols[i][j]->val);
            this->board_cols[i][j - offset]->val = 0;
            if ((int)log10(1<<this->board_cols[i][j]->val) > this->largest_nums[i]) {
              this->largest_nums[i] = (int)log10(1<<this->board_cols[i][j]->val);
            }
            --this->filled_rows[j - offset];
            --this->filled_cols[i];
            j -= offset;
            break;
          }
          if (this->board_cols[i][j - offset]->val) {
            break;
          }
          ++offset;
        }
      }
      //move all cells to right-most spots
      int top_free = -1;
      int top_taken = (this->size + 1);
      
      while (top_taken > this->size) {
        int j = (this->size - 1);
        while (!(j < 0) && ((top_free < 0) || (top_taken > this->size))) {
          if ((j < (this->size - 1)) && this->board_cols[i][j]->val
              && !this->board_cols[i][j+1]->val) {
            top_taken = j;
          }
          if (!this->board_cols[i][j]->val && (j > top_free)) {
            top_free = j;
          }
          --j;
        }
        if (top_taken < this->size) {
          swap_tiles(this->board_cols[i][top_free], this->board_cols[i][top_taken]);
          ++filled_rows[top_free];
          --filled_rows[top_taken];
          removed.push_back(this->board_cols[i][top_free]->num);
          top_free = top_taken;
          top_taken = (this->size + 1);
        }
        else {
          top_taken = -1;
        }
      }
      
      //add new empty tiles to this->free
      int j = 0;
      while (!this->board_cols[i][j]->val) {
        auto pos = this->free.find(this->board_cols[i][j]->num);
        if (pos == this->free.end()) {
          this->free.emplace(this->board_cols[i][j]->num,
                             this->all_tiles[i+(j*this->size)]);
        }
        ++j;
      }
      for (auto it = removed.begin(); it != removed.end(); ++it) {
        auto pos = this->free.find(*it);
        if (pos != this->free.end()) {
          this->free.erase(pos);
        }
      }
    }
  }
}

void Game::move_left() {
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_rows[i]) {
      std::vector<int> removed;
      int offset = 1;
      for (int j = 0; j < (this->size - offset); ++j) {
        offset = 1;
        while ((j < (this->size - offset))
               && this->board_rows[i][j]->val) {
          if (this->board_rows[i][j]->val == this->board_rows[i][j + offset]->val) {
            ++this->board_rows[i][j]->val;
            this->score += (int)(1<<this->board_rows[i][j]->val);
            this->board_rows[i][j + offset]->val = 0;
            if ((int)log10(1<<this->board_rows[i][j]->val) > this->largest_nums[j]) {
              this->largest_nums[j] = (int)log10(1<<this->board_rows[i][j]->val);
            }
            --this->filled_cols[j + offset];
            --this->filled_rows[i];
            j += offset;
            break;
          }
          if (this->board_rows[i][j + offset]->val) {
            break;
          }
          ++offset;
        }
      }
      //move all cells to topmost spots
      int top_free = (this->size + 1);
      int top_taken = (this->size + 1);
      
      while (top_taken > this->size) {
        int j = 0;
        while ((j < this->size) && ((top_free > this->size) || (top_taken > this->size))) {
          if (j && this->board_rows[i][j]->val
              && !this->board_rows[i][j-1]->val) {
            top_taken = j;
          }
          if (!this->board_rows[i][j]->val && (j < top_free)) {
            top_free = j;
          }
          ++j;
        }
        if (top_taken < this->size) {
          swap_tiles(this->board_rows[i][top_free], this->board_rows[i][top_taken]);
          ++filled_cols[top_free];
          --filled_cols[top_taken];
          removed.push_back(this->board_rows[i][top_free]->num);
          top_free = top_taken;
          top_taken = (this->size + 1);
        }
        else {
          top_taken = -1;
        }
      }
      
      //add new empty tiles to this->free
      int j = this->size-1;
      while (!this->board_rows[i][j]->val) {
        auto pos = this->free.find(this->board_rows[i][j]->num);
        if (pos == this->free.end()) {
          this->free.emplace(this->board_rows[i][j]->num,
                             this->all_tiles[j+(i*this->size)]);
        }
        --j;
      }
      for (auto it = removed.begin(); it != removed.end(); ++it) {
        auto pos = this->free.find(*it);
        if (pos != this->free.end()) {
          this->free.erase(pos);
        }
      }
    }
  }
}

void Game::move_right() {
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_rows[i]) {
      std::vector<int> removed;
      int offset = 1;
      for (int j = (this->size - 1); !((j - offset) < 0); --j) {
        offset = 1;
        while (!((j-offset) < 0) && this->board_rows[i][j]->val) {
          if (this->board_rows[i][j]->val == this->board_rows[i][j - offset]->val) {
            ++this->board_rows[i][j]->val;
            this->score += (int)(1<<this->board_rows[i][j]->val);
            this->board_rows[i][j - offset]->val = 0;
            --this->filled_cols[j - offset];
            --this->filled_rows[i];
            if ((int)log10(1<<this->board_rows[i][j]->val) > this->largest_nums[j]) {
              this->largest_nums[j] = (int)log10(1<<this->board_rows[i][j]->val);
            }
            j -= offset;
            break;
          }
          if (this->board_rows[i][j - offset]->val) {
            break;
          }
          ++offset;
        }
      }
      //move all cells to bottom-most spots
      int top_free = -1;
      int top_taken = (this->size + 1);
      
      while (top_taken > this->size) {
        int j = (this->size - 1);
        while (!(j < 0) && ((top_free < 0) || (top_taken > this->size))) {
          if ((j < (this->size - 1)) && this->board_rows[i][j]->val
              && !this->board_rows[i][j+1]->val) {
            top_taken = j;
          }
          if (!this->board_rows[i][j]->val && (j > top_free)) {
            top_free = j;
          }
          --j;
        }
        if (top_taken < this->size) {
          swap_tiles(this->board_rows[i][top_free], this->board_rows[i][top_taken]);
          ++filled_cols[top_free];
          --filled_cols[top_taken];
          removed.push_back(this->board_rows[i][top_free]->num);
          top_free = top_taken;
          top_taken = (this->size + 1);
        }
        else {
          top_taken = -1;
        }
      }
      
      //add new empty tiles to this->free
      int j = 0;
      while (!this->board_rows[i][j]->val) {
        auto pos = this->free.find(this->board_rows[i][j]->num);
        if (pos == this->free.end()) {
          this->free.emplace(this->board_rows[i][j]->num,
                             this->all_tiles[j+(i*this->size)]);
        }
        ++j;
      }
      for (auto it = removed.begin(); it != removed.end(); ++it) {
        auto pos = this->free.find(*it);
        if (pos != this->free.end()) {
          this->free.erase(pos);
        }
      }
    }
  }
}

bool Game::check_up_moves() const{
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_cols[i]) {
      for (int j = 0; j < (this->size-1); ++j) {
        if ((this->board_cols[i][j]->val
             && (this->board_cols[i][j]->val == this->board_cols[i][j+1]->val))
            || (!this->board_cols[i][j]->val
                && this->board_cols[i][j+1]->val)) {
              return true;
            }
      }
    }
  }
  return false;
}

bool Game::check_down_moves() const{
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_cols[i]) {
      for (int j = 0; j < (this->size-1); ++j) {
        if ((this->board_cols[i][j]->val
             && (this->board_cols[i][j]->val == this->board_cols[i][j+1]->val))
            || (this->board_cols[i][j]->val
                && !this->board_cols[i][j+1]->val)) {
              return true;
            }
      }
    }
  }
  return false;
}

bool Game::check_left_moves() const{
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_rows[i]) {
      for (int j = 0; j < (this->size-1); ++j) {
        if ((this->board_rows[i][j]->val
             && (this->board_rows[i][j]->val == this->board_rows[i][j+1]->val))
            || (!this->board_rows[i][j]->val
                && this->board_rows[i][j+1]->val)) {
              return true;
            }
      }
    }
  }
  return false;
}

bool Game::check_right_moves() const{
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_rows[i]) {
      for (int j = 0; j < (this->size-1); ++j) {
        if ((this->board_rows[i][j]->val
             && (this->board_rows[i][j]->val == this->board_rows[i][j+1]->val))
            || (this->board_rows[i][j]->val
                && !this->board_rows[i][j+1]->val)) {
              return true;
            }
      }
    }
  }
  return false;
}

bool Game::is_full() const{
  return this->free.empty();
}

Game& Game::operator=(Game &rhs) {
  this->all_tiles.clear();
  this->board_rows.clear();
  this->board_cols.clear();
  this->filled_rows.clear();
  this->filled_cols.clear();
  this->largest_nums.clear();
  this->free.clear();
  this->size = rhs.size;
  this->score = rhs.score;
  this->oss.str(rhs.oss.str());
  
  this->all_tiles.resize((int)pow(this->size,2));
  this->board_rows.resize(this->size);
  this->board_cols.resize(this->size);
  this->filled_rows.resize(this->size);
  this->filled_cols.resize(this->size);
  this->largest_nums.resize(this->size);
  
  for (int i = 0; i < this->size; ++i) {
    this->board_rows[i].resize(this->size);
    this->board_cols[i].resize(this->size);
    this->filled_rows[i] = rhs.filled_rows[i];
    this->filled_cols[i] = rhs.filled_cols[i];
    this->largest_nums[i] = rhs.largest_nums[i];
  }
  for (int i = 0; i < (this->size * this->size); ++i) {
    this->all_tiles[i].num = i;
    this->all_tiles[i].val = rhs.all_tiles[i].val;
    this->board_rows[(i / this->size)][(i % this->size)] = &this->all_tiles[i];
    this->board_cols[(i % this->size)][(i / this->size)] = &this->all_tiles[i];
    this->free.emplace(i, this->all_tiles[i]);
  }
  
  return *this;
}

void Game::get_largest() {
  for (int i = 0; i < this->size; ++i) {
    if (this->filled_cols[i]) {
      for (int j = 0; j < this->size; ++j) {
        if ((int)log10(1<<this->board_cols[i][j]->val) > this->largest_nums[i]) {
          this->largest_nums[i] = (int)log10(1<<this->board_cols[i][j]->val);
        }
      }
    }
  }
}

long Game::get_score() {
  return this->score;
}

static void swap_tiles(Tile *lhs, Tile *rhs) {
  Tile temp;
  temp.val = lhs->val;
  lhs->val = rhs->val;
  rhs->val = temp.val;
}

std::vector<Tile>& Game::get_board() {
  return all_tiles;
}
