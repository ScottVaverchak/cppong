#include <cstdio>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cppong_error.cpp"
#include "cppong_vec.cpp"
#include "cppong_rect.cpp"
#include "cppong_debug_ui.cpp"
#include "cppong_font.cpp"
#include "cppong_entity.cpp"
#include "cppong_renderer.cpp"

int cppong_main() {
    printf("Initializing SDL...\n");
    World world = {};
    world.state = GameGameState::InGame;
    world.window_w = 800;
    world.window_h = 600;
    world.gamearea = {
        static_cast<int>((world.window_w - (world.window_w * 0.9f)) * 0.5f),
        static_cast<int>((world.window_h - (world.window_h * 0.75f)) * 0.5f),
        static_cast<int>(world.window_w * 0.9f),
        static_cast<int>(world.window_h * 0.75f)
    };

    const int PLAYER_OFFSET_X = 10;

    Paddle player = {};
    player.pos = { (float)(world.gamearea.x + PLAYER_OFFSET_X), world.gamearea.h * 0.5f };
    player.hitbox = rect(player.pos, 24.0f, 60.0f);
    player.srcrect = {0, 0, 16 * 2, 16 * 6};
    player.speed = 100.0f;
    player.w = 16.0f;
    player.h = 16.0f * 3.0f;
    player.forward = {1.0f, 0.0f};

    Paddle oppo = {};
    oppo.pos = { (float)(world.gamearea.x + world.gamearea.w) - PLAYER_OFFSET_X, world.gamearea.h * 0.5f };
    oppo.hitbox = rect(oppo.pos, 24.0f, 60.0f);
    oppo.srcrect = {0, 0, 16 * 2, 16 * 6};
    oppo.speed = 100.0f;
    oppo.w = 16.0f;
    oppo.h = 16.0f * 3.0f;
    oppo.forward = {-1.0f, 0.0f};

    Ball ball = {};
    ball.spawn_pos = { 
        world.gamearea.x + (world.gamearea.w * 0.5f), 
        world.gamearea.y + (world.gamearea.h * 0.5f) 
    };
    ball.pos = ball.spawn_pos;
    ball.srcrect = {16 * 2, 16 * 3, 16 * 2, 16 * 2};
    ball.vel = { 2, 0 };
    ball.w = ball.h = 16.0f;
    ball.radius = 16.0f * 0.5f;

    std::vector<Entity*> entities = { &player, &oppo, &ball };
    
    // @TODO(sjv): We will want a way to set resolution?
    // @TODO(sjv): Can we get the supported resolutions / aspect ratio for the screen?

    const Uint32 window_flags = 0; // SDL_WINDOW_FULLSCREEN;

    // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    SDL_ErrorCheck(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window *window = SDL_ErrorCheck(SDL_CreateWindow("cppong", 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          SDL_WINDOWPOS_UNDEFINED, 
                                                          world.window_w, world.window_h, 
                                                          window_flags));
  
    SDL_Renderer *renderer = SDL_ErrorCheck(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));

    SDL_ErrorCheck(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND));
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    TTF_ErrorCheck(TTF_Init());    

    FontCache *fc;
    init_font_cache(&fc, "assets/uni0553-webfont.ttf", renderer);

    SDL_Texture *spritesheet_texture = load_texture_from_file(renderer, "assets/spritesheet.png");
    SDL_Texture *shadow_texture = load_texture_from_file(renderer, "assets/spritesheet.png");
    SDL_ErrorCheck(SDL_SetTextureColorMod(shadow_texture, 0x00, 0x00, 0x00));
    SDL_ErrorCheck(SDL_SetTextureAlphaMod(shadow_texture, 0x33));
    Spritesheet spritesheet = { spritesheet_texture, shadow_texture, 16, 16 };
    
    bool quit { false };

    SDL_Event e;

    Uint32 prev_dt { 1 };
    float dt = 0.0f;
    
    while (!quit) {
        Uint32 begin = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } 

            if (e.type == SDL_KEYUP) {
                switch(e.key.keysym.sym) {
                    case SDLK_d: {
                        world.display_debug = !world.display_debug;
                    } break;

                    case SDLK_ESCAPE: {
                        if(world.state == GameGameState::InGame) {
                            world.state = GameGameState::Paused;
                        } else if(world.state == GameGameState::Paused) {
                            world.state = GameGameState::InGame;
                        }
                    } break;

                    case SDLK_z: {
                        if(world.display_debug) {
                            switch(world.state) {
                                case(GameGameState::CountDown): {
                                    world.state = GameGameState::InGame;
                                } break;
                                case(GameGameState::InGame): {
                                    world.state = GameGameState::Paused;
                                } break;
                                case(GameGameState::Paused): {
                                    world.state = GameGameState::PlayerScored;
                                } break;
                                case(GameGameState::PlayerScored): {
                                    world.state = GameGameState::CountDown;
                                } break;
                            }
                        }
                    } break;
                }
            }

        }

        const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
        float dy { 0 };
        
        if(world.state != GameGameState::Paused) {
            if(keyboard_state[SDL_SCANCODE_UP]) {
                dy = -1;
            } else if (keyboard_state[SDL_SCANCODE_DOWN]) {
                dy = 1;
            }
        }

        if((player.dstrect().y + dy) >= world.gamearea.y && (player.dstrect().y + dy + player.dstrect().h) < world.gamearea.y + world.gamearea.h)
        {
            player.pos.y += dy * dt * player.speed;
            oppo.pos.y += dy * dt * player.speed;
        }

        update_world(&world, &ball, entities, dt);
        render_world(&world, renderer, entities, &spritesheet, fc, dt);
        
        const Uint32 idt { SDL_GetTicks() - begin };
        dt = (float)idt / 1000.0f;

        prev_dt = idt;
    }

    printf("Exiting gracefully... :)\n");
    
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
