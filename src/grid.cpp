
#include "raylib.h"
#include "raymath.h"
#include "sub_board.hpp"
#include "big_board.hpp"
#include "globals.hpp"
#include "grid.hpp"

void grid::calc_global_grid_pols(){
    for(int i = 0; i<4; i++){
        global_grid_pols[i] = {rel_to_global(rel_grid_pols[i].start), rel_to_global(rel_grid_pols[i].end)};
    }
};

void grid::calc_global_grid_spots(){
    for (int i = 0; i<9; i++){
        global_grid_spots[i] = rel_to_global(rel_grid_spots[i]);
    }
};
        
void grid::setup(){
    rel_grid_spots.resize(9);
    global_grid_spots.resize(9);
    rel_grid_pols.resize(4);
    global_grid_pols.resize(4);

    rel_grid_spots[0] = Vector2{1.f/6.f - buff, 1.f/6.f - buff};
    rel_grid_spots[1] = Vector2{1.f/2.f, 1.f/6.f - buff};
    rel_grid_spots[2] = Vector2{5.f/6.f + buff, 1.f/6.f - buff};
    rel_grid_spots[3] = Vector2{1.f/6.f - buff, 1.f/2.f};
    rel_grid_spots[4] = Vector2{1.f/2.f, 1.f/2.f};
    rel_grid_spots[5] = Vector2{5.f/6.f + buff, 1.f/2.f};
    rel_grid_spots[6] = Vector2{1.f/6.f - buff, 5.f/6.f + buff};
    rel_grid_spots[7] = Vector2{1.f/2.f, 5.f/6.f + buff};
    rel_grid_spots[8] = Vector2{5.f/6.f + buff, 5.f/6.f + buff};

    rel_grid_pols[0] = pole{Vector2{1.f/3.f - buff/2, 0.f}, Vector2{1.f/3.f - buff/2, 1.f}};
    rel_grid_pols[1] = pole{Vector2{2.f/3.f + buff/2, 0.f}, Vector2{2.f/3.f + buff/2, 1.f}};
    rel_grid_pols[2] = pole{Vector2{0.f, 1.f/3.f - buff/2}, Vector2{1.f, 1.f/3.f - buff/2}};
    rel_grid_pols[3] = pole{Vector2{0.f, 2.f/3.f + buff/2}, Vector2{1.f, 2.f/3.f + buff/2}};

    global_pole_width = rel_pole_width*_area.width;
    global_position_center = global_position + Vector2{_area.width/2.f, _area.height/2.f};

    calc_global_grid_pols();
    calc_global_grid_spots();
};


grid::grid(){
    _area.width = GetScreenWidth();
    _area.height = GetScreenHeight();
    global_position = {0, 0};
    rel_pole_width = 0.05f;
    buff = 0.0125f;  
    global_pole_width = rel_pole_width * _area.width;
    color = {133, 125, 103, 255};
    setup();
};

grid::grid(Vector2 position, float width, float height, Color grid_color, float relative_pole_width) {
    _area.width = width;
    _area.height = height;
    global_position = position;
    rel_pole_width = relative_pole_width;
    buff = relative_pole_width/4.f;  
    global_pole_width = rel_pole_width * _area.width;
    color = grid_color;
    setup();
};

//methods
void grid::draw_grid(){
    for (int i = 0; i < 4; i++) {
        DrawLineEx(global_grid_pols[i].start, global_grid_pols[i].end, global_pole_width, color);
    }
}

void grid::draw_spots(){
    for (int i = 0; i<9; i++){
        DrawCircleV(global_grid_spots[i], 20, RED);
    }
}

void grid::draw_X(int index){
    float width = ((_area.width/3.f - global_pole_width)/2.f)*0.85f;
    pole diag1 = {{global_grid_spots[index].x - width, global_grid_spots[index].y - width}, {global_grid_spots[index].x + width, global_grid_spots[index].y + width}};
    pole diag2 = {{global_grid_spots[index].x - width, global_grid_spots[index].y + width}, {global_grid_spots[index].x + width, global_grid_spots[index].y - width}};

    float thickness = global_pole_width;
    Color X_color  = RED;

    DrawLineEx(diag1.start, diag1.end, thickness, X_color);
    DrawLineEx(diag2.start, diag2.end, thickness, X_color);
};

void grid::draw_tie(){
    float width = (_area.width/2.f)*0.85f;
    pole diag1 = {{global_position_center.x - width, global_position_center.y - width}, {global_position_center.x + width, global_position_center.y - width}};
    pole diag2 = {{global_position_center.x, global_position_center.y - width}, {global_position_center.x, global_position_center.y + width}};

    float thickness = global_pole_width*2;
    Color T_color  = LIME;

    DrawLineEx(diag1.start, diag1.end, thickness, T_color);
    DrawLineEx(diag2.start, diag2.end, thickness, T_color);
};

void grid::draw_O(int index){
    float width = ((_area.width/3.f - global_pole_width)/2.f)*0.85f;

    float thickness = global_pole_width;

    Color O_color  = BLUE;

    DrawRing(global_grid_spots[index], width - thickness, width, 0, 360, 64, O_color);
};


void grid::draw_winner(piece winner){
    float width = (_area.width/2.f)*0.85f;

    if (winner == CROSS){
        pole diag1 = {{global_position_center.x - width, global_position_center.y - width}, {global_position_center.x + width, global_position_center.y + width}};
        pole diag2 = {{global_position_center.x - width, global_position_center.y + width}, {global_position_center.x + width, global_position_center.y - width}};

        float thickness = global_pole_width*2;
        Color X_color  = RED;

        DrawLineEx(diag1.start, diag1.end, thickness, X_color);
        DrawLineEx(diag2.start, diag2.end, thickness, X_color);

    } else if (winner == DOT){
        float thickness = global_pole_width*2;

        Color O_color  = BLUE;

        DrawRing(global_position_center, width-thickness, width, 0, 360, 64, O_color);
    };
};

Vector2 grid::rel_to_global(Vector2 rel_position) {
    Vector2 Global = {global_position.x + rel_position.x * _area.width, global_position.y + rel_position.y * _area.height};
    return Global;
}

Vector2 grid::global_to_rel(Vector2 global_position){
    Vector2 Rel = {(global_position.x-this->global_position.x)/_area.width, (global_position.y-this->global_position.y)/_area.height};
    return Rel;
}
