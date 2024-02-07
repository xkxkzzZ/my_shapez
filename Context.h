

#ifndef CONTEXT_H
#define CONTEXT_H
#include <memory>
#include <vector>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "Renderer.h"


struct Context final {
    static TTF_Font *font;
    Window window;

    Renderer renderer;

    Context(Window&& window,Renderer&& renderer):
    window(std::move(window)),renderer(std::move(renderer)) {}

    static void Init();

    static void Quit();

    static Context& Inst();


private:
    static std::unique_ptr<Context>instance_;
    static SDL_Texture* circle_mine_texture;
    static SDL_Texture* triangle_mine_texture;
    static SDL_Texture* circle_texture;
    static SDL_Texture* triangle_texture;
    static SDL_Texture* half_circle_texture;
    static SDL_Texture* digger_texture;
    static SDL_Texture* translater_texture;
    static SDL_Texture* right_translater_texture;
    static SDL_Texture* left_translater_texture;
    static SDL_Texture* cutter_texture;
    static SDL_Texture* cutter_left_texture;
    static SDL_Texture* cutter_right_texture;
    static SDL_Texture* center_texture;
    static SDL_Texture* bin_texture;
    static SDL_Texture* rotator_texture;
    static SDL_Texture* back_texture;
    static SDL_Texture* save_texture;
    static SDL_Texture* newgame_texture;
    static SDL_Texture* continuegame_texture;
    static SDL_Texture* biggermap_texture;
    static SDL_Texture* biggercenter_texture;
    static SDL_Texture* increasevalue_texture;
    static SDL_Texture* clear_texture;

public:
    void drawBackground(int W,int H);
    void draw_mouse(bool Is_Get,Machine_type Get_Type,Rotation Get_Rotation, Direction Get_Direction, SDL_Rect MouseRect, double button_angle);
    void draw_button(vector<SDL_Rect>buttons);
    void drawString(string s,SDL_Rect Rect, SDL_Color color);
    void drawScore(Shape_type needtype, int neednum, int mynum, int money);
    void draw_betweenLevel(vector<SDL_Rect>buff);
    void draw_beginning(vector<SDL_Rect>begs);
    void set_window_size(int w, int h);
};
#endif //CONTEXT_H
