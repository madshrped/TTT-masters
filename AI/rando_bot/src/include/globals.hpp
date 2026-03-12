#pragma once
#include "raylib.h"
#include <vector>
#include <fstream>
#include <iostream>

extern Color BG_COLOR;

extern std::vector<std::vector<int>> WINNING_SETS;

float dist(Vector2 v1, Vector2 v2);

void impose_line_n(std::ifstream *file, std::string *line, int n);

enum piece{
    CROSS = 1,
    EMPTY = 0,
    DOT = -1
};

struct move{
    int sub; 
    int spot;
};

struct raw_boardstate {
    std::vector<std::vector<piece>> board = std::vector<std::vector<piece>>(9, std::vector<piece>(9, piece::EMPTY));;
    piece turn; 
    short current; 
};