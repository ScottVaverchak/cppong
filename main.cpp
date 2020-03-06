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
    
    bool quit = false;         
    SDL_Rect rectm = {0, 0, 100, 100};
    SDL_Event e;                                            
    while (!quit) {                                         
        while (SDL_PollEvent(&e)) {                         
            if (e.type == SDL_QUIT) {                       
                quit = true;
            }                                               
        }

        SDL_ErrorCheck(SDL_RenderClear(renderer));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF));
        SDL_ErrorCheck(SDL_RenderDrawRect(renderer, &rectm));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF));

        SDL_RenderPresent(renderer);
    }

    printf("Exiting gracefully... :)\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
