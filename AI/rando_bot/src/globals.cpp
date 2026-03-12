#include "globals.hpp"
#include <math.h>
#include "raylib.h"
#include <vector>
#include <fstream>

void impose_line_n(std::ifstream *file, std::string *line, int n){
    for (int i = 0; i<n; i++){
        std::getline(*file, *line);
    };
};

Color BG_COLOR = {224, 211, 175, 255};

std::vector<std::vector<int>> WINNING_SETS = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6}
};

float dist(Vector2 v1, Vector2 v2){
    return sqrt(pow((v1.x-v2.x), 2.f) + pow((v1.y - v2.y), 2.f));
};