
#ifndef GAME_H
#define GAME_H

#include "Model.h"
#include "Context.h"


class Game {
    enum  {
        gaming, between_level, beginning
    }game_state;
    bool isquit = false;
    SDL_Event event{};
    bool auto_run = true;
    uint64_t last_time = SDL_GetTicks64();
    double angle = 0;
    double button_angle = 0;
    int mouseX{}, mouseY{};

    bool Is_Get = false;
    Machine_type Get_Type;
    Direction Get_Direction = north;
    Rotation Get_Rotation = r_middle;

    bool Have_Bigged_Map = false;
    bool Have_Bigged_Center = false;

    SDL_Rect MapRect;
    SDL_Rect MouseRect;
    SDL_Rect button_digger;
    SDL_Rect button_translater;
    SDL_Rect button_cutter;
    SDL_Rect button_bin;
    SDL_Rect button_rotator;
    SDL_Rect button_back;
    SDL_Rect button_save;
    SDL_Rect buf_digger;
    SDL_Rect buf_translater;
    SDL_Rect buf_cutter;
    SDL_Rect continue_game;
    SDL_Rect new_game;
    SDL_Rect bigger_map;
    SDL_Rect bigger_center;
    SDL_Rect increase_value;
    SDL_Rect button_clear;


    void Game_init();
    void begin_new_game();
    void continue_old_game();
    void end_game();
    void clear_machine();
    void Size_Rect();

    bool check_mouse_rect(SDL_Rect r);

    void main_loop();

    void draw_gaming();

    void message_handling();
    void message_handling_beginning();
    void message_handling_gaming();
    void message_handling_between_level();


    void Load_background();
    void Load_out();
    void Load_in();
    void Load_General_buf_out();
    void Load_General_buf_in();

    void next_level();

    void Clear_buf();
    void Bigger_map();
    void Bigger_center();
    void Smaller_map();
    void Smaller_center();

public:
    Game();

    ~Game();

    void run();
};


#endif //GAME_H
