#include <cstdio>
#include <cstdint>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "vec.cpp"

const int WINDOW_W = 800;
const int WINDOW_H = 400;

const int GAMEAREA_W = WINDOW_W * 0.9f;
const int GAMEAREA_H = WINDOW_H * 0.75f;
const int GAMEAREA_X = (WINDOW_W - GAMEAREA_W) / 2;
const int GAMEAREA_Y = (WINDOW_H - GAMEAREA_H) / 2;

const int BALL_SPAWN_X = GAMEAREA_X + (GAMEAREA_W / 2);
const int BALL_SPAWN_Y = GAMEAREA_Y + (GAMEAREA_H / 2);

const int BALL_RADIUS = 10;

const float PI = 3.141592f;

int SDL_ErrorCheck(int code) {
    if(code < 0) {
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

void draw_colored_circle(SDL_Renderer *renderer, Vec2i position, int radius, uint32_t color) {
    Uint8 r, g, b, a;
    SDL_ErrorCheck(SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a));
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 
            (color & 0xFF000000) >> 24, 
            (color & 0x00FF0000) >> 16, 
            (color & 0x0000FF00) >> 8, 
            (color & 0x000000FF)));

    const int segments = 8;
    assert(segments >= 3);

    const float segment_radians = (2.0f * PI) / segments;

    for(size_t i = 1; i <= segments; i++) {
        const auto x = (sinf(i * segment_radians) * radius) + position.x;
        const auto y = (cosf(i * segment_radians) * radius) + position.y;

        const auto nx = (sinf((i + 1) * segment_radians) * radius) + position.x;
        const auto ny = (cosf((i + 1) * segment_radians) * radius) + position.y;

        SDL_ErrorCheck(SDL_RenderDrawLine(renderer, x, y, nx, ny));
    }

    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, r, g, b, a));
}

struct Entity {
    Vec2i pos;
    Vec2i vel;

    SDL_Rect hitbox;
    SDL_Rect srcrect;
};

Vec2i ball_pos = { BALL_SPAWN_X, BALL_SPAWN_Y };

SDL_Texture *load_texture_from_file(SDL_Renderer *renderer, const char* filename) {

    int image_width, image_height, image_channels;
    unsigned char *image = stbi_load(filename, &image_width, &image_height, &image_channels, STBI_rgb_alpha);

    if(image == nullptr) {
        printf("Error loading: %s", filename);
        abort();
    }

    SDL_Surface *image_surface = SDL_ErrorCheck(SDL_CreateRGBSurfaceFrom(
        image, 
        image_width, 
        image_height, 
        32,
        image_width * 4,
        0x000000FF,
        0x0000FF00,
        0x00FF0000,
        0xFF000000
    ));

    SDL_Texture *texture = SDL_ErrorCheck(SDL_CreateTextureFromSurface(renderer, image_surface));
    
    stbi_image_free(image);
    
    return texture;
}

enum Rect_Side {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};

bool paddle_ball_collision(const SDL_Rect rect, const int radius, const Vec2i ball_position) {
    auto dx = ball_position.x - max(rect.x, min(ball_position.x, rect.x + rect.w));
    auto dy = ball_position.y - max(rect.y, min(ball_position.y, rect.y + rect.h));
    return (dx * dx + dy * dy) < (radius * radius);
}

int cppong_main() {
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

    Entity ball = {};
    ball.pos = { BALL_SPAWN_X, BALL_SPAWN_Y};
    ball.srcrect = {0, 0, 16, 16};
    ball.vel = { 1, 2 };

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
    TTF_Font *main_font = TTF_ErrorCheck(TTF_OpenFont("assets/uni0553-webfont.ttf", 32));
    SDL_Surface *font_surface = TTF_ErrorCheck(TTF_RenderText_Blended(main_font, "cppong++", {255, 0, 255}));
    SDL_Texture *font_texture = SDL_ErrorCheck(SDL_CreateTextureFromSurface(renderer, font_surface));
    
    SDL_FreeSurface(font_surface);

    int font_width, font_height;
    SDL_ErrorCheck(SDL_QueryTexture(font_texture, nullptr, nullptr, &font_width, &font_height));
    SDL_Rect font_rect = { (WINDOW_W / 2) - (font_width / 2), 0, font_width, font_height};

    SDL_Texture *paddle_texture = load_texture_from_file(renderer, "assets/RAM.png");
    SDL_Texture *spritesheet_texture = load_texture_from_file(renderer, "assets/spritesheet.png");

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
        {
            player.pos.y += dy * 2;
            oppo.pos.y += dy * 2;
        }

        ball.pos += ball.vel;

        if(ball.pos.x > GAMEAREA_X + GAMEAREA_W || ball.pos.x < GAMEAREA_X)
            ball.vel *= {-1, 1};
        if(ball.pos.y > GAMEAREA_Y + GAMEAREA_H || ball.pos.y < GAMEAREA_Y)
            ball.vel *= {1, -1};

        SDL_Rect play_rectm = {player.pos.x, player.pos.y, player.hitbox.w, player.hitbox.h };
        SDL_Rect oppo_rectm = {oppo.pos.x, oppo.pos.y, oppo.hitbox.w, oppo.hitbox.h };
        SDL_Rect ball_rectm = {ball.pos.x - BALL_RADIUS, ball.pos.y - BALL_RADIUS, BALL_RADIUS * 2, BALL_RADIUS * 2 };
                             
        if(paddle_ball_collision(play_rectm, BALL_RADIUS, ball.pos))
            ball.vel *= {-1, 1};

        if(paddle_ball_collision(oppo_rectm, BALL_RADIUS, ball.pos))
            ball.vel *= {-1, 1};

        SDL_ErrorCheck(SDL_RenderClear(renderer));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF));
        SDL_ErrorCheck(SDL_RenderCopy(renderer, paddle_texture, &player.srcrect, &play_rectm));
        SDL_ErrorCheck(SDL_RenderCopyEx(renderer, paddle_texture, 
                                        &oppo.srcrect, &oppo_rectm, 0, 
                                        nullptr, SDL_FLIP_HORIZONTAL));
        
        SDL_ErrorCheck(SDL_RenderCopy(renderer, spritesheet_texture, &ball.srcrect, &ball_rectm));

        SDL_RenderCopy(renderer, font_texture, nullptr, &font_rect);

        if(display_debug) {
            SDL_Rect play_pos = {player.pos.x, player.pos.y, 2, 2 };
            SDL_Rect oppo_pos = {oppo.pos.x, oppo.pos.y, 2, 2  };

            draw_colored_rectangle(renderer, play_rectm, 0x0000FFFF);
            draw_colored_rectangle(renderer, play_pos, 0xFF00FFFF);
            draw_colored_rectangle(renderer, oppo_rectm, 0xFFFFFFFF);
            draw_colored_rectangle(renderer, oppo_pos, 0x0000FFFF);
            draw_colored_rectangle(renderer, world, 0xFF0000FF);
            draw_colored_circle(renderer, ball.pos, BALL_RADIUS, 0xFF00FFFF);
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
