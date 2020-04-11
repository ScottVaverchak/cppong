struct Spritesheet {
    SDL_Texture* texture;
    const int tile_width;
    const int tile_height;
};

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

void render_start(SDL_Renderer *renderer) {
    SDL_ErrorCheck(SDL_RenderClear(renderer));
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x77, 0x77, 0x99, 0xFF));
}

void render_entities(SDL_Renderer *renderer, const World *world, const std::vector<Entity *> &entities, SDL_Texture *spritesheet) {
    for(const auto &entity: entities) {
        SDL_Rect dstrect = rect_to_sdl(entity->dstrect());
        
        if(Paddle *p = dynamic_cast<Paddle *>(entity)) {
            // 2 * ((x - 0) / (100 - 0)) - 1
            // world->gamearea
            auto y = 2 * ((entity->pos.y - world->gamearea.y) / ((world->gamearea.h + world->gamearea.y) - world->gamearea.y)) - 1.0f;
            draw_solid_rectangle(renderer, entity->dstrect().offset(-y * 5.0f ), 0x000000FF);
        } 

        SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                        &entity->srcrect, &dstrect, 0, 
                                        nullptr, entity->flipmode));
    
    }
}

void render_border(SDL_Renderer *renderer, SDL_Rect gamearea, Spritesheet *spritesheet, const Vec2<int> offset = {0, 0}) {
    SDL_Rect srctl { 
        spritesheet->tile_width  * offset.x, 
        spritesheet->tile_height * offset.y,
        spritesheet->tile_width, 
        spritesheet->tile_height 
    };

    SDL_Rect srctm { 
        spritesheet->tile_width  * (offset.x + 1), 
        spritesheet->tile_height * offset.y, 
        spritesheet->tile_width, 
        spritesheet->tile_height 
    };

    SDL_Rect srctr { 
        spritesheet->tile_width  * (offset.x + 2), 
        spritesheet->tile_height * offset.y, 
        spritesheet->tile_width, 
        spritesheet->tile_height 
    };

    SDL_Rect srcml { 
        spritesheet->tile_width  * offset.x, 
        spritesheet->tile_height * (offset.y + 1), 
        spritesheet->tile_width, 
        spritesheet->tile_height 
    };

    SDL_Rect srcmr { 
        spritesheet->tile_width  * (offset.x + 2), 
        spritesheet->tile_height * (offset.y + 1), 
        spritesheet->tile_width, 
        spritesheet->tile_height 
    };

    SDL_Rect srcbl { 
        spritesheet->tile_width  * offset.x, 
        spritesheet->tile_height * (offset.y + 2), 
        spritesheet->tile_width, 
        spritesheet->tile_height 
    };

    SDL_Rect srcbm { 
        spritesheet->tile_width  * (offset.x + 1), 
        spritesheet->tile_height * (offset.y + 2), 
        spritesheet->tile_width, 
        spritesheet->tile_height 
    };

    SDL_Rect srcbr { 
        spritesheet->tile_width  * (offset.x + 2), 
        spritesheet->tile_height * (offset.y + 2), 
        spritesheet->tile_width, 
        spritesheet->tile_height 
    };


    //  TOP
    SDL_Rect dsttl {
        gamearea.x - spritesheet->tile_width, 
        gamearea.y - spritesheet->tile_height, 
        spritesheet->tile_width, 
        spritesheet->tile_height
    };

    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet->texture, 
                                    &srctl, &dsttl, 0,  
                                    nullptr, SDL_FLIP_NONE));

    SDL_Rect dsttm {
        gamearea.x, 
        gamearea.y - spritesheet->tile_height, 
        gamearea.w, 
        spritesheet->tile_height
    };

    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet->texture, 
                                    &srctm, &dsttm, 0,  
                                    nullptr, SDL_FLIP_NONE));
    
    SDL_Rect dsttr {
        gamearea.x + gamearea.w, 
        gamearea.y - spritesheet->tile_height, 
        spritesheet->tile_width, 
        spritesheet->tile_height
    };

    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet->texture, 
                                    &srctr, &dsttr, 0,  
                                    nullptr, SDL_FLIP_NONE));

    // MIDDLE
    SDL_Rect dstml {
        gamearea.x - spritesheet->tile_width, 
        gamearea.y, 
        spritesheet->tile_width, 
        gamearea.h
    };

    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet->texture, 
                                    &srcml, &dstml, 0,  
                                    nullptr, SDL_FLIP_NONE));

    SDL_Rect dstmr {
        gamearea.x + gamearea.w, 
        gamearea.y, 
        spritesheet->tile_width, 
        gamearea.h
    };

    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet->texture, 
                                    &srcmr, &dstmr, 0,  
                                    nullptr, SDL_FLIP_NONE));

    // BOTTOM
    SDL_Rect dstbl {
        gamearea.x - spritesheet->tile_width, 
        gamearea.y + gamearea.h, 
        spritesheet->tile_width, 
        spritesheet->tile_height
    };

    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet->texture, 
                                    &srcbl, &dstbl, 0,  
                                    nullptr, SDL_FLIP_NONE));

    SDL_Rect dstbm {
        gamearea.x, 
        gamearea.y + gamearea.h, 
        gamearea.w, 
        spritesheet->tile_height
    };

    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet->texture, 
                                    &srcbm, &dstbm, 0,  
                                    nullptr, SDL_FLIP_NONE));

    SDL_Rect dstbr {
        gamearea.x + gamearea.w, 
        gamearea.y + gamearea.h, 
        spritesheet->tile_width, 
        spritesheet->tile_height
    };

    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet->texture, 
                                    &srcbr, &dstbr, 0,  
                                    nullptr, SDL_FLIP_NONE));

}

void render_world(World *world, SDL_Renderer *renderer, const std::vector<Entity *> &entities, Spritesheet *spritesheet, FontCache *fc, float dt) {
    render_start(renderer);
    render_border(renderer, rect_to_sdl(world->gamearea), spritesheet, {2, 0});
    render_entities(renderer, world, entities, spritesheet->texture);

    render_text(fc, renderer, "cppong++", 32, {(world->window_w * 0.5f), 0.0f });
    render_text(fc, renderer, std::to_string(world->player1_score), 32, { 20.0f, 0.0f });
    render_text(fc, renderer, std::to_string(world->player2_score), 32, { world->window_w - 20.0f - 32.0f, 0.0f });
    
    std::string name;
    switch(world->state) {
        case(GameGameState::CountDown): {
            Vec2f count_dims = {};
            
            auto count = std::to_string(((int)world->countDownCounter) + 1);

            string_texture_dims(fc, count, 128, count_dims);
            Vec2f pos1 = { (world->window_w * 0.5f) - (count_dims.x * 0.5f), (world->window_w * 0.5f) - count_dims.y - 50.0f };
            render_text(fc, renderer, count, 128, pos1);

            name = "CountDown";                
        } break;
        case(GameGameState::InGame): {
            name = "InGame";
        } break;
        case(GameGameState::Paused): {
            Vec2f dims = {};
            
            string_texture_dims(fc, "paused", 128, dims);
            Vec2f pos = { (world->window_w * 0.5f) - (dims.x * 0.5f), (world->window_w * 0.5f) - dims.y };
            render_text(fc, renderer, "paused", 128, pos + 5.0f);
            draw_solid_rectangle(renderer, {0.0f, 0.0f, (float)world->window_w, (float)world->window_h}, 0x03030388);
            render_text(fc, renderer, "paused", 128, pos);

            name = "Paused";
        } break;
        case(GameGameState::PlayerScored): {
            name = "PlayerScored";
        } break;
    }

    if(world->display_debug) {
        draw_colored_rectangle(renderer, rect_cast<float>(world->gamearea), 0xFF0000FF);
        for(const auto &entity: entities) {
            if(Paddle *p = dynamic_cast<Paddle *>(entity)) {
                draw_colored_rectangle(renderer, p->dstrect(), 0x0000FFFF);
            } else if(Ball *b = dynamic_cast<Ball *>(entity)) {
                draw_colored_circle(renderer, b->pos, b->radius, 0xFF00FFFF);
            } 

            draw_colored_circle(renderer, entity->pos, 5, 0xFF00FFFF);
        }
        render_text(fc, renderer, name, 48, { (float)world->gamearea.x, world->gamearea.h + 48.0f});
    }

    SDL_RenderPresent(renderer);
}