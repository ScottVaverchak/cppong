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

void render_border(SDL_Renderer *renderer, SDL_Rect gamearea, SDL_Texture *spritesheet) {
    // @TODO(sjv): This looks like it could be optimized more
    // @NOTE(sjv): Maybe this should be its own data struct for 9 slices

    SDL_Rect srctl = { 16 * 2, 16 * 0, 16, 16 };
    SDL_Rect srctm = { 16 * 3, 16 * 0, 16, 16 };
    SDL_Rect srctr = { 16 * 4, 16 * 0, 16, 16 };
    SDL_Rect srcml = { 16 * 2, 16 * 1, 16, 16 };
    SDL_Rect srcmr = { 16 * 4, 16 * 1, 16, 16 };
    SDL_Rect srcbl = { 16 * 2, 16 * 2, 16, 16 };
    SDL_Rect srcbm = { 16 * 3, 16 * 2, 16, 16 };
    SDL_Rect srcbr = { 16 * 4, 16 * 2, 16, 16 };

    //  TOP
    SDL_Rect dsttl = {gamearea.x - 16, gamearea.y - 16, 16, 16};
    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                    &srctl, &dsttl, 0,  
                                    nullptr, SDL_FLIP_NONE));

    SDL_Rect dsttm = {gamearea.x, gamearea.y - 16, gamearea.w, 16};
    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                    &srctm, &dsttm, 0,  
                                    nullptr, SDL_FLIP_NONE));
    
    SDL_Rect dsttr = {gamearea.x + gamearea.w, gamearea.y - 16, 16, 16};
    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                    &srctr, &dsttr, 0,  
                                    nullptr, SDL_FLIP_NONE));

    // MIDDLE
    SDL_Rect dstml = {gamearea.x - 16, gamearea.y, 16, gamearea.h};
    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                    &srcml, &dstml, 0,  
                                    nullptr, SDL_FLIP_NONE));

    SDL_Rect dstmr = {gamearea.x + gamearea.w, gamearea.y, 16, gamearea.h};
    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                    &srcmr, &dstmr, 0,  
                                    nullptr, SDL_FLIP_NONE));

    // BOTTOM
    SDL_Rect dstbl = {gamearea.x - 16, gamearea.y + gamearea.h, 16, 16};
    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                    &srcbl, &dstbl, 0,  
                                    nullptr, SDL_FLIP_NONE));

    SDL_Rect dstbm = {gamearea.x, gamearea.y + gamearea.h, gamearea.w, 16};
    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                    &srcbm, &dstbm, 0,  
                                    nullptr, SDL_FLIP_NONE));

    SDL_Rect dstbr = {gamearea.x + gamearea.w, gamearea.y + gamearea.h, 16, 16};
    SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                    &srcbr, &dstbr, 0,  
                                    nullptr, SDL_FLIP_NONE));

}



// void render_end() {}