const int LOW_END_ASCII = 32;
const int HIGH_END_ASCII = 122;

struct CharTexture final {
    SDL_Texture *texture;
    int width;
    int height;
};

struct FontCache final {
    CharTexture *char_textures;
    const char *font_location;
    float font_size;
};

void init_font_cache(FontCache **font_cache, const char *font_location, SDL_Renderer *renderer) {
    FontCache *fc { new FontCache };
    
    fc->char_textures = new CharTexture[HIGH_END_ASCII - LOW_END_ASCII];
    fc->font_location = font_location;
    fc->font_size = 32.0f;

    TTF_Font *main_font { TTF_ErrorCheck(TTF_OpenFont(fc->font_location, fc->font_size)) };

    for(int ascii_code = LOW_END_ASCII; ascii_code <= HIGH_END_ASCII; ascii_code++) {
        const char *ch = new char[1]{(char)ascii_code};

        SDL_Surface *font_surface = TTF_ErrorCheck(TTF_RenderText_Blended(main_font, ch, {255, 255, 255}));
        SDL_Texture *font_texture = SDL_ErrorCheck(SDL_CreateTextureFromSurface(renderer, font_surface));
        SDL_FreeSurface(font_surface);

        auto index { ascii_code - 32 };
        int w, h;
        
        SDL_QueryTexture(font_texture, nullptr, nullptr, &w, &h);
        fc->char_textures[index].width = w;
        fc->char_textures[index].height = h;
        fc->char_textures[index].texture = font_texture;
    }

    *font_cache = fc;

    TTF_CloseFont(main_font);
}

void render_text(FontCache *font_cache, SDL_Renderer *renderer, const char *text, float font_size, Vec2f position) {
    const size_t text_length { strlen(text) };
    float xoffset { 0 };
    const float aspect_ratio { font_size / font_cache->font_size };
    const float padding { 2.0f * aspect_ratio };
    for(int i = 0; i < text_length; i++) {
        int ascii { (int)text[i] };
        if(ascii < LOW_END_ASCII || ascii > HIGH_END_ASCII)
            continue;

        CharTexture char_texture = font_cache->char_textures[ascii - 32];
      
        auto width = char_texture.width * aspect_ratio;
        SDL_Rect location = {(int)position.x + (int)xoffset, (int)position.y, (int)width, (int)font_size };
        xoffset += width + padding;

        SDL_RenderCopy(renderer, char_texture.texture, nullptr, &location);
    }
}

void render_text(FontCache *font_cache, SDL_Renderer *renderer, std::string text, float font_size, Vec2f position) {
    render_text(font_cache, renderer, text.c_str(), font_size, position);
}
