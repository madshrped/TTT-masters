#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <atomic>

#include "big_board.hpp"
#include "sub_board.hpp"
#include "globals.hpp"

extern "C" move rando_bot(raw_boardstate raw_boardstate, std::atomic<int> *time_left);
