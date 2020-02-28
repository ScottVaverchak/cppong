#include <SDL2/SDL.h>
#include <cstdio>

int main(int argc, char** argv) {
  printf("Initializing SDL...\n");
  
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init error: %s\n",SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("cppong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Surface *surface;

  if(window == nullptr) {
    printf("SDL_CreateWindow error: %s\n",SDL_GetError());
    return 1;
  }

  bool quit = false;                                      
  SDL_Event e;                                            
  while (!quit) {                                         
    while (SDL_PollEvent(&e)) {                         
        if (e.type == SDL_QUIT) {                       
            quit = true;
        }                                               
    }

    surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);                                                   
  }

  printf("Exiting gracefully... :)\n");
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
