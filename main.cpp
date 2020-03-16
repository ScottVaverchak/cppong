#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <png.h>
#include <cstdio>
#include <cstdint>
#include <cassert>

#include "vec.cpp"

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

struct Entity {
    Vec2i pos;
    SDL_Rect hitbox;
    SDL_Rect srcrect;
};

int main(int argc, char** argv) {
    printf("Initializing SDL...\n");
    const int PLAYER_OFFSET_X = 5;

    Entity player = {};
    player.pos = { GAMEAREA_X + PLAYER_OFFSET_X, GAMEAREA_H / 2 };
    player.hitbox = {player.pos.x, player.pos.y, 12 * 2, 30 * 2 };
    player.srcrect = { 8, 1, 12, 30 }; 

    Entity oppo = {};
    oppo.pos = { (GAMEAREA_X + GAMEAREA_W) - PLAYER_OFFSET_X - 12 * 2, GAMEAREA_H / 2 };
    oppo.hitbox = {oppo.pos.x, oppo.pos.y, 12 * 2, 30 * 2 };
    oppo.srcrect = { 8, 1, 12, 30 }; 

    assert((player.hitbox.w * 2) < GAMEAREA_W);
    assert(player.hitbox.h < GAMEAREA_H);    
    assert(player.pos.x > GAMEAREA_X && player.pos.x < (GAMEAREA_W + GAMEAREA_X));
    assert(player.pos.y > GAMEAREA_Y && player.pos.y < (GAMEAREA_H + GAMEAREA_Y));

    assert((oppo.hitbox.w * 2) < GAMEAREA_W);
    assert(oppo.hitbox.h < GAMEAREA_H);    
    assert(oppo.pos.x > GAMEAREA_X && oppo.pos.x < (GAMEAREA_W + GAMEAREA_X));
    assert(oppo.pos.y > GAMEAREA_Y && oppo.pos.y < (GAMEAREA_H + GAMEAREA_Y));

    SDL_ErrorCheck(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window *window = SDL_ErrorCheck(SDL_CreateWindow("cppong", 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          WINDOW_W, WINDOW_H, 
                                                          SDL_WINDOW_RESIZABLE));
  
    SDL_Renderer *renderer = SDL_ErrorCheck(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    SDL_Rect world = {GAMEAREA_X, GAMEAREA_Y, GAMEAREA_W, GAMEAREA_H};

    // Font below
    TTF_ErrorCheck(TTF_Init());    
    TTF_Font *main_font = TTF_ErrorCheck(TTF_OpenFont("uni0553-webfont.ttf", 32));
    SDL_Surface *font_surface = TTF_ErrorCheck(TTF_RenderText_Blended(main_font, "cppong++", {255, 0, 255}));
    SDL_Texture *font_texture = SDL_ErrorCheck(SDL_CreateTextureFromSurface(renderer, font_surface));
    
    SDL_FreeSurface(font_surface);

    int font_width, font_height;
    SDL_ErrorCheck(SDL_QueryTexture(font_texture, nullptr, nullptr, &font_width, &font_height));
    SDL_Rect font_rect = { (WINDOW_W / 2) - (font_width / 2), 0, font_width, font_height};
    // Font fin.h

    // png fun begins here friend
    png_image image_friend;
    memset(&image_friend, 0, sizeof(png_image));

    image_friend.version = PNG_IMAGE_VERSION;
    if(!png_image_begin_read_from_file(&image_friend, "RAM.png")) {
        fprintf(stderr, "libpng errored reading RAM.png: %s", image_friend.message);
        abort();
    }

    image_friend.format = PNG_FORMAT_RGBA;

    uint32_t *pixel_friends = new uint32_t[image_friend.width * image_friend.height];
    
    if (!png_image_finish_read(&image_friend, nullptr, pixel_friends, 0, nullptr)) {
        fprintf(stderr, "libpng errored: %s", image_friend.message);
        abort();
    }

    SDL_Surface *image_surface = SDL_ErrorCheck(SDL_CreateRGBSurfaceFrom(
        pixel_friends, 
        image_friend.width, 
        image_friend.height, 
        32,
        image_friend.width * 4,
        0x000000FF,
        0x0000FF00,
        0x00FF0000,
        0xFF000000
    ));

    SDL_Texture *paddle_texture = SDL_ErrorCheck(SDL_CreateTextureFromSurface(renderer, image_surface));
    // png is big time done

    bool quit = false;
    bool display_debug = false;

    SDL_Event e;

    Uint32 prev_dt = 1;
    
    while (!quit) {
        Uint32 begin = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } 

            if (e.type == SDL_KEYUP) {
                switch(e.key.keysym.sym) {
                    case SDLK_d: {
                        display_debug = !display_debug;
                    } break;
                }
            }

        }

        const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
        int8_t dy = 0;
        if(keyboard_state[SDL_SCANCODE_UP]) {
            dy = -1;
        } else if (keyboard_state[SDL_SCANCODE_DOWN]) {
            dy = 1;
        }

        if(player.pos.y + dy >= GAMEAREA_Y && (player.pos.y + dy + player.hitbox.h) < GAMEAREA_Y + GAMEAREA_H)
            player.pos.y += dy;

        SDL_ErrorCheck(SDL_RenderClear(renderer));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF));

        SDL_Rect play_rectm = {player.pos.x, player.pos.y, player.hitbox.w, player.hitbox.h };
        SDL_Rect oppo_rectm = {oppo.pos.x, oppo.pos.y, oppo.hitbox.w, oppo.hitbox.h };

        SDL_ErrorCheck(SDL_RenderCopy(renderer, paddle_texture, &player.srcrect, &play_rectm));
        
        SDL_ErrorCheck(SDL_RenderCopyEx(renderer, paddle_texture, 
                                        &oppo.srcrect, &oppo_rectm, 0, 
                                        nullptr, SDL_FLIP_HORIZONTAL));
        
        SDL_RenderCopy(renderer, font_texture, nullptr, &font_rect);

        if(display_debug) {
            SDL_Rect play_pos = {player.pos.x, player.pos.y, 2, 2 };
            SDL_Rect oppo_pos = {oppo.pos.x, oppo.pos.y, 2, 2  };
            draw_colored_rectangle(renderer, play_rectm, 0x0000FFFF);
            draw_colored_rectangle(renderer, play_pos, 0xFF00FFFF);

            draw_colored_rectangle(renderer, oppo_rectm, 0xFFFFFFFF);
            draw_colored_rectangle(renderer, oppo_pos, 0x0000FFFF);

            draw_colored_rectangle(renderer, world, 0xFF0000FF);
            
        }

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
