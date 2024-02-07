
#include "Renderer.h"

Renderer::Renderer(const Window&window) {
    renderer_ = SDL_CreateRenderer(window.window_, -1, 0);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
}

void Renderer::SetColor(const SDL_Color&c) const {
    SDL_SetRenderDrawColor(renderer_, c.r, c.g, c.b, c.a);
}

void Renderer::Clear() const {
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255); // 白色背景
    SDL_RenderClear(renderer_);
}

void Renderer::Present() const {
    SDL_RenderPresent(renderer_);
}
/*
void Renderer::DrawCircle(int centerX, int centerY, int radius) const {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer_, centerX + x, centerY + y);
            }
        }
    }
}
*/
void Renderer::DrawRect(const SDL_Rect& rect) {
    SDL_RenderDrawRect(renderer_,&rect);
}

void Renderer::FillRect(const SDL_Rect& rect) const {
    SDL_RenderFillRect(renderer_,&rect);
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
}


void Renderer::addTexture(SDL_Texture* texture, SDL_Rect Rect) {
    SDL_RenderCopy(renderer_,texture, nullptr, &Rect);
}

void Renderer::addTexture(SDL_Texture*texture, SDL_Rect Rect, double angle) {
    SDL_RenderCopyEx(renderer_,texture, nullptr, &Rect,
                                     angle,NULL, SDL_FLIP_NONE);
}

SDL_Texture* Renderer::LoadTexture(const std::string& filePath) const {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (surface == nullptr) {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    if(!renderer_) {
        SDL_Log("error");
    }
    if (texture == nullptr) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return nullptr;
    }
    return texture;
}

SDL_Rect Renderer::get_Rect(SDL_Rect sourceRect, Direction d,Rotation r, double off) {
    SDL_Rect destRect;
    destRect.x = sourceRect.x;
    destRect.y = sourceRect.y;
    destRect.w = sourceRect.w;
    destRect.h = sourceRect.h;
    switch (d) {
        case north:
            destRect.y = sourceRect.y - (double)BlockSize * (off - 0.5) * 0.8;
        if(r == r_left) {
            destRect.x = sourceRect.x - (double)BlockSize * (off - 0.5) * 0.5 - 0.2 * (double)BlockSize;
            destRect.y = sourceRect.y - (double)BlockSize * (off - 0.5) * 0.5 + 0.2 * (double)BlockSize;
        }
        if(r == r_right) {
            destRect.x = sourceRect.x + (double)BlockSize * (off - 0.5) * 0.5 + 0.2 * (double)BlockSize;
            destRect.y = sourceRect.y - (double)BlockSize * (off - 0.5) * 0.5 + 0.2 * (double)BlockSize;
        }
        break;
        case south:
            destRect.y = sourceRect.y + (double)BlockSize * (off - 0.5) * 0.8;
        if(r == r_left) {
            destRect.x = sourceRect.x + (double)BlockSize * (off - 0.5)* 0.5 + 0.2 * (double)BlockSize;
            destRect.y = sourceRect.y + (double)BlockSize * (off - 0.5) * 0.5 - 0.2 * (double)BlockSize;
        }
        if(r == r_right) {
            destRect.x = sourceRect.x - (double)BlockSize * (off - 0.5)* 0.5 - 0.2 * (double)BlockSize;
            destRect.y = sourceRect.y + (double)BlockSize * (off - 0.5) * 0.5 - 0.2 * (double)BlockSize;
        }
        break;
        case east:
            destRect.x = sourceRect.x + (double)BlockSize * (off - 0.5) * 0.8;
        if(r == r_left) {
            destRect.y = sourceRect.y - (double)BlockSize * (off - 0.5)* 0.5 - 0.2 * (double)BlockSize;
            destRect.x = sourceRect.x + (double)BlockSize * (off - 0.5) * 0.5 - 0.2 * (double)BlockSize;
        }
        if(r == r_right) {
            destRect.y = sourceRect.y + (double)BlockSize * (off - 0.5)* 0.5 + 0.2 * (double)BlockSize;
            destRect.x = sourceRect.x + (double)BlockSize * (off - 0.5) * 0.5 - 0.2 * (double)BlockSize;
        }
        break;
        case west:
            destRect.x = sourceRect.x - (double)BlockSize * (off - 0.5) * 0.8;
        if(r == r_left) {
            destRect.y = sourceRect.y + (double)BlockSize * (off - 0.5)* 0.5 + 0.2 * (double)BlockSize;
            destRect.x = sourceRect.x - (double)BlockSize * (off - 0.5) * 0.5 + 0.2 * (double)BlockSize;
        }
        if(r == r_right) {
            destRect.y = sourceRect.y - (double)BlockSize * (off - 0.5)* 0.5 - 0.2 * (double)BlockSize;
            destRect.x = sourceRect.x - (double)BlockSize * (off - 0.5) * 0.5 + 0.2 * (double)BlockSize;
        }
        break;
        default:
            break;
    }
    return destRect;
}

