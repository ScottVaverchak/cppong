const int LOW_END_ASCII = 32;
const int HIGH_END_ASCII = 122;

struct FontCache {
    SDL_Texture **textures;
    const char *font_location;
    float font_size;
};

void init_font_cache(FontCache **font_cache, const char *font_location, SDL_Renderer *renderer) {
    // FontCache *fc = (FontCache *)malloc(sizeof(FontCache));
    
    FontCache *fc = new FontCache;
    
    fc->textures = new SDL_Texture*[HIGH_END_ASCII - LOW_END_ASCII];
    fc->font_location = font_location;
    fc->font_size = 32.0f;

    TTF_Font *main_font = TTF_ErrorCheck(TTF_OpenFont(fc->font_location, fc->font_size));

    for(int index = LOW_END_ASCII; index <= HIGH_END_ASCII; index++) {
        const char *ch = new char[1]{(char)index};

        SDL_Surface *font_surface = TTF_ErrorCheck(TTF_RenderText_Blended(main_font, ch, {255, 255, 255}));
        SDL_Texture *font_texture = SDL_ErrorCheck(SDL_CreateTextureFromSurface(renderer, font_surface));
        SDL_FreeSurface(font_surface);

        fc->textures[(index - 32)] = font_texture;
    }

    *font_cache = fc;

    TTF_CloseFont(main_font);
}


void render_text(FontCache *font_cache, SDL_Renderer *renderer, const char *text, float font_size, Vec2i position) {
    const size_t text_length = strlen(text);
    float xoffset = 0;
    const float aspect_ratio = font_size / font_cache->font_size;
    const float padding = 2 * aspect_ratio;
    for(int i = 0; i < text_length; i++) {
        int ascii = (int)text[i];
        if(ascii < LOW_END_ASCII || ascii > HIGH_END_ASCII)
            continue;

        SDL_Texture *char_texture = font_cache->textures[ascii - 32];
        
        int w, h;

        // TODO: This should be generated in the INIT funciton
        SDL_QueryTexture(char_texture, nullptr, nullptr, &w, &h);
        w *= aspect_ratio;
        SDL_Rect location = {position.x + (int)xoffset, position.y, w, (int)font_size };
        xoffset += w + padding;

        SDL_RenderCopy(renderer, char_texture, nullptr, &location);
    }
}
