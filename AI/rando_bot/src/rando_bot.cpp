#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

#include "big_board.hpp"
#include "globals.hpp"
#include "rando_bot.hpp"

void print_move(move a) { std::cout << a.sub << ", " << a.spot << std::endl; }

move rando_bot(raw_boardstate boardstate, std::atomic<int> *time_left) {
  big_board board = big_board(boardstate);
  std::vector<move> moves = board.get_allowed_moves();

  srand(time(0));
  int pick = rand() % moves.size();
  return moves[pick];
}
