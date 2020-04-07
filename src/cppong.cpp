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

const int WINDOW_W = 800;
const int WINDOW_H = 400;

enum class GameGameState {
    CountDown,
    InGame,
    PlayerWon
};

struct GameState {
    int player1_score;
    int player2_score;

    GameGameState state;
};

const SDL_Rect GAMEAREA = {
    static_cast<int>((WINDOW_W - (WINDOW_W * 0.9f)) * 0.5f),
    static_cast<int>((WINDOW_H - (WINDOW_H * 0.75f)) * 0.5f),
    static_cast<int>(WINDOW_W * 0.9f),
    static_cast<int>(WINDOW_H * 0.75f)
};

Vec2f ball_spawn_pos = { 
    GAMEAREA.x + (GAMEAREA.w * 0.5f), 
    GAMEAREA.y + (GAMEAREA.h * 0.5f) 
};

int cppong_main() {
    printf("Initializing SDL...\n");
    GameState game_state = {};
    game_state.state = GameGameState::CountDown;

    const int PLAYER_OFFSET_X = 10;

    Paddle player = {};
    player.pos = { (float)(GAMEAREA.x + PLAYER_OFFSET_X), GAMEAREA.h * 0.5f };
    player.hitbox = rect(player.pos, 24.0f, 60.0f);
    player.srcrect = {16, 0, 16, 16 * 3};
    player.w = 16.0f;
    player.h = 16.0f * 3.0f;

    Paddle oppo = {};
    oppo.pos = { (float)(GAMEAREA.x + GAMEAREA.w) - PLAYER_OFFSET_X, GAMEAREA.h * 0.5f };
    oppo.hitbox = rect(oppo.pos, 24.0f, 60.0f);
    oppo.srcrect = {16, 0, 16, 16 * 3};
    oppo.w = 16.0f;
    oppo.h = 16.0f * 3.0f;

    Ball ball = {};
    ball.pos = ball_spawn_pos;
    ball.srcrect = {0, 0, 16, 16};
    ball.vel = { 2, 0 };
    ball.w = ball.h = 16.0f;
    ball.radius = 16.0f * 0.5f;

    std::vector<Entity*> entities = { &player, &oppo, &ball};

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

    SDL_Texture *spritesheet_texture = load_texture_from_file(renderer, "assets/spritesheet.png");
    Spritesheet spritesheet = { spritesheet_texture, 16, 16 };
    bool quit { false };
    bool display_debug { true };

    SDL_Event e;

    Uint32 prev_dt { 1 };
    
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

                    case SDLK_z: {
                        if(display_debug) {
                            switch(game_state.state) {
                                case(GameGameState::CountDown): {
                                    game_state.state = GameGameState::InGame;
                                } break;
                                case(GameGameState::InGame): {
                                    game_state.state = GameGameState::PlayerWon;
                                } break;
                                case(GameGameState::PlayerWon): {
                                    game_state.state = GameGameState::CountDown;
                                } break;
                            }
                        }
                    } break;
                }
            }

        }

        const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
        int8_t dy { 0 };

        if(keyboard_state[SDL_SCANCODE_UP]) {
            dy = -1;
        } else if (keyboard_state[SDL_SCANCODE_DOWN]) {
            dy = 1;
        }

        if((player.dstrect().y + dy) >= GAMEAREA.y && (player.dstrect().y + dy + player.dstrect().h) < GAMEAREA.y + GAMEAREA.h)
        {
            player.pos.y += dy * 2;
            oppo.pos.y += dy * 2;
        }

        switch(game_state.state) {
            case(GameGameState::CountDown): {

            } break;
            case(GameGameState::InGame): {
                ball.pos += ball.vel;

                if((ball.pos.x + ball.radius) > GAMEAREA.x + GAMEAREA.w) {
                    ball.vel *= {-1, 1};
                    ball.pos = ball_spawn_pos;
                    game_state.player1_score++;
                }
                    

                if ((ball.pos.x - ball.radius) < GAMEAREA.x) {
                    ball.pos = ball_spawn_pos;
                    ball.vel *= {-1, 1};
                    game_state.player2_score++;
                }
                    

                if((ball.pos.y + ball.radius) > GAMEAREA.y + GAMEAREA.h || (ball.pos.y - ball.radius) < GAMEAREA.y)
                    ball.vel *= {1, -1};

                update_collision(entities, &ball);
            } break;
            case(GameGameState::PlayerWon): {

            } break;
        }

        render_start(renderer);
        render_border(renderer, GAMEAREA, &spritesheet, {2, 0});
        render_entities(renderer, entities, spritesheet_texture);

        render_text(fc, renderer, "cppong++", 32, {(WINDOW_W * 0.5f), 0.0f });
        render_text(fc, renderer, std::to_string(game_state.player1_score), 32, { 20.0f, 0.0f });
        render_text(fc, renderer, std::to_string(game_state.player2_score), 32, { WINDOW_W - 20.0f - 32.0f, 0.0f });
        
        std::string name;
        switch(game_state.state) {
            case(GameGameState::CountDown): {
                name = "CountDown";                
            } break;
            case(GameGameState::InGame): {
                name = "InGame";
            } break;
            case(GameGameState::PlayerWon): {
                name = "PlayerWon";
            } break;
        }

        if(display_debug) {
            draw_colored_rectangle(renderer, player.dstrect(), 0x0000FFFF);
            draw_colored_rectangle(renderer, oppo.dstrect(), 0xFFFFFFFF);
            draw_colored_rectangle(renderer, sdl_to_rect<float>(GAMEAREA), 0xFF0000FF);

            draw_colored_circle(renderer, ball.pos, ball.radius, 0xFF00FFFF);
            render_text(fc, renderer, name,48, { (float)GAMEAREA.x, GAMEAREA.h + 48.0f});
        }

        SDL_RenderPresent(renderer);
        
        const Uint32 dt { SDL_GetTicks() - begin };
        prev_dt = dt;
        
    }

    printf("Exiting gracefully... :)\n");
    
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
