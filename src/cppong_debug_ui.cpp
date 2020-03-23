
void draw_colored_rectangle(SDL_Renderer *renderer, Rectf rect, uint32_t color) {

    Uint8 r, g, b, a;
    SDL_ErrorCheck(SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a));
    
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, 
        (color & 0xFF000000) >> 24, 
        (color & 0x00FF0000) >> 16, 
        (color & 0x0000FF00) >> 8, 
        (color & 0x000000FF)));

    auto sdl_rect = rect_to_sdl(rect);
    
    SDL_ErrorCheck(SDL_RenderDrawRect(renderer, &sdl_rect));
    SDL_ErrorCheck(SDL_SetRenderDrawColor(renderer, r, g, b, a));
}

void draw_colored_circle(SDL_Renderer *renderer, Vec2f position, int radius, uint32_t color) {
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
