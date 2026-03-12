#include "big_board.hpp"
#include "globals.hpp"
#include "sub_board.hpp"
#include <iostream>
#include <vector>

big_board::big_board() {
  sub_boards.resize(9);
  for (int i = 0; i < 9; i++) {
    for (int n = 0; n < 9; n++) {
      sub_boards[i].get_spots()[n] = EMPTY;
    }
  };
  turn = CROSS;
  current = -1;
  winning_set = -1;
  winner = EMPTY;
  draw = false;
};

big_board::big_board(std::vector<sub_board> set_board, piece set_turn,
                     short cur) {
  sub_boards.resize(9);
  for (int i = 0; i < 9; i++) {
    sub_boards[i] = set_board[i];
  };
  turn = set_turn;
  current = cur;
  winning_set = -1;
  winner = EMPTY;
  draw = false;
  check_wins();
};

big_board::big_board(raw_boardstate boardstate) {
  sub_boards.resize(9);
  turn = boardstate.turn;
  current = boardstate.current;
  winning_set = -1;
  winner = EMPTY;
  draw = false;

  for (int sub = 0; sub < 9; sub++) {
    sub_boards[sub] = sub_board(boardstate.board[sub],
                                true ? current == sub || current == -1 : false);
  }

  check_wins();
};

raw_boardstate big_board::get_raw_boardstate() {
  raw_boardstate boardstate;
  for (int sub = 0; sub < 9; sub++) {
    boardstate.board[sub] = sub_boards[sub].get_spots();
  }
  boardstate.turn = turn;
  boardstate.current = this->current;

  return boardstate;
};

void big_board::next_turn() {
  check_wins();
  check_draws();
  turn = static_cast<piece>(-turn);
}

void big_board::check_draws(){
  for (int sub = 0; sub < 9; sub++) {
    sub_boards[sub].check_draw();
  };
  for (int i = 0; i < WINNING_SETS.size(); i++) {
    int empties = 0;
    int sum = 0; 
    for (int n = 0; n < 3; n++) {
      sub_board cur = sub_boards[WINNING_SETS[i][n]];
      if(cur.get_winner() == EMPTY){
        empties += 1;
      }
      if(cur.get_draw()){
        sum = -1;
        break;
      }
      sum += cur.get_winner(); 
    }
    if((abs(sum) == 2 && empties == 1) || (abs(sum) == 1 && empties == 2) || empties == 3){
      return;
    }
  }
  draw = true;
}

void big_board::check_wins() {
  for (int sub = 0; sub < 9; sub++) {
    sub_boards[sub].check_wins();
  };

  for (int i = 0; i < 8; i++) {
    int sum = 0;
    for (int n = 0; n < 3; n++) {
      sum += sub_boards[WINNING_SETS[i][n]].get_winner();
    };

    if (abs(sum) == 3) {
      winner = static_cast<piece>(sum / 3.f);
      winning_set = i;
      return;
    };
  };
};

void big_board::place_piece(int sub, int spot) {
  if (!is_valid_move((move){sub, spot})) {
    std::cerr << '{' << sub << ", " << spot << '}' << " is not at valid move"
              << std::endl;
    winner = static_cast<piece>(-turn);
    return;
  }

  sub_boards[sub].place_piece(spot, turn);
  check_wins();
  check_draws();

  if (sub_boards[spot].get_winner() != EMPTY || sub_boards[spot].get_draw()) {
    current = -1;
  } else {
    current = spot;
  };

  for (int i = 0; i < 9; i++) {
    if (i == spot || current == -1) {
      sub_boards[i].set_active(true);
    } else {
      sub_boards[i].set_active(false);
    }
  }
  next_turn();
};

std::vector<move> big_board::get_allowed_moves() {
  std::vector<move> moves;

  for (int sub = 0; sub < 9; sub++) {
    for (int spot = 0; spot < 9; spot++) {
      move _move = {sub, spot};
      if (is_valid_move(_move)) {
        moves.push_back(_move);
      }
    }
  }
  return moves;
};

bool big_board::is_valid_move(move _move) {
  if (!(_move.sub >= 0 && _move.sub < 9)) {
    return false;
  }
  if (!(_move.spot >= 0 && _move.spot < 9)) {
    //std::cout << "move.spot: " << _move.spot << std::endl;
    return false;
  }
  if (_move.sub != current and current != -1) {
    //std::cout << "current: " << current << std::endl;
    return false;
  }
  if (!sub_boards[_move.sub].get_active()) {
    //std::cout << "active: " << sub_boards[_move.sub].get_active() << std::endl;
    return false;
  }
  if (sub_boards[_move.sub].get_spots()[_move.spot] != EMPTY) {
    //std::cout << "spot: " << sub_boards[_move.sub].get_spots()[_move.spot] << std::endl;
    return false;
  }
  return true;
}

void big_board::print() {
  for (int big_rows = 0; big_rows < 3; big_rows++) {
    std::cout << "-------------------------------------\n";
    for (int line = 0; line < 3; line++) {
      std::cout << "| ";
      for (int block = 0; block < 3; block++) {
        for (int spot = 0; spot < 3; spot++) {
          std::cout << ' ';
          std::cout
              << sub_boards[big_rows * 3 + block].get_spots()[line * 3 + spot];
          std::cout << ' ';
        };
        if (sub_boards[big_rows * 3 + block].get_active()) {
          std::cout << " < ";
        } else {
          std::cout << " | ";
        };
      }
      std::cout << '\n';
    }
  }
  std::cout << "-------------------------------------\n";
};
