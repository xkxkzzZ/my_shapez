cmake_minimum_required(VERSION 3.27)
project(Shape6)

set(CMAKE_CXX_STANDARD 17)

include_directories(D:\\sdl\\SDL2-MINGW\\x86_64-w64-mingw32\\include)
link_directories(D:\\sdl\\SDL2-MINGW\\x86_64-w64-mingw32\\lib)

add_executable(Shape6 main.cpp
        Model.cpp
        Model.h
        Game.cpp
        Game.h
        Context.cpp
        Context.h
        Renderer.cpp
        Renderer.h
        window.cpp
        window.h
)

target_link_libraries(Shape6 SDL2 SDL2main SDL2_image SDL2_ttf)
