#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdint>

const int WINDOW_W = 800;
const int WINDOW_H = 400;
const int PLAYER_W = 25;
const int PLAYER_H = 100;
const int PLAYER_X = 10;
const int GAMEAREA_W = WINDOW_W;
const int GAMEAREA_H = WINDOW_H;
const int GAMEAREA_X = 0;
const int GAMEAREA_Y = 0;

int SDL_ErrorCheck(int code) {
    if(code != 0) {
        printf("SDL error: %s\n", SDL_GetError());
        abort();
    }

    return code;
}

template<typename T>
T *SDL_ErrorCheck(T *ptr) {
    if(ptr == nullptr) {
        printf("SDL error: %s\n", SDL_GetError());
        abort();
    }

    return ptr;
}

void draw_colored_rectangle(SDL_Renderer *renderer, SDL_Rect rect, uint32_t color) {

    // R G B A
    Uint8 r, g, b, a;
    SDL_ErrorCheck(SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a));
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, (color & 0xFF000000) >> 24, (color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, (color & 0x000000FF)));
    SDL_ErrorCheck(SDL_RenderDrawRect(renderer, &rect));
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, r, g, b, a));
}

int main(int argc, char** argv) {
    printf("Initializing SDL...\n");
  
    SDL_ErrorCheck(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window *window = SDL_ErrorCheck(SDL_CreateWindow("cppong", 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          WINDOW_W, WINDOW_H, 
                                                          SDL_WINDOW_RESIZABLE));
  
    SDL_Renderer *renderer = SDL_ErrorCheck(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");


    bool quit = false;         
    SDL_Rect world = {GAMEAREA_X, GAMEAREA_Y, GAMEAREA_W, GAMEAREA_H};
    SDL_Rect player = {PLAYER_X, 10, PLAYER_W, PLAYER_H};
    SDL_Event e;

    Uint32 prev_dt = 1;
    
    while (!quit) {
        Uint32 begin = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
        int8_t dy = 0;
        if(keyboard_state[SDL_SCANCODE_UP]) {
            dy = -1;
        } else if (keyboard_state[SDL_SCANCODE_DOWN]) {
            dy = 1;
        }

        if(player.y + dy >= 0 && (player.y + dy + player.h) < 400)
            player.y += dy;

        SDL_ErrorCheck(SDL_RenderClear(renderer));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF));
        draw_colored_rectangle(renderer, player, 0x0000FFFF);
        draw_colored_rectangle(renderer, world, 0xFF0000FF);

        SDL_RenderPresent(renderer);
        
        const Uint32 dt = SDL_GetTicks() - begin;
        prev_dt = dt;
        
    }

    printf("Exiting gracefully... :)\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
