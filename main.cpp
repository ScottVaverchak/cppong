#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdint>

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

int main(int argc, char** argv) {
    printf("Initializing SDL...\n");
  
    SDL_ErrorCheck(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window *window = SDL_ErrorCheck(SDL_CreateWindow("cppong", 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          400, 400, 
                                                          SDL_WINDOW_RESIZABLE));
  
    SDL_Renderer *renderer = SDL_ErrorCheck(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    const int PLAYER_W = 25;
    const int PLAYER_H = 100;
    const int PLAYER_X = 10;
    

    bool quit = false;         
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
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF));
        SDL_ErrorCheck(SDL_RenderDrawRect(renderer, &player));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF));
        
        SDL_RenderPresent(renderer);
        
        const Uint32 dt = SDL_GetTicks() - begin;
        prev_dt = dt;
        
    }

    printf("Exiting gracefully... :)\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
