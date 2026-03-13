#include "raylib.h"
#include "raymath.h"
#include <nlohmann/json.hpp>

#include <cstring>
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <random>

#include "big_board.hpp"
#include "globals.hpp"
#include "grid.hpp"
#include "player_banner.hpp"
#include "sub_board.hpp"

#define SETTINGS_PATH "settings.json"

float screen_width = 800;
float screen_height = 800;

Color sub_color = {173, 151, 104, 255};

Vector2 main_pos = {screen_width * 0.125f, screen_height * 0.2f};
grid main_grid(main_pos, screen_width * 0.75f, screen_height * 0.75f,
               {133, 125, 103, 255}, 0.05f);
big_board board;
std::vector<grid> sub_grids;

float hand_place_aq;

std::string config_path_cross; 
std::string config_path_dot; 

player_banner cross_banner;
player_banner dot_banner;

std::string win_msg = ""; 

std::string so_path_cross;
std::string so_path_dot;

std::thread getting_move;
std::atomic<bool> ready_to_move = false;
std::atomic<bool> game_stopped = false;
int time_since_last_move;
static std::atomic<int> time_left;

auto start_time = std::chrono::steady_clock::now();  
bool hand_turn = false;

nlohmann::json settings;

int get_closest(Vector2 spot, std::vector<Vector2> candidates) {
  std::vector<float> dist_list;
  dist_list.resize(candidates.size());

  if (spot.x < 0 || spot.y < 0 || spot.x > screen_width ||
      spot.y > screen_height) {
    return -1;
  }

  int target = 0;

  for (int i = 0; i < candidates.size(); i++) {
    dist_list[i] = dist(spot, candidates[i]);
    if (dist_list[i] < dist(spot, candidates[target])) {
      target = static_cast<int>(i);
    };
  };
  return target;
};

Vector2 GetMousePositionScaled() {
    float scale = (float)GetRenderWidth() / (float)GetScreenWidth();
    Vector2 mouse = GetMousePosition();
    return (Vector2){ mouse.x * scale, mouse.y * scale };
}

move hand_place(raw_boardstate b, std::atomic<int> *t) {
  hand_turn = true;
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse_pos = GetMousePositionScaled();
    int sub = get_closest(mouse_pos, main_grid.get_global_grid_spots());

    int spot = get_closest(mouse_pos, sub_grids[sub].get_global_grid_spots());

    if (!board.is_valid_move((move){sub, spot})) {
      return {-1, -1};
    };

    if (dist(mouse_pos, sub_grids[sub].get_global_grid_spots()[spot]) <=
        hand_place_aq) {
      move _move = {sub, spot};
      return _move;
    };
  };
  return {-1, -1};
};

move get_move(raw_boardstate boardstate, std::string so_file_path, std::string func_name) {
  hand_turn = false;
  big_board _board = big_board(boardstate);
  _board.print();
  move _move;
  move (*func)(raw_boardstate, std::atomic<int>*);
  void *so_file = dlopen(so_file_path.c_str(), RTLD_LAZY);
  if (!so_file) {
    func = hand_place;
  } else {
    dlerror();
    void *sym = dlsym(so_file, func_name.c_str());
    if (!sym) {
      std::cerr << "dlsym failed for " << func_name << std::endl;
      //fallback or exit
    }
    std::memcpy(&func, &sym, sizeof(sym));
  }
  _move = func(boardstate, &time_left);

  dlclose(so_file);
  return _move;
}

void next_move() {
  raw_boardstate current_boardstate = board.get_raw_boardstate();
  move _move;
  if (board.get_turn() == CROSS) {
    if (so_path_cross == "") {
      _move = hand_place((raw_boardstate){}, &time_left);
    } else {
      _move = get_move(current_boardstate, so_path_cross,
                       cross_banner.get_player_name());
    }
  } else if (board.get_turn() == DOT) {
    if (so_path_dot == "") {
      _move = hand_place((raw_boardstate){}, &time_left);
    } else {
      _move = get_move(current_boardstate, so_path_dot,
                       dot_banner.get_player_name());
    }
  };

  if (board.is_valid_move(_move) && !game_stopped) {
    board.place_piece(_move.sub, _move.spot);
  } else if (_move.sub != -1 && _move.spot != -1 && !game_stopped) {
    std::cerr << '{' << _move.sub << ", " << _move.spot << '}'
              << " is not at valid move" << std::endl;
    board.set_winner(static_cast<piece>(-board.get_turn()));
    win_msg = " won due to opponent misplacing";
    game_stopped = true;
  }
  ready_to_move = true;
};

void player_banner_init(){
  config_path_cross = settings["cross_path"].get<std::string>() + "config.csv";
  config_path_dot = settings["dot_path"].get<std::string>() + "config.csv";

  cross_banner = player_banner(config_path_cross);
  dot_banner = player_banner(config_path_dot);
}

void draw() {
  BeginDrawing();
  ClearBackground(BG_COLOR);

  cross_banner.draw_right({screen_width * 0.025f, screen_height * 0.06875f},
                          screen_width * 0.375f, screen_height * 0.1125f,
                          true ? board.get_turn() == CROSS : false);
  dot_banner.draw_left(
      {screen_width - cross_banner.get_position().x - cross_banner.get_width(),
       cross_banner.get_position().y},
      cross_banner.get_width(), cross_banner.get_height(),
      true ? board.get_turn() == DOT : false);

  DrawText("Tic Tac Toe Masters",
           screen_width / 2.f - MeasureText("Tic Tac Toe Masters", 20) / 2.f,
           10, 20, BLACK);

  int status_font_size = 30;
  int status_margin = 15;
  int status_x = status_margin;
  int status_y = status_margin;
  Color status_color = BLACK;
  
  int countdown_font_size = cross_banner.get_height();
  int countdown_x = screen_width/2 - countdown_font_size/2;
  int countdown_y = cross_banner.get_position().y;
  Color countdown_color = BLACK;

  const char *turn_label = "Turn:";
  int turn_label_font_size = 30;
  int turn_label_margin = 15;
  int turn_label_width = MeasureText(turn_label, turn_label_font_size);

  // Calculate dynamic positions
  int turn_label_x = screen_width - turn_label_width - 2 * turn_label_margin -
                     turn_label_font_size;
  int turn_label_y = turn_label_margin;

  DrawText(turn_label, turn_label_x, turn_label_y, turn_label_font_size, BLACK);

  char turn_char = (board.get_turn() == CROSS) ? 'X' : 'O';
  char turn_str[2] = {turn_char, '\0'};
  Color turn_color = (board.get_turn() == CROSS) ? RED : BLUE;

  // Place the turn character right after the label, with a margin
  int turn_char_x = turn_label_x + turn_label_width + turn_label_margin;
  int turn_char_y = turn_label_y;

  DrawText(turn_str, turn_char_x, turn_char_y, turn_label_font_size,
           turn_color);

  if (board.get_winner() == EMPTY && !board.get_draw()) {
    if (board.get_current() != -1) {
      Vector2 indicator_pos =
          sub_grids[board.get_current()].get_global_position();
      Vector2 indicator_size = {
          sub_grids[board.get_current()].get_area().width,
          sub_grids[board.get_current()].get_area().height};
      Color indicator_color = GREEN;

      DrawRectangleV(indicator_pos, indicator_size, indicator_color);

    } else {
      Vector2 indicator_pos = main_grid.get_global_position();
      Vector2 indicator_size = {main_grid.get_area().width,
                                main_grid.get_area().height};
      Color indicator_color = GREEN;

      DrawRectangleV(indicator_pos, indicator_size, indicator_color);
    };
  };

  for (int sub = 0; sub < 9; sub++) {
    sub_board cur_sub = board.get_sub_boards()[sub];
    if (cur_sub.get_winner() == EMPTY && !cur_sub.get_draw()) {
      sub_grids[sub].draw_grid();
      for (int spot = 0; spot < 9; spot++) {
        piece curr = board.get_sub_boards()[sub].get_spots()[spot];
        if (curr != EMPTY) {
          if (curr == CROSS) {
            sub_grids[sub].draw_X(spot);
          } else {
            sub_grids[sub].draw_O(spot);
          };
        };
      };
    } else if(cur_sub.get_winner() == EMPTY && cur_sub.get_draw()){
      sub_grids[sub].draw_tie();
    }else if(cur_sub.get_winner() != EMPTY){
      sub_grids[sub].draw_winner(board.get_sub_boards()[sub].get_winner());
    };
  };

  main_grid.draw_grid();
  
  if (board.get_winner() == EMPTY && !board.get_draw()) {
    const char *ongoing_text = "Ongoing...";
    DrawText(ongoing_text, status_x, status_y, status_font_size, status_color);
    if(!hand_turn){
      DrawText(std::to_string((int)settings["timeout"]-time_since_last_move).c_str(), countdown_x, countdown_y, countdown_font_size, countdown_color);
    };
  } else if (board.get_draw() && board.get_winner() == EMPTY){
    const char *draw_label = "Draw :(";
    DrawText(draw_label, status_x, status_y, status_font_size, status_color);
  } else {
    std::string winner_label = "Winner: ";
    DrawText(winner_label.c_str(), status_x, status_y, status_font_size, status_color);

    int label_width = MeasureText(winner_label.c_str(), status_font_size);
    int winner_x = status_x + label_width + 5;

    Vector2 win_msg_pos = {main_grid.get_global_position().x,
                           main_grid.get_global_position().y+main_grid.get_area().height};
    int win_msg_font_size = status_font_size;

    if (board.get_winner() == CROSS) {
      DrawText(cross_banner.get_player_name().c_str(), winner_x, status_y, status_font_size, RED);
      DrawText((cross_banner.get_player_name() + win_msg).c_str(), win_msg_pos.x, win_msg_pos.y, win_msg_font_size, RED);
    } else if (board.get_winner() == DOT) {
      DrawText(dot_banner.get_player_name().c_str(), winner_x, status_y, status_font_size, BLUE);
      DrawText((dot_banner.get_player_name() + win_msg).c_str(), win_msg_pos.x, win_msg_pos.y, win_msg_font_size, BLUE);
    } else {
      DrawText("None", winner_x, status_y, status_font_size, status_color);
    }
    
    if(board.get_winning_set() != -1){
      float width = (main_grid.get_area().width/2.f)*0.85f;
      std::vector<int> win_set = WINNING_SETS[board.get_winning_set()];  
      Vector2 start = {main_grid.get_global_grid_spots()[win_set[0]].x,
                      main_grid.get_global_grid_spots()[win_set[0]].y}; 
      Vector2 end = {main_grid.get_global_grid_spots()[win_set[2]].x,
                     main_grid.get_global_grid_spots()[win_set[2]].y};
   
      float lenght = main_grid.get_area().width;
      //start = Vector2Scale(Vector2Normalize(start), lenght);
      //end = Vector2Scale(Vector2Normalize(end), lenght);

      float thickness = main_grid.get_global_pole_width()/2;
      Color strike_color = VIOLET;

      DrawLineEx(start, end, thickness, strike_color);
    }
  };

  EndDrawing();
}

void read_settings(std::string settings_path){
  using json = nlohmann::json;
  std::ifstream settings_file(settings_path);
  settings = json::parse(settings_file);
}

void setup_board(){
  if(settings["starting"].get<std::string>() == "cross"){
    board.set_turn(CROSS);
  } else if(settings["starting"].get<std::string>() == "dot"){
    board.set_turn(DOT);
  };

  if(settings["seed"] == -1){
    srand(time(0));
  } else {
    srand((int)settings["seed"]);
  }

  for(int i = 0; i < std::max((int)settings["starting_pieces"], 0); i++){
    std::vector<move> _moves = board.get_allowed_moves(); 
    int pick = rand() % _moves.size();
    move _move = _moves[pick];
    board.place_piece(_move.sub, _move.spot);
  }
}

int main() {
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(screen_width, screen_height, "TTT-masters");
  printf("Screen: %dx%d\n", GetScreenWidth(), GetScreenHeight());
  printf("Render: %dx%d\n", GetRenderWidth(), GetRenderHeight());
  SetTargetFPS(60);

  std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

  if (!std::filesystem::exists(SETTINGS_PATH)) {
      std::cout << SETTINGS_PATH << " does not exist";
      return 0; 
  }
  read_settings(SETTINGS_PATH); 

  player_banner_init();

  so_path_cross = settings["cross_path"].get<std::string>() + cross_banner.get_player_name() + ".so";
  if (!std::filesystem::exists(so_path_cross)) {
    std::cout << so_path_cross << " does not exist";
    so_path_cross = "";
  }

  so_path_dot = settings["dot_path"].get<std::string>() + dot_banner.get_player_name() + ".so";
  if (!std::filesystem::exists(so_path_dot)) {
    std::cout << so_path_dot << " does not exist";
    so_path_dot = "";
  }

  setup_board();

  sub_grids.resize(9);

  for (int sub = 0; sub < 9; sub++) {
    float sub_pole_width = main_grid.get_rel_pole_width();
    float sub_width =
        main_grid.get_area().width / 3.f - main_grid.get_global_pole_width();
    float sub_height =
        main_grid.get_area().height / 3.f - main_grid.get_global_pole_width();
    Vector2 offset_vector = {sub_width / 2.f, sub_height / 2.f};
    Vector2 sub_pos =
        Vector2Subtract(main_grid.get_global_grid_spots()[sub], offset_vector);
    sub_grids[sub] =
        grid(sub_pos, sub_width, sub_height, sub_color, sub_pole_width);
  };

  hand_place_aq = sub_grids[0].get_area().width / 3.f -
                  sub_grids[0].get_global_pole_width();

  board.print();
  getting_move = std::thread(next_move);
  while (!WindowShouldClose()) {
    draw();
    if (board.get_winner() != EMPTY || board.get_draw()) {
      continue;
    }
    time_since_last_move = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count();
    time_left = (int)settings["timeout"]*1000 - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
    if(time_since_last_move >= static_cast<int>(settings["timeout"]) and !hand_turn){
      board.set_winner(static_cast<piece>(-board.get_turn()));
      win_msg = " won due to opponent timeout";
      game_stopped = true;
      getting_move.detach();
    } else if(ready_to_move){
      getting_move.join();
      ready_to_move = false;
      start_time = std::chrono::steady_clock::now();
      getting_move = std::thread([]() {
        try {
          next_move();
        } catch (const std::exception& e) {
            board.set_winner(static_cast<piece>(-board.get_turn()));
            win_msg = " won due to opponent chrash";
            game_stopped = true;
            std::cerr << "Thread crashed: " << e.what() << std::endl;
        } catch (...) {
            board.set_winner(static_cast<piece>(-board.get_turn()));
            win_msg = " won due to opponent chrash";
            game_stopped = true;
            std::cerr << "Thread crashed: unknown error" << std::endl;
        }
      });
    } 
  }
  CloseWindow();
  return 0;
}
