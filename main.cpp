#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstdint>
#include <cassert>


const int WINDOW_W = 800;
const int WINDOW_H = 400;

const int GAMEAREA_W = WINDOW_W * 0.9f;
const int GAMEAREA_H = WINDOW_H * 0.75f;
const int GAMEAREA_X = (WINDOW_W - GAMEAREA_W) / 2;
const int GAMEAREA_Y = (WINDOW_H - GAMEAREA_H) / 2;

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

int TTF_ErrorCheck(int code) {
    if(code != 0) {
        printf("SDL TTF error: %s\n", TTF_GetError());
        abort();
    }

    return code;
}

template<typename T>
T *TTF_ErrorCheck(T *ptr) {
    if(ptr == nullptr) {
        printf("SDL TTF error: %s\n", TTF_GetError());
        abort();
    }

    return ptr;
}

void draw_colored_rectangle(SDL_Renderer *renderer, SDL_Rect rect, uint32_t color) {

    // R G B A
    Uint8 r, g, b, a;
    SDL_ErrorCheck(SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a));
    
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 
        (color & 0xFF000000) >> 24, 
        (color & 0x00FF0000) >> 16, 
        (color & 0x0000FF00) >> 8, 
        (color & 0x000000FF)));

    SDL_ErrorCheck(SDL_RenderDrawRect(renderer, &rect));
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, r, g, b, a));
}

int main(int argc, char** argv) {
    printf("Initializing SDL...\n");
  
    const int PLAYER_W = 25;
    assert((PLAYER_W * 2) < GAMEAREA_W);
    
    const int PLAYER_H = 100;
    assert(PLAYER_H < GAMEAREA_H);

    const int PLAYER_OFFSET_X = 5;
    const int PLAYER_X = GAMEAREA_X + PLAYER_OFFSET_X;
    assert(PLAYER_X > GAMEAREA_X && PLAYER_X < (GAMEAREA_W + GAMEAREA_X));

    const int PLAYER_Y = (GAMEAREA_H) / 2;
    assert(PLAYER_Y > GAMEAREA_Y && PLAYER_Y < (GAMEAREA_H + GAMEAREA_Y));

    SDL_ErrorCheck(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window *window = SDL_ErrorCheck(SDL_CreateWindow("cppong", 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          WINDOW_W, WINDOW_H, 
                                                          SDL_WINDOW_RESIZABLE));
  
    SDL_Renderer *renderer = SDL_ErrorCheck(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    TTF_ErrorCheck(TTF_Init());    
    TTF_Font *main_font = TTF_ErrorCheck(TTF_OpenFont("uni0553-webfont.ttf", 32));
    SDL_Surface *font_surface = TTF_ErrorCheck(TTF_RenderText_Blended(main_font, "cppong++", {255, 0, 255}));
    SDL_Texture *font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    
    SDL_FreeSurface(font_surface);

    int font_width, font_height;
    SDL_QueryTexture(font_texture, nullptr, nullptr, &font_width, &font_height);
    SDL_Rect font_rect = { (WINDOW_W / 2) - (font_width / 2), 0, font_width, font_height};
    bool quit = false;         
    SDL_Rect world = {GAMEAREA_X, GAMEAREA_Y, GAMEAREA_W, GAMEAREA_H};
    SDL_Rect player = {PLAYER_X, PLAYER_Y, PLAYER_W, PLAYER_H};
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

        if(player.y + dy >= GAMEAREA_Y && (player.y + dy + player.h) < GAMEAREA_Y + GAMEAREA_H)
            player.y += dy;

        SDL_ErrorCheck(SDL_RenderClear(renderer));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF));
        draw_colored_rectangle(renderer, player, 0x0000FFFF);
        draw_colored_rectangle(renderer, world, 0xFF0000FF);
        
        SDL_RenderCopy(renderer, font_texture, nullptr, &font_rect);
        SDL_RenderPresent(renderer);
        
        const Uint32 dt = SDL_GetTicks() - begin;
        prev_dt = dt;
        
    }

    printf("Exiting gracefully... :)\n");
    SDL_DestroyWindow(window);
    TTF_CloseFont(main_font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
