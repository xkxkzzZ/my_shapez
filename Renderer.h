
#ifndef RENDERER_H
#define RENDERER_H

#include "Model.h"
#include "window.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
class Renderer final {
public:
    explicit Renderer(const Window&);
    Renderer(const Renderer&)=delete;
    Renderer(Renderer&& other) noexcept {
        renderer_ = other.renderer_;
        other.renderer_ = nullptr;
    }
    Renderer& operator=(const Renderer&)=delete;
    ~Renderer();
    void SetColor(const SDL_Color&) const;
    void Clear() const;
    void Present() const;
public://基本绘图
    //void DrawCircle(int centerX,int centerY,int radius)const;
    void DrawRect(const SDL_Rect& rect);
    void FillRect(const SDL_Rect& rect) const;
    void DrawLine(int x1,int y1,int x2,int y2);
    void addTexture(SDL_Texture*,SDL_Rect);
    void addTexture(SDL_Texture*,SDL_Rect,double angle);
    SDL_Texture* LoadTexture(const std::string&filePath) const;
    bool exist() {
        return renderer_!=nullptr;
    }
    static SDL_Rect get_Rect(SDL_Rect sourceRect, Direction d, Rotation r ,double off);


    SDL_Renderer* renderer_;
};
#endif //RENDERER_H
