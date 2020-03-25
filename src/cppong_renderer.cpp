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

void render_entities(SDL_Renderer *renderer, const std::vector<Entity *> &entities, SDL_Texture *spritesheet) {
    SDL_ErrorCheck(SDL_RenderClear(renderer));
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF));

    for(const auto &entity: entities) {
        SDL_Rect dstrect = rect_to_sdl(entity->dstrect());

        SDL_ErrorCheck(SDL_RenderCopyEx(renderer, spritesheet, 
                                        &entity->srcrect, &dstrect, 0, 
                                        nullptr, entity->flipmode));
    
    }
}