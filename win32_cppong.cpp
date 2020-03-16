#define WIN_OS
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>
#include "cppong.cpp"

int main(int argc, char** argv) {
    SDL_SetMainReady();
    cppong_main();
    return 0;
}