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
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF));
}

void render_entities(SDL_Renderer *renderer, const std::vector<Entity *> &entities, SDL_Texture *spritesheet) {
    for(const auto &entity: entities) {
        SDL_Rect dstrect = rect_to_sdl(entity->dstrect());

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

