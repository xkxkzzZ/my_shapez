
#include "Context.h"


std::unique_ptr<Context> Context::instance_ = nullptr;
TTF_Font* Context::font = nullptr;
SDL_Texture* Context::circle_texture = nullptr;
SDL_Texture* Context::triangle_texture = nullptr;
SDL_Texture* Context::half_circle_texture = nullptr;
SDL_Texture* Context::circle_mine_texture = nullptr;
SDL_Texture* Context::triangle_mine_texture = nullptr;
SDL_Texture* Context::right_translater_texture = nullptr;
SDL_Texture* Context::left_translater_texture = nullptr;
SDL_Texture* Context::digger_texture = nullptr;
SDL_Texture* Context::translater_texture = nullptr;
SDL_Texture* Context::cutter_texture = nullptr;
SDL_Texture* Context::cutter_left_texture = nullptr;
SDL_Texture* Context::cutter_right_texture = nullptr;
SDL_Texture* Context::center_texture = nullptr;
SDL_Texture* Context::bin_texture = nullptr;
SDL_Texture* Context::rotator_texture = nullptr;
SDL_Texture* Context::back_texture = nullptr;
SDL_Texture* Context::save_texture = nullptr;
SDL_Texture* Context::newgame_texture = nullptr;
SDL_Texture* Context::continuegame_texture = nullptr;
SDL_Texture* Context::biggermap_texture = nullptr;
SDL_Texture* Context::biggercenter_texture = nullptr;
SDL_Texture* Context::increasevalue_texture = nullptr;
SDL_Texture* Context::clear_texture = nullptr;


void Context::Init() {
    if (!instance_) {
        Window window("Hello", Mapw *BlockSize, (Maph+1) * BlockSize);
        Renderer renderer(window);
        instance_ = std::make_unique<Context>(Context{
            std::move(window), std::move(renderer)
        });
        TTF_Init();
        //instance_->renderer
        circle_mine_texture = instance_->renderer.LoadTexture("PNGs\\circle_mine.PNG");
        triangle_mine_texture = instance_->renderer.LoadTexture("PNGs\\triangle_mine.PNG");
        circle_texture = instance_->renderer.LoadTexture("PNGs\\circle.PNG");
        triangle_texture = instance_->renderer.LoadTexture("PNGs\\triangle.PNG");
        half_circle_texture = instance_->renderer.LoadTexture("PNGs\\half_circle.PNG");
        digger_texture = instance_->renderer.LoadTexture("PNGs\\digger.PNG");
        translater_texture = instance_->renderer.LoadTexture("PNGs\\translater.PNG");
        right_translater_texture = instance_->renderer.LoadTexture("PNGs\\right_translater.PNG");
        left_translater_texture = instance_->renderer.LoadTexture("PNGs\\left_translater.PNG");;
        cutter_texture = instance_->renderer.LoadTexture("PNGs\\cutter.PNG");
        cutter_left_texture = instance_->renderer.LoadTexture("PNGs\\cutter_left.PNG");
        cutter_right_texture = instance_->renderer.LoadTexture("PNGs\\cutter_right.PNG");
        center_texture = instance_->renderer.LoadTexture("PNGs\\center.PNG");
        bin_texture = instance_->renderer.LoadTexture("PNGs\\bin.PNG");
        rotator_texture = instance_->renderer.LoadTexture("PNGs\\rotator.PNG");
        back_texture = instance_->renderer.LoadTexture("PNGs\\back.PNG");
        save_texture = instance_->renderer.LoadTexture("PNGs\\save.PNG");
        newgame_texture = instance_->renderer.LoadTexture("PNGs\\newgame.PNG");
        continuegame_texture = instance_->renderer.LoadTexture("PNGs\\continuegame.PNG");
        biggermap_texture = instance_->renderer.LoadTexture("PNGs\\biggermap.PNG");
        biggercenter_texture = instance_->renderer.LoadTexture("PNGs\\biggercenter.PNG");
        increasevalue_texture = instance_->renderer.LoadTexture("PNGs\\increasevalue.PNG");
        clear_texture = instance_->renderer.LoadTexture("PNGs\\clear.PNG");

        font = TTF_OpenFont("Loading\\font5.ttf", 50);
        if (!font) SDL_Log("can not open font", SDL_GetError());
    }
}

void Context::Quit() {
    instance_.reset();
    SDL_DestroyTexture( circle_mine_texture) ;
    SDL_DestroyTexture( triangle_mine_texture) ;
    SDL_DestroyTexture( circle_texture) ;
    SDL_DestroyTexture( triangle_texture) ;
    SDL_DestroyTexture( digger_texture) ;
    SDL_DestroyTexture( translater_texture) ;
    SDL_DestroyTexture( right_translater_texture) ;
    SDL_DestroyTexture( left_translater_texture) ;
    SDL_DestroyTexture( cutter_texture) ;
    SDL_DestroyTexture( center_texture) ;
    SDL_DestroyTexture( bin_texture) ;
    SDL_DestroyTexture( rotator_texture) ;
    SDL_DestroyTexture( newgame_texture) ;
    SDL_DestroyTexture( continuegame_texture) ;
    SDL_DestroyTexture( biggermap_texture) ;
    TTF_CloseFont(font);
}

Context& Context::Inst() {
    SDL_assert(instance_);
    return (*instance_);
}

void Context::drawBackground(int W, int H) {

    SDL_Rect BlockRect;
    double angle;
    BlockRect.w = BlockSize;
    BlockRect.h = BlockSize;
    renderer.Clear();

    renderer.SetColor(SDL_Color{200, 200, 200, 200});
    for (int x = 0; x < W * BlockSize; x += BlockSize) {
        renderer.DrawLine(x, 0, x, H * BlockSize);
    }
    for (int y = 0; y < H * BlockSize; y += BlockSize) {
        renderer.DrawLine(0, y, W * BlockSize, y);
    }

    for (int x = 0; x < W; x += 1) {
        for (int y = 0; y < H; y += 1) {
            Background_type bt = background[x][y];
            BlockRect.x = x * BlockSize;
            BlockRect.y = y * BlockSize;
            switch (bt) {
                case circle_mine:
                    renderer.addTexture(circle_mine_texture, BlockRect);
                    break;
                case triangle_mine:
                    renderer.addTexture(triangle_mine_texture, BlockRect);
                    break;
                default:
                    break;
            }
        }
    }
    for (int x = 0; x < W; x += 1) {
        for (int y = 0; y < H; y += 1) {
            Machine* machine = machine_map[x][y];
            if (!machine) continue;
            BlockRect.x = x * BlockSize;
            BlockRect.y = y * BlockSize;
            switch (machine->direction) {
            case south: angle = 180; break;
            case east: angle = 90; break;
            case west: angle = 270; break;
            case north: angle = 0; break;
            default: angle = 0; break;
            }
            switch (machine->type) {
                case digger:
                    renderer.addTexture(digger_texture, BlockRect, angle);
                    break;
                case translater: {
                    if(machine->rotation == r_middle) renderer.addTexture(translater_texture, BlockRect, angle);
                    if(machine->rotation == r_left) renderer.addTexture(left_translater_texture, BlockRect, angle);
                    if(machine->rotation == r_right) renderer.addTexture(right_translater_texture, BlockRect, angle);
                    break;
                }
                case center: {
                    renderer.addTexture(center_texture, BlockRect);
                    break;
                }
                case cutter: {
                    renderer.addTexture(cutter_left_texture, BlockRect, angle);
                    break;
                }
                case cutter_righthalf:
                    renderer.addTexture(cutter_right_texture, BlockRect, angle);
                    break;
                case bin:
                    renderer.addTexture(bin_texture, BlockRect);
                    break;
                case rotator:
                    renderer.addTexture(rotator_texture, BlockRect, angle);
                    break;
            default:
                break;
            }
        }
    }
    for (int x = 0; x < W; x += 1) {
        for (int y = 0; y < H; y += 1) {
            Machine* machine = machine_map[x][y];
            if (!machine) continue;
            if (machine->type != translater) continue;
            BlockRect.x = x * BlockSize;
            BlockRect.y = y * BlockSize;
            for (const auto shape: machine->shape_list) {
                SDL_Rect ShapeRect = Renderer::get_Rect(BlockRect, machine->direction,machine->rotation, shape->offset);
                switch (shape->type) {
                case circle:
                    renderer.addTexture(circle_texture, ShapeRect, shape->angle);
                    break;
                case triangle:
                    renderer.addTexture(triangle_texture, ShapeRect, shape->angle);
                    break;
                case half_circle:
                    renderer.addTexture(half_circle_texture, ShapeRect, shape->angle);
                    break;
                default: break;
                }
            }
        }
    }
}

void Context::draw_mouse(bool Is_Get, Machine_type Get_Type, Rotation Get_Rotation, Direction Get_Direction, SDL_Rect MouseRect, double button_angle) {
    if (Is_Get) {
        switch (Get_Type) {
            case digger:
                renderer.addTexture(digger_texture, MouseRect, button_angle);
                break;
            case translater:
                if(Get_Rotation ==  r_middle) renderer.addTexture(translater_texture, MouseRect, button_angle);
                if(Get_Rotation ==  r_left) renderer.addTexture(left_translater_texture, MouseRect, button_angle);
                if(Get_Rotation ==  r_right) renderer.addTexture(right_translater_texture, MouseRect, button_angle);
                break;
            case cutter:
                renderer.addTexture(cutter_left_texture, MouseRect, button_angle);
                switch (Get_Direction) {
                    case north: MouseRect.x += BlockSize; break;
                    case south: MouseRect.x -= BlockSize; break;
                    case east: MouseRect.y += BlockSize; break;
                    case west: MouseRect.y -= BlockSize; break;
                }
                renderer.addTexture(cutter_right_texture, MouseRect, button_angle);
                break;
            case bin:
                renderer.addTexture(bin_texture, MouseRect,button_angle);
                break;
            case rotator:
                renderer.addTexture(rotator_texture, MouseRect, button_angle);
            break;
        default:
            break;
        }
    }
}

void Context::draw_button(vector<SDL_Rect> buttons) {
    renderer.addTexture(digger_texture,buttons[0]);
    renderer.addTexture(translater_texture,buttons[1]);
    renderer.addTexture(cutter_texture,buttons[2]);
    renderer.addTexture(bin_texture,buttons[3]);
    renderer.addTexture(rotator_texture,buttons[4]);
    renderer.addTexture(back_texture,buttons[5]);
    renderer.addTexture(save_texture,buttons[6]);
}

void Context::drawString(string s, SDL_Rect Rect, SDL_Color color) {
    SDL_Surface*  scoreSurface = TTF_RenderUTF8_Blended(font,s.c_str(), color);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer.renderer_,scoreSurface);
    renderer.addTexture(scoreTexture,Rect);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
}

void Context::drawScore(Shape_type needtype, int neednum, int mynum, int money) {
    SDL_Rect ScoreRect;
    ScoreRect.w = BlockSize* 6.2;
    ScoreRect.h =BlockSize * 0.5;
    ScoreRect.x = (Mapw - 8.5) * BlockSize;
    ScoreRect.y = (Maph + 0.3) * BlockSize;
    SDL_Color color {0, 0, 0, 188};
    drawString("Need:         Num:"+to_string(mynum)+"/"+to_string(neednum) +  "   money:"+to_string(money), ScoreRect, color);

    ScoreRect.w = BlockSize* 0.9;
    ScoreRect.h =BlockSize * 0.9;
    ScoreRect.x = (Mapw - 7.3) * BlockSize;
    ScoreRect.y = (Maph + 0.05) * BlockSize;
    switch (needtype) {
        case circle:
            renderer.addTexture(circle_texture,ScoreRect);
            break;
        case triangle:
            renderer.addTexture(triangle_texture,ScoreRect);
            break;
        case half_circle:
            if(My_level == 3) renderer.addTexture(half_circle_texture,ScoreRect);
            else if (My_level == 4)  renderer.addTexture(half_circle_texture,ScoreRect, 180);
            break;
        default:break;
    }
}

void Context::draw_betweenLevel(vector<SDL_Rect> buff) {
    //renderer.SetColor(SDL_Color{192,192,192,255});
    renderer.SetColor(SDL_Color{255, 255, 224, 255});
    renderer.FillRect(buff[0]);
    renderer.FillRect(buff[1]);
    renderer.FillRect(buff[2]);

    SDL_Color color {64,64,64,255};
    SDL_Rect txtRect;
    txtRect = {BlockSize*(Mapw/2-1), (int)(BlockSize*((double)Maph/2-3.35)), BlockSize*4, (int)(BlockSize * 0.7)};
    drawString("digger's speed ++",txtRect,color);
    txtRect = {int(BlockSize*(Mapw/2-1.25)), (int)(BlockSize*((double)Maph/2-0.35)), (int)(BlockSize*4.5), (int)(BlockSize * 0.7)};
    drawString("translator's speed ++",txtRect,color);
    txtRect = {BlockSize*(Mapw/2-1), (int)(BlockSize*((double)Maph/2+2.65)), BlockSize*4, (int)(BlockSize * 0.7)};
    drawString("cutter's speed ++",txtRect,color);
}

void Context::draw_beginning(vector<SDL_Rect> begs) {
    renderer.Clear();

    renderer.SetColor({211, 211, 211, 255});
    renderer.FillRect({0,0,Mapw*BlockSize, (Maph+1) * BlockSize});
    renderer.SetColor({200, 230, 245, 150} );
    renderer.FillRect(begs[0]);
    renderer.FillRect(begs[1]);
    renderer.addTexture(newgame_texture,begs[0]);
    renderer.addTexture(continuegame_texture,begs[1]);
    renderer.addTexture(biggermap_texture,begs[2]);
    renderer.addTexture(biggercenter_texture,begs[3]);
    renderer.addTexture(increasevalue_texture,begs[4]);
    renderer.addTexture(clear_texture,begs[5]);


    SDL_Color color {64,64,64,255};
    SDL_Rect ScoreRect;
    ScoreRect.w = BlockSize* 5;
    ScoreRect.h =BlockSize * 0.5;
    ScoreRect.x = (Mapw - 8.5) * BlockSize;
    ScoreRect.y = (Maph + 0.3) * BlockSize;
    drawString("My money is:" + to_string(My_money), ScoreRect, color);
}

void Context::set_window_size(int w, int h) {
    window.Set_window_size(w, h);
}

