

#include "Game.h"

#include <fstream>
#include <sstream>

Game::Game() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_Log("sdl init failed");
    }
    TTF_Init();
    Context::Init();
    Game_init();
}

Game::~Game() {
    end_game();
    SDL_Quit();
}

void Game::Game_init() {
    My_level = 1;
    game_state = beginning;
    machine_map.resize(Mapw);
    for (auto&it: machine_map) {
        it.resize(Maph);
    }
    background.resize(Mapw);
    for (auto&it: background) {
        it.resize(Maph);
    }
    Size_Rect();
    Load_General_buf_in();
}

void Game::begin_new_game() {

    My_level = 1;
    My_num = 0;
    clear_machine();
    Load_General_buf_in();
    Load_background();


}

void Game::continue_old_game() {
    Load_background();
    Load_General_buf_in();
    Load_in();
}



void Game::Size_Rect() {
    MapRect = {0,0,Mapw*BlockSize, Maph*BlockSize};
    MouseRect = {0,0,BlockSize,BlockSize};

    button_digger = {2*BlockSize,Maph*BlockSize,BlockSize,BlockSize};
    button_translater = {3*BlockSize,Maph*BlockSize,BlockSize,BlockSize};
    button_cutter = {4*BlockSize,Maph*BlockSize,2*BlockSize,BlockSize};
    button_bin = {6*BlockSize,Maph*BlockSize,BlockSize,BlockSize};
    button_rotator = {7*BlockSize,Maph*BlockSize,BlockSize,BlockSize};
    button_back = {0,Maph*BlockSize,BlockSize*2,BlockSize};
    button_save = {(Mapw-2)*BlockSize,Maph*BlockSize,BlockSize*2,BlockSize};

    buf_digger = {int(BlockSize*(Mapw / 2 - 2)), BlockSize*(Maph /2 - 4), BlockSize*6, BlockSize * 2};
    buf_translater = {int(BlockSize*(Mapw / 2 - 2)), BlockSize*(Maph /2 - 1), BlockSize*6, BlockSize * 2};
    buf_cutter = {int(BlockSize*(Mapw / 2 - 2)), BlockSize*(Maph /2 +2), BlockSize*6, BlockSize * 2};

    new_game = {BlockSize*(Mapw / 2 - 3), BlockSize*(Maph /2 - 4), BlockSize*6, BlockSize * 2};
    continue_game = {BlockSize*(Mapw / 2 - 3), BlockSize*(Maph /2 - 1), BlockSize*6, BlockSize * 2};
    bigger_map = {BlockSize*(Mapw / 2 - 6), BlockSize*(Maph /2 +2), BlockSize*6, (int)(BlockSize * 1.5)};
    bigger_center = {BlockSize*(Mapw / 2 +2), BlockSize*(Maph /2 +2), BlockSize*6, (int)(BlockSize * 1.5)};
    increase_value = {BlockSize*(Mapw / 2 -6), BlockSize*(Maph /2 +4), BlockSize*7, (int)(BlockSize * 1.5)};
    button_clear = {BlockSize*(Mapw / 2 +4), BlockSize*(Maph /2 +4), BlockSize*4, (int)(BlockSize * 1.5)};
}



void Game::end_game() {
    Load_out();
    Load_General_buf_out();
    clear_machine();
}

void Game::clear_machine() {
    for (int i = 0; i < Mapw; i++) {
        for (int j = 0; j < Maph; j++) {
            if (machine_map[i][j]) {
                delete machine_map[i][j];
                machine_map[i][j] = nullptr;
            }
        }
    }
}


bool Game::check_mouse_rect(SDL_Rect r) {
    return r.x <= mouseX && mouseX <= r.x + r.w && r.y <= mouseY && mouseY <= r.y + r.h;
}

void Game::main_loop() {
    for (auto&it: machine_map) {
        for (auto&machine: it) {
            if (machine) {
                machine->work();
            }
        }
    }
    next_level();
}

void Game::draw_gaming() {
    Context::Inst().drawBackground(Mapw, Maph);
    Context::Inst().drawScore(My_center->Get_needtype(My_level), My_center->Get_neednum(My_level), My_num, My_money);
    Context::Inst().draw_mouse(Is_Get, Get_Type, Get_Rotation, Get_Direction, MouseRect, button_angle);
    Context::Inst().draw_button({button_digger, button_translater, button_cutter, button_bin,button_rotator,button_back,button_save});
}

void Game::run() {
    while (!isquit) {
        message_handling();
        if(game_state == gaming) {
            if (auto_run) {
                uint64_t curr_time = SDL_GetTicks64();
                if (curr_time - last_time > 50) {
                    main_loop();
                    draw_gaming();
                    last_time = curr_time;
                }
            }
        }
        if(game_state==between_level) {
            Context::Inst().draw_betweenLevel({buf_digger, buf_translater, buf_cutter});
        }
        if(game_state == beginning) {
            Context::Inst().draw_beginning({new_game, continue_game, bigger_map, bigger_center, increase_value, button_clear});
        }
        //Load_out();
        Context::Inst().renderer.Present();
    }
}


void Game::message_handling() {
    while (SDL_PollEvent(&event)) {
        if(game_state == beginning) {
            message_handling_beginning();
        }
        else if (game_state == gaming) {
            message_handling_gaming();
        }
        else if(game_state == between_level) {
            message_handling_between_level();
        }
    }
}



void Game::message_handling_gaming() {
    switch (event.type) {
            case SDL_QUIT:
                isquit = true;
                break;
            case SDL_MOUSEBUTTONDOWN: {
                Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
                //SDL_Log("Mouse coordinates: x=%d, y=%d", mouseX, mouseY);
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (check_mouse_rect(MapRect)) {
                        if (!Is_Get) SDL_Log("Nothing to put");
                        else {
                            int x = mouseX / BlockSize, y = mouseY / BlockSize;
                            //if (machine_map[x][y]) SDL_Log("Cant put Here!");
                            Machine* m = machine_map[x][y];
                            if (m && m->type != center) {
                                if(m->type == cutter) {
                                    delete machine_map[m->x2][m->y2];
                                    machine_map[m->x2][m->y2] = nullptr;
                                }
                                else if(m->type == cutter_righthalf) {
                                    delete machine_map[m->x1][m->y1];
                                    machine_map[m->x1][m->y1] = nullptr;
                                }
                                delete machine_map[x][y];
                                machine_map[x][y] = nullptr;
                            }
                            //else {
                            switch (Get_Type) {
                                case digger:
                                    machine_map[x][y] = new Digger(Get_Direction, x, y);
                                    break;
                                case translater:
                                    machine_map[x][y] = new Translater(Get_Direction, Get_Rotation, x, y);
                                    Get_Rotation=r_middle;
                                    break;
                                case cutter: {
                                    Machine* c = new Cutter(Get_Direction, x, y);
                                    machine_map[x][y] = c;
                                    if(machine_map[c->x2][c->y2]) {
                                        delete machine_map[c->x2][c->y2];
                                        machine_map[c->x2][c->y2] = nullptr;
                                    }
                                    machine_map[c->x2][c->y2] = new Cutter_righthalf(Get_Direction, c->x2, c->y2);
                                    break;
                                }
                                case bin:
                                    machine_map[x][y] = new Bin(Get_Direction, x, y);
                                break;
                                case rotator:
                                    machine_map[x][y] = new Rotator(Get_Direction, x, y);
                                break;

                            default:
                                break;
                            }
                            //}
                            Is_Get = false;
                        }
                    }
                    else if (check_mouse_rect(button_digger)) {
                        Is_Get = true;
                        Get_Type = digger;
                    }
                    else if (check_mouse_rect(button_translater)) {
                        Is_Get = true;
                        Get_Type = translater;
                    }
                    else if (check_mouse_rect(button_cutter)) {
                        Is_Get = true;
                        Get_Type = cutter;
                    }
                    else if (check_mouse_rect(button_bin)) {
                        Is_Get = true;
                        Get_Type = bin;
                    }
                    else if (check_mouse_rect(button_rotator)) {
                        Is_Get = true;
                        Get_Type = rotator;
                    }
                    else if (check_mouse_rect(button_back)) {
                        game_state = beginning;
                    }
                    else if (check_mouse_rect(button_save)) {
                        Load_out();
                    }

                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (check_mouse_rect(MapRect)) {
                        int x = mouseX / BlockSize, y = mouseY / BlockSize;
                        Machine* m = machine_map[x][y];
                        if (m && m->type != center) {
                            if(m->type == cutter) {
                                delete machine_map[m->x2][m->y2];
                                machine_map[m->x2][m->y2] = nullptr;
                            }
                            if(m->type == cutter_righthalf) {
                                delete machine_map[m->x1][m->y1];
                                machine_map[m->x1][m->y1] = nullptr;
                            }
                            delete machine_map[x][y];
                            machine_map[x][y] = nullptr;
                        }
                        else SDL_Log("Nothing to delete");
                    }
                }
                break;
            }

            case SDL_MOUSEMOTION: {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                MouseRect.x = mouseX - MouseRect.w / 2;
                MouseRect.y = mouseY - MouseRect.h / 2;
                break;
            }

            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_w:
                    Get_Direction = north;
                    button_angle = 0;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    Get_Direction = south;
                    button_angle = 180;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    Get_Direction = west;
                    button_angle = 270;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    Get_Direction = east;
                    button_angle = 90;
                    break;
                case SDLK_k:
                    Get_Rotation = Rotation::r_middle;
                    break;
                case SDLK_j:
                    Get_Rotation = Rotation::r_left;
                    break;
                case SDLK_l:
                    Get_Rotation = Rotation::r_right;
                    break;
                case SDLK_r:
                    main_loop();
                    break;
                case SDLK_t:
                    auto_run = !auto_run;
                    SDL_Log("auto run is : %d", auto_run);
                    break;
                case SDLK_o:
                    Load_out();
                    SDL_Log("Loading out ... !");
                    break;
                case SDLK_i:
                    Load_in();
                    SDL_Log("Loading in ... !");
                    break;
                case SDLK_b:
                    Load_background();
                    SDL_Log("Loading Background ... !");
                    break;

                default:
                    break;
                }
                break;
            }

            default:
                break;
            }
}



void Game::message_handling_between_level() {
    switch (event.type) {
        case SDL_QUIT:
            isquit = true;
        break;
        case SDL_MOUSEMOTION: {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            if(check_mouse_rect(buf_digger)) {
                SDL_Log("Digger speed ++");
                Digger::speed ++;
                for(auto it: machine_map) {
                    for(auto m: it) {
                        if(m)
                            if(m->type==digger) m->timer.interval -= 25;
                    }
                }
                game_state = gaming;
            }
            if(check_mouse_rect(buf_translater)) {
                SDL_Log("Translator speed ++");
                Translater::speed ++;
                for(auto it: machine_map) {
                    for(auto m: it) {
                        if(m)
                            if(m->type==translater) m->timer.interval -= 7;
                    }
                }
                game_state = gaming;
            }
            if(check_mouse_rect(buf_cutter)) {
                SDL_Log("Cutter speed ++");
                Cutter::speed ++;
                for(auto it: machine_map) {
                    for(auto m: it) {
                        if(m)
                            if(m->type==cutter) m->timer.interval -= 15;
                    }
                }
                game_state = gaming;
            }
        }
        break;
    }
}



void Game::message_handling_beginning() {
    switch (event.type) {
        case SDL_QUIT:
            isquit = true;
        break;
        case SDL_MOUSEMOTION: {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            if(check_mouse_rect(continue_game)) {
                continue_old_game();
                game_state = gaming;
            }
            else if(check_mouse_rect(new_game)) {
                begin_new_game();
                game_state = gaming;
            }
            else if(check_mouse_rect(bigger_map)) {
                if(My_money <50) SDL_Log("No enough money!");
                else if(Have_Bigged_Map) SDL_Log("Map is the biggest!");
                else {
                    Bigger_map();
                    Have_Bigged_Map = true;
                    SDL_Log("Bigger Map  ...  !");
                    My_money -= 50;
                }
            }
            else if(check_mouse_rect(bigger_center)) {
                if(My_money < 30) SDL_Log("No enough money!");
                else if (Have_Bigged_Center) SDL_Log("Center is the biggest!");
                else {
                    Bigger_center();
                    Have_Bigged_Center = true;
                    My_money -= 30;
                    SDL_Log("Bigger Center ... !");
                }
            }
            else if(check_mouse_rect(increase_value)) {
                if(My_money >= 20) {
                    Shape::increasing_value ++;
                    SDL_Log("Increased Value successfully.. %d !", Shape::increasing_value);
                    My_money -= 20;
                }
                else SDL_Log("No enough money!");
            }
            else if(check_mouse_rect(button_clear)) {
                Clear_buf();
                SDL_Log("Clear All General buf ... !");
            }
            break;
        }
        default:break;
    }
}

void Game::Load_background() {
    ifstream input;
    if(!Have_Bigged_Map) input.open ("Loading\\LoadBackground.txt");
    else input.open("Loading\\LoadBackground2.txt");
    if (!input.is_open()) SDL_Log("Input Load failed");
    else {
        string Line;
        int i = 0, j = 0, type;
        while (getline(input, Line)) {
            istringstream iss(Line);
            while (iss >> type) {
                background[j][i] = (Background_type)type;
                if (background[j][i] == Background_type::center_mine)
                    machine_map[j][i] = new Center(north, j, i);
                j++;
            }
            j = 0;
            i++;
        }
    }
    input.close();
}



void Game::Load_out() {
    ofstream output("Loading\\LoadMap1.txt");
    if (!output.is_open()) SDL_Log("Output Load failed");
    else {
        for (int i = 0; i < Mapw; i++) {
            for (int j = 0; j < Maph; j++) {
                if (machine_map[i][j]) {
                    output << i << " " << j << " " << machine_map[i][j]->type << " "
                            << machine_map[i][j]->direction << " " << machine_map[i][j]->rotation << endl;
                }
            }
        }
    }
    output.close();
    ofstream output_buf("Loading\\LoadBuf1.txt");
    if (!output_buf.is_open()) SDL_Log("Output Load failed");
    else {
        output_buf << My_level << " " << My_num << " " << Digger::speed << " " << Translater::speed << " " <<
                Cutter::speed << endl;
    }
    output.close();
}


void Game::Load_in() {
    ifstream input("Loading\\LoadMap1.txt");
    if (!input.is_open()) SDL_Log("Input Load failed");
    else {
        string Line;
        while (getline(input, Line)) {
            int x, y, type, d, r;
            istringstream iss(Line);
            if (iss >> x >> y >> type >> d >> r) {
                if(machine_map[x][y] && machine_map[x][y]->type == center) continue;
                switch ((Machine_type)type) {
                    case digger:
                        machine_map[x][y] = new Digger((Direction)d, x, y);
                        break;
                    case cutter:
                        machine_map[x][y] = new Cutter((Direction)d, x, y);
                        break;
                    case cutter_righthalf:
                        machine_map[x][y] = new Cutter_righthalf((Direction)d, x, y);
                        break;
                    case translater:
                        machine_map[x][y] = new Translater((Direction)d, (Rotation)r, x, y);
                        break;
                    case bin:
                        machine_map[x][y] = new Bin((Direction)d, x, y);
                        break;
                    case rotator:
                        machine_map[x][y] = new Rotator((Direction)d, x, y);
                }
            }
        }
    }
    input.close();
    ifstream input_buf("Loading\\LoadBuf1.txt");
    if (!input_buf.is_open()) SDL_Log("Input Load failed");
    else {
        input_buf >> My_level >> My_num >> Digger::speed >> Translater::speed >> Cutter::speed;
    }
    input_buf.close();
}

void Game::Load_General_buf_out() {
    ofstream output("Loading\\generalbuf.txt");
    if (!output.is_open()) SDL_Log("Output Load failed");
    output<<My_money<<" "<<Have_Bigged_Map<<" "<<Have_Bigged_Center<<" "<<Shape::increasing_value;
    output.close();
}

void Game::Load_General_buf_in() {
    ifstream input("Loading\\generalbuf.txt");
    if (!input.is_open()) SDL_Log("Input Load failed");
    input>>My_money>>Have_Bigged_Map>>Have_Bigged_Center>>Shape::increasing_value;
    input.close();
    if(Have_Bigged_Map) Bigger_map();
    if(Have_Bigged_Center) Bigger_center();
}

void Game::next_level() {
    if (My_center->Get_neednum(My_level) == My_num && My_level < 4) {
        My_level++;
        game_state = between_level;
        My_num = 0;
    }
}

void Game::Clear_buf() {
    Smaller_center();
    Smaller_map();
    My_money = Have_Bigged_Center = Have_Bigged_Map = Shape::increasing_value = 0;
    Load_General_buf_out();
}

void Game::Bigger_map() {
    if(!Have_Bigged_Map) {
        Load_out();
        Mapw += 5;
        Maph += 5;
        Context::Inst().set_window_size(Mapw * BlockSize, (Maph+1) * BlockSize);
        machine_map.resize(Mapw);
        for (auto&it: machine_map) {
            it.resize(Maph);
        }
        background.resize(Mapw);
        for (auto&it: background) {
            it.resize(Maph);
        }
        Size_Rect();
        Have_Bigged_Map = true;
        Load_background();
        Load_in();
    }
    else SDL_Log("Map is the biggest");
}

void Game::Smaller_map() {
    if(Have_Bigged_Map) {
        Load_out();
        Mapw -= 5;
        Maph -= 5;
        Context::Inst().set_window_size(Mapw * BlockSize, (Maph+1) * BlockSize);
        machine_map.resize(Mapw);
        for (auto&it: machine_map) {
            it.resize(Maph);
        }
        background.resize(Mapw);
        for (auto&it: background) {
            it.resize(Maph);
        }
        Size_Rect();
        Have_Bigged_Map = false;
        Load_background();
        Load_in();
    }
}

void Game::Bigger_center() {
    if(!Have_Bigged_Center) {
        for(int i=8; i<=10; i++) {
            for(int j=5; j<=7; j++) {
                if(machine_map[i][j]) {
                    delete machine_map[i][j];
                }
                machine_map[i][j] = new Center(north, i, j);
                background[i][j] = Background_type::center_mine;
            }
        }
        Have_Bigged_Center = true;
    }
    else SDL_Log("Center is the biggest");
}

void Game::Smaller_center() {
    if(Have_Bigged_Center) {
        for(int i=8; i<=10; i++) {
            for(int j=5; j<=7; j++) {
                if(machine_map[i][j]) {
                    delete machine_map[i][j];
                    machine_map[i][j] = nullptr;
                }
                background[i][j] = Background_type::empty;
            }
        }
        Have_Bigged_Center = false;
    }
}
