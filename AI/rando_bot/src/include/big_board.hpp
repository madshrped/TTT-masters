#pragma once
#include "globals.hpp"
#include "sub_board.hpp"
#include <vector>

class big_board {
private:
  std::vector<sub_board> sub_boards;
  short current;
  int winning_set;
  piece turn;
  piece winner;
  bool draw;

public:
  big_board();
  big_board(std::vector<sub_board> set_board, piece set_turn, short cur);
  big_board(raw_boardstate boardstate);

  std::vector<sub_board> get_sub_boards() { return sub_boards; };

  short get_current() { return current; };

  int get_winning_set() {return winning_set; };

  piece get_winner() { return winner; };

  bool get_draw() { return draw; };

  void set_winner(piece _winner) { winner = _winner; };

  piece get_turn() { return turn; };
  void set_turn(piece t) { turn = t; };

  raw_boardstate get_raw_boardstate();
  std::vector<move> get_allowed_moves();
  void place_piece(int sub_board_index, int spot);
  void check_draws();
  void check_wins();
  bool is_valid_move(move _move);
  void print();

  void next_turn();
};

