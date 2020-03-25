struct Entity {
    Vec2f pos;
    Vec2f vel;

    float w, h;
    SDL_Rect srcrect;
    SDL_RendererFlip flipmode = SDL_FLIP_NONE;

    constexpr Rectf dstrect() const {
        return rect(pos - vec2(w * 0.5f, h * 0.5f), w, h);
    }
};

struct Paddle : Entity {
    Rectf hitbox;
};

struct Ball : Entity {
    float radius;
};

struct CollisionRecord {
    Vec2f world_position;
};

bool paddle_ball_collision(const Entity *entity, const Ball *ball, CollisionRecord *record) {
    auto hitbox = entity->dstrect();

    auto dx = ball->pos.x - max(hitbox.x, min(ball->pos.x, hitbox.x + hitbox.w));
    auto dy = ball->pos.y - max(hitbox.y, min(ball->pos.y, hitbox.y + hitbox.h));

    record->world_position.x = ball->pos.x;
    record->world_position.y = ball->pos.y;
    
    return (dx * dx + dy * dy) < (ball->radius * ball->radius);
}

void update_collision(std::vector<Entity*> entities, Ball *ball) {
    for(const auto &entity: entities) {
        if(entity == ball) continue;

        CollisionRecord left_paddle_coll = {};
        if(paddle_ball_collision(entity, ball, &left_paddle_coll)) {
            ball->vel *= {-1, 1};
        }
    }
}
