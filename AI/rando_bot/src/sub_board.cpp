
#include "sub_board.hpp"
#include "globals.hpp"
#include "iostream"
#include <vector>

sub_board::sub_board() {
  spots.resize(9);
  for (int i = 0; i < 9; i++) {
    spots[i] = EMPTY;
  };
  active = true;
  draw = false;
};

sub_board::sub_board(std::vector<piece> set_board, bool active) {
  spots.resize(9);
  for (int i = 0; i < 9; i++) {
    spots[i] = set_board[i];
  };
  this->active = active;
  draw = false;
};

void sub_board::print() {
  std::cout << "Status: ";
  if (active) {
    std::cout << "Active :)" << std::endl;
  } else {
    std::cout << "Not Active :(" << std::endl;
  };
  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0 && i != 0) {
      std::cout << '\n';
    };
    std::cout << spots[i];
  }
  std::cout << std::endl;
}

void sub_board::place_piece(int spot, piece p) {
  if (spot < 0 || spot > 8) {
    std::cout << "Invalid spot index: " << spot << std::endl;
    return;
  }
  if (spots[spot] != EMPTY) {
    std::cout << "Spot already occupied!" << std::endl;
    return;
  }
  spots[spot] = p;
};

void sub_board::check_draw(){
  for (int spot = 0; spot < 9; spot++) {
    if(spots[spot] == EMPTY){
      return;
    }
  }
  if(winner == EMPTY){
    draw = true;
  }
}
void sub_board::check_wins() {
  for (int i = 0; i < 8; i++) {
    int sum = 0;
    for (int n = 0; n < 3; n++) {
      sum += spots[WINNING_SETS[i][n]];
    };

    if (abs(sum) == 3) {
      winner = static_cast<piece>(sum / 3.f);
      return;
    };
  };
};
