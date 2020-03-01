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
  SDL_Surface *surface;

  if(window == nullptr) {
    printf("SDL_CreateWindow error: %s\n",SDL_GetError());
    return 1;
  }
  
  uint8_t col = 0x00;
  bool quit = false;         

  SDL_Event e;                                            
  while (!quit) {                                         
    while (SDL_PollEvent(&e)) {                         
        if (e.type == SDL_QUIT) {                       
            quit = true;
        }                                               
    }

    surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 0xFF, col, 0xFF));
    SDL_UpdateWindowSurface(window);
    col = ++col % 255;
  }

  printf("Exiting gracefully... :)\n");
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
