struct Entity {
    Vec2f pos;
    Vec2f vel;

    SDL_Rect hitbox;
    SDL_Rect srcrect;
};


bool paddle_ball_collision(const SDL_Rect rect, const int radius, const Vec2f ball_position) {
    auto dx = ball_position.x - max((float)rect.x, min(ball_position.x, (float)(rect.x + rect.w)));
    auto dy = ball_position.y - max((float)rect.y, min(ball_position.y, (float)(rect.y + rect.h)));
    return (dx * dx + dy * dy) < (radius * radius);
}
