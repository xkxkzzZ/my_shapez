
#include "window.h"
Window::Window(const std::string&title, int w, int h) {
    window_ = SDL_CreateWindow("Hello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        w,h,SDL_WINDOW_SHOWN);
}

void Window::Set_window_size(int w, int h) {
    SDL_SetWindowSize(window_, w, h);
}

Window::~Window() {
    SDL_DestroyWindow(window_);
}
