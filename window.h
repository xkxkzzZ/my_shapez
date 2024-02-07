
#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <SDL2/SDL.h>


class Window final {
public:
    friend class Renderer;
    Window(const std::string& title,int w,int h);
    Window(const Window&)=delete;
    Window(Window&& other)  noexcept {
        window_ = other.window_;
        other.window_= nullptr;
    }
    void Set_window_size(int w, int h);
    Window& operator=(const Window&) = delete;
    ~Window();

private:
    SDL_Window* window_;
};




#endif //WINDOW_H
