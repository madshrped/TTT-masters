#pragma once
#include "raylib.h"
#include <vector>

class grid{
    private: 
        Vector2 global_position;
        Vector2 global_position_center;
        float rel_pole_width;
        float global_pole_width;

        struct area {
            float width;
            float height;
        };
        
        std::vector<Vector2> rel_grid_spots;
        std::vector<Vector2> global_grid_spots;

        area _area;
        Color color;
        float buff;

        struct pole{
            Vector2 start;
            Vector2 end;
        };

        std::vector<pole> rel_grid_pols;
        std::vector<pole> global_grid_pols;

        void calc_global_grid_pols();
        void calc_global_grid_spots();
        void setup();

    public:

    //constructers
        grid();
        grid(Vector2 position, float width, float height, Color grid_color, float relative_pole_width);

    //methods
        void draw_grid();
        void draw_spots();
        void draw_X(int index);
        void draw_O(int index);
        void draw_tie();
        void draw_winner(piece winner);

        Vector2 rel_to_global(Vector2 rel_position);
        Vector2 global_to_rel(Vector2 global_position);

        Vector2 get_global_position(){
            return global_position;
        };

        Vector2 get_global_position_center(){
            return global_position_center;
        };

        float get_rel_pole_width(){
            return rel_pole_width;
        }
        float get_global_pole_width(){
            return global_pole_width;
        };

        std::vector<Vector2> get_rel_grid_spots(){
            return rel_grid_spots;
        };

        void set_rel_grid_spot(short index, Vector2 val){
            rel_grid_spots[index] = val;
        };


        std::vector<Vector2> get_global_grid_spots(){
            return global_grid_spots;
        };

        std::vector<pole> get_rel_grid_pols(){
            return rel_grid_pols;
        };

        void set_rel_grid_pole(short index, pole val){
            rel_grid_pols[index] = val;
        };

        std::vector<pole> get_global_grid_pols(){
            return global_grid_pols;
        };

        Color get_color(){
            return color;
        };

        area get_area(){
            return _area;
        };

};
