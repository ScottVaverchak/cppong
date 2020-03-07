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
                                                          SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));

    if(window == nullptr) {
        printf("SDL_CreateWindow error: %s\n",SDL_GetError());
        return 1;
    }
  
    SDL_Renderer *renderer = SDL_ErrorCheck(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
    const int player_w = 25;
    const int player_h = 100;
    const int player_x = 10;

    bool quit = false;         
    SDL_Rect player = {player_x, 10, player_w, player_h};
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

        if(keyboard_state[SDL_SCANCODE_UP]) {
            player.y -= 1;
        } else if (keyboard_state[SDL_SCANCODE_DOWN]) {
            player.y += 1;
        }


        SDL_ErrorCheck(SDL_RenderClear(renderer));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF));
        SDL_ErrorCheck(SDL_RenderDrawRect(renderer, &player));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF));

        SDL_RenderPresent(renderer);

    }

    printf("Exiting gracefully... :)\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
