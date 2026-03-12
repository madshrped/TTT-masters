#include "player_banner.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include "globals.hpp"
#include "raylib.h"
#include "raymath.h"

player_banner::player_banner(){
    player_name = "ANONYMOUS";
    creator_tag = "Anonymous";
    pic_path = "src/assets/no_user.png";
    width = 100;
    height = 100;
    position = {0,0};
};

player_banner::player_banner(std::string config_path){

    if (!std::filesystem::exists(config_path)) {
        std::cerr << "Config file does not exist: " << config_path << std::endl;
        player_name = "ANONYMOUS";
        creator_tag = "Anonymous";
        pic_path = "src/assets/no_user.png";
        width = 100;
        height = 100;
        position = {0,0};
        std::cout << "Using default values." << std::endl;
        std::cout << "Please create a config file at " << config_path << " with the format:\n"
                  << "creator_tag,player_name,pic_path" << std::endl;
        return;
    };
    std::ifstream config(config_path);
    if (!config) {
        std::cerr << "Failed to open: " << config_path << std::endl;
        std::cerr << "Config file does not exist: " << config_path << std::endl;
        player_name = "ANONYMOUS";
        creator_tag = "Anonymous";
        pic_path = "src/assets/no_user.png";
        width = 100;
        height = 100;
        position = {0,0};
        std::cout << "Using default values." << std::endl;
        std::cout << "Please create a config file at " << config_path << " with the format:\n"
                  << "creator_tag,player_name,pic_path" << std::endl;
        return;
    };;

    std::string line;
    impose_line_n(&config, &line, 2);
    std::cout << line << std::endl;

    std::stringstream ss(line);
    std::vector<std::string> cells;
    cells.resize(3);

    for( int i = 0; i<static_cast<int>(cells.size()); i++){
        std::getline(ss, cells[i], ',');
    };  
        
    creator_tag = cells[0];
    player_name = cells[1];
    pic_path = cells[2];

    config.close();

    if(creator_tag == ""){
        creator_tag = "Anonymous";
    };
    
    if(player_name == ""){
        player_name = "ANONYMOUS";
    }

    if (!std::filesystem::exists(pic_path) || pic_path == "") {
        std::cout << pic_path << " - does not exist." << std::endl;
        pic_path = "src/assets/no_user.png";
    };

    width = 100;
    height = 100;
    position = {0,0};
};

void player_banner::load_profile_texture(float _height, float _width){
    Image user_pic = LoadImage(pic_path.c_str());
    if (user_pic.data == nullptr) {
        std::cerr << "Failed to load image: " << pic_path << std::endl;
        return;
    };

    ImageResize(&user_pic, _width, _height);

    profile_pic = LoadTextureFromImage(user_pic);
    UnloadImage(user_pic);
};

void player_banner::draw_right(Vector2 _position, float _width, float _height, bool turn) {
    if (profile_pic.height != static_cast<int>(height)){
        load_profile_texture(static_cast<int>(height), static_cast<int>(height));
    };

    position = _position;
    height = _height;
    width = _width;
    float player_font_spacing = height*0.05;
    float creator_tag_font_spacing = height*0.05;

    float sep = height*0.3f;
    float player_name_ratio = 0.60f;
    float creator_tag_ratio = 0.40f;
    float padding = 0.1f*height;

    Rectangle outline = {position.x, position.y, width, height};
    Vector2 text_offset = {height+padding+sep, 0};

    DrawRectangleGradientEx(outline, RED, RED, WHITE, WHITE);

    if(turn){
        DrawRectangleLinesEx(outline, padding/2.f, GREEN);
    } else {
        DrawRectangleLinesEx(outline, padding/2.f, WHITE);
    }

    DrawTextureEx(profile_pic, position + Vector2{padding, 0}, 0, 1, WHITE);

    float player_font_size = (height-padding*2)*player_name_ratio;
    float creator_tag_font_size = (height-padding*2)*creator_tag_ratio;

    Vector2 player_name_area = MeasureTextEx(GetFontDefault(), player_name.c_str(), (height-2*padding)*player_name_ratio, player_font_spacing);
    Vector2 creator_tag_area =  MeasureTextEx(GetFontDefault(), creator_tag.c_str(), (height-2*padding)*creator_tag_ratio, creator_tag_font_spacing);

    float allowed_text_width = width-height-2*padding-sep;
    
    if(player_name_area.x > allowed_text_width){
        float scaling = (allowed_text_width)/player_name_area.x;
        player_font_size *= scaling;
        player_font_spacing *= scaling;
    };

    if(creator_tag_area.x > allowed_text_width){
        float scaling = (allowed_text_width)/creator_tag_area.x;
        creator_tag_font_size *= scaling;
        creator_tag_font_spacing *= scaling;

    };

    Vector2 player_name_offset = Vector2{text_offset.x, (height-padding*2)*player_name_ratio-player_font_size + padding};
    Vector2 creator_tag_offset = Vector2{text_offset.x, (height-padding*2) - creator_tag_font_size + padding};

    DrawTextEx(GetFontDefault(), player_name.c_str(), position + player_name_offset, player_font_size, player_font_spacing, BLACK);
    DrawTextEx(GetFontDefault(), creator_tag.c_str(), position + creator_tag_offset, creator_tag_font_size, creator_tag_font_spacing, GRAY);
};


void player_banner::draw_left(Vector2 _position, float _width, float _height, bool turn){
    if (profile_pic.height != static_cast<int>(height)){
        load_profile_texture(static_cast<int>(height), static_cast<int>(height));
    };

    position = _position;
    height = _height;
    width = _width;
    float player_font_spacing = height*0.05;
    float creator_tag_font_spacing = height*0.05;

    float sep = height*0.3f;
    float player_name_ratio = 0.60f;
    float creator_tag_ratio = 0.40f;
    float padding = 0.1f*height;

    Rectangle outline = {position.x, position.y, width, height};
    Vector2 pic_offset = {width-(height+padding), 0};

    DrawRectangleGradientEx(outline, WHITE, WHITE, BLUE, BLUE);

    if(turn){
        DrawRectangleLinesEx(outline, padding/2.f, GREEN);
    } else {
        DrawRectangleLinesEx(outline, padding/2.f, WHITE);
    }

    DrawTextureEx(profile_pic, position + pic_offset, 0, 1, WHITE);

    float player_font_size = (height-padding*2)*player_name_ratio;
    float creator_tag_font_size = (height-padding*2)*creator_tag_ratio;

    Vector2 player_name_area = MeasureTextEx(GetFontDefault(), player_name.c_str(), (height-2*padding)*player_name_ratio, player_font_spacing);
    Vector2 creator_tag_area =  MeasureTextEx(GetFontDefault(), creator_tag.c_str(), (height-2*padding)*creator_tag_ratio, creator_tag_font_spacing);

    float allowed_text_width = pic_offset.x-padding-sep;
    
    if(player_name_area.x > allowed_text_width){
        float scaling = (allowed_text_width)/player_name_area.x;
        player_font_size *= scaling;
        player_font_spacing *= scaling;
    };

    if(creator_tag_area.x > allowed_text_width){
        float scaling = (allowed_text_width)/creator_tag_area.x;
        creator_tag_font_size *= scaling;
        creator_tag_font_spacing *= scaling;

    };

    Vector2 player_name_offset = Vector2{padding, (height-padding*2)*player_name_ratio-player_font_size + padding};
    Vector2 creator_tag_offset = Vector2{padding, (height-padding*2) - creator_tag_font_size + padding};

    DrawTextEx(GetFontDefault(), player_name.c_str(), position + player_name_offset, player_font_size, player_font_spacing, BLACK);
    DrawTextEx(GetFontDefault(), creator_tag.c_str(), position + creator_tag_offset, creator_tag_font_size, creator_tag_font_spacing, GRAY);
};
