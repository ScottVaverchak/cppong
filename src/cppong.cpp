#include <cstdio>
#include <cstdint>
#include <cassert>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cppong_error.cpp"
#include "cppong_vec.cpp"
#include "cppong_rect.cpp"
#include "cppong_debug_ui.cpp"
#include "cppong_font.cpp"
#include "cppong_entity.cpp"
#include "cppong_renderer.cpp"

const int WINDOW_W = 800;
const int WINDOW_H = 400;

const SDL_Rect GAMEAREA = {
    static_cast<int>((WINDOW_W - (WINDOW_W * 0.9f)) / 2),
    static_cast<int>((WINDOW_H - (WINDOW_H * 0.75f)) / 2),
    static_cast<int>(WINDOW_W * 0.9f),
    static_cast<int>(WINDOW_H * 0.75f)
};

const int BALL_RADIUS = 10;

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

int cppong_main() {
    printf("Initializing SDL...\n");
    const int PLAYER_OFFSET_X = 5;

    Entity player = {};
    player.pos = { (float)(GAMEAREA.x + PLAYER_OFFSET_X), GAMEAREA.h / 2.0f };
    player.hitbox = rect(player.pos, 24.0f, 60.0f);
    player.srcrect = { 8, 1, 12, 30 }; 

    Entity oppo = {};
    oppo.pos = { (float)(GAMEAREA.x + GAMEAREA.w) - PLAYER_OFFSET_X - 12 * 2, GAMEAREA.h / 2.0f };
    oppo.hitbox = rect(oppo.pos, 24.0f, 60.0f);
    oppo.srcrect = { 8, 1, 12, 30 }; 

    Entity ball = {};
    ball.pos = { GAMEAREA.x + (GAMEAREA.w / 2.0f), GAMEAREA.y + (GAMEAREA.h / 2.0f) };
    ball.srcrect = {0, 0, 16, 16};
    ball.vel = { 2, 4 };

    // TODO: are these asserts needed?
    assert((player.hitbox.w * 2) < GAMEAREA.w);
    assert(player.hitbox.h < GAMEAREA.h);    
    assert(player.pos.x > GAMEAREA.x && player.pos.x < (GAMEAREA.w + GAMEAREA.x));
    assert(player.pos.y > GAMEAREA.y && player.pos.y < (GAMEAREA.h + GAMEAREA.y));

    assert((oppo.hitbox.w * 2) < GAMEAREA.w);
    assert(oppo.hitbox.h < GAMEAREA.h);    
    assert(oppo.pos.x > GAMEAREA.x && oppo.pos.x < (GAMEAREA.w + GAMEAREA.x));
    assert(oppo.pos.y > GAMEAREA.y && oppo.pos.y < (GAMEAREA.h + GAMEAREA.y));

    SDL_ErrorCheck(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window *window = SDL_ErrorCheck(SDL_CreateWindow("cppong", 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          WINDOW_W, WINDOW_H, 
                                                          SDL_WINDOW_RESIZABLE));
  
    SDL_Renderer *renderer = SDL_ErrorCheck(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    TTF_ErrorCheck(TTF_Init());    

    FontCache *fc;
    init_font_cache(&fc, "assets/uni0553-webfont.ttf", renderer);

    SDL_Texture *paddle_texture = load_texture_from_file(renderer, "assets/RAM.png");
    SDL_Texture *spritesheet_texture = load_texture_from_file(renderer, "assets/spritesheet.png");

    bool quit = false;
    bool display_debug = false;

    SDL_Event e;
    Vec2f left_coll_pos = {};
    Vec2f right_coll_pos = {};

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

        if(player.pos.y + dy >= GAMEAREA.y && (player.pos.y + dy + player.hitbox.h) < GAMEAREA.y + GAMEAREA.h)
        {
            player.pos.y += dy * 2;
            oppo.pos.y += dy * 2;
        }

        ball.pos += ball.vel;

        if((ball.pos.x + BALL_RADIUS) > GAMEAREA.x + GAMEAREA.w || (ball.pos.x - BALL_RADIUS) < GAMEAREA.x)
            ball.vel *= {-1, 1};

        if((ball.pos.y + BALL_RADIUS) > GAMEAREA.y + GAMEAREA.h || (ball.pos.y - BALL_RADIUS) < GAMEAREA.y)
            ball.vel *= {1, -1};

        Rectf play_rectm = rect(player.pos, player.hitbox.w, player.hitbox.h);
        Rectf oppo_rectm = rect(oppo.pos, oppo.hitbox.w, oppo.hitbox.h);

        auto ball_dia = static_cast<float>(BALL_RADIUS * 2.0f);
        Rectf ball_rectm = rect(ball.pos - static_cast<float>(BALL_RADIUS), ball_dia, ball_dia);


        CollisionRecord left_paddle_coll = {};
        if(paddle_ball_collision(play_rectm, BALL_RADIUS, ball.pos, &left_paddle_coll)) {
            left_coll_pos.x = play_rectm.x + 24;
            left_coll_pos.y = (left_paddle_coll.world_position.y - play_rectm.y);

            ball.vel *= {-1, 1};
        }
            

        CollisionRecord right_paddle_coll = {};
        if(paddle_ball_collision(oppo_rectm, BALL_RADIUS, ball.pos, &right_paddle_coll)) {
            right_coll_pos.x = oppo_rectm.x;
            right_coll_pos.y = (right_paddle_coll.world_position.y - oppo_rectm.y);
            ball.vel *= {-1, 1};
        }
            

        SDL_ErrorCheck(SDL_RenderClear(renderer));
        SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF));
        
        SDL_Rect play_rect = rect_to_sdl(play_rectm);
        SDL_Rect oppo_rect = rect_to_sdl(oppo_rectm);
        SDL_Rect ball_rect = rect_to_sdl(ball_rectm);
        
        SDL_ErrorCheck(SDL_RenderCopy(renderer, paddle_texture, &player.srcrect, &play_rect));
        SDL_ErrorCheck(SDL_RenderCopyEx(renderer, paddle_texture, 
                                        &oppo.srcrect, &oppo_rect, 0, 
                                        nullptr, SDL_FLIP_HORIZONTAL));
        
        SDL_ErrorCheck(SDL_RenderCopy(renderer, spritesheet_texture, &ball.srcrect, &ball_rect));

        render_text(fc, renderer, "cppong++", 32, {(WINDOW_W / 2), 0});
        render_text(fc, renderer, "so much game",48, { GAMEAREA.x, GAMEAREA.h + 48});

        if(display_debug) {
            draw_colored_rectangle(renderer, play_rectm, 0x0000FFFF);
            draw_colored_rectangle(renderer, oppo_rectm, 0xFFFFFFFF);
            draw_colored_rectangle(renderer, sdl_to_rect<float>(GAMEAREA), 0xFF0000FF);
            draw_colored_circle(renderer, ball.pos, BALL_RADIUS, 0xFF00FFFF);
            draw_colored_circle(renderer, { left_coll_pos.x, left_coll_pos.y + play_rectm.y}, 6, 0xFF00FFFF);
            draw_colored_circle(renderer, { right_coll_pos.x, right_coll_pos.y + oppo_rectm.y}, 6, 0xFF00FFFF);
        
        }

        SDL_RenderPresent(renderer);
        
        const Uint32 dt = SDL_GetTicks() - begin;
        prev_dt = dt;
        
    }

    printf("Exiting gracefully... :)\n");
    
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
