struct Entity {
    Vec2f pos;
    Vec2f vel;

    Rectf hitbox;
    SDL_Rect srcrect;
};

struct CollisionRecord {
    Vec2f world_position;
};


bool paddle_ball_collision(const Rectf rect, const int radius, const Vec2f ball_position, CollisionRecord *record) {
    auto dx = ball_position.x - max(rect.x, min(ball_position.x, rect.x + rect.w));
    auto dy = ball_position.y - max(rect.y, min(ball_position.y, rect.y + rect.h));

    record->world_position.x = ball_position.x;
    record->world_position.y = ball_position.y;
    
    return (dx * dx + dy * dy) < (radius * radius);
}
