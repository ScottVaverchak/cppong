enum class GameGameState {
    CountDown,
    InGame,
    PlayerWon
};

struct World {
    int player1_score;
    int player2_score;

    GameGameState state;
    Recti gamearea;

    int window_w, window_h;
    bool display_debug;
};

struct Entity {
    Vec2f pos;
    Vec2f vel;

    float w, h;
    float speed;

    SDL_Rect srcrect;
    SDL_RendererFlip flipmode = SDL_FLIP_NONE;

    virtual void check_collision() = 0;

    constexpr Rectf dstrect() const {
        return rect(pos - vec2(w * 0.5f, h * 0.5f), w, h);
    }
};

struct Paddle final : Entity {
    Rectf hitbox;

    void check_collision() {}
};

struct Ball final : Entity {
    float radius;
    Vec2f spawn_pos;

    void check_collision() {}
};

struct CollisionRecord final {
    Vec2f world_position;
};

bool paddle_ball_collision(const Entity *entity, const Ball *ball, CollisionRecord *record) {
    auto hitbox = entity->dstrect();

    auto dx { ball->pos.x - max(hitbox.x, min(ball->pos.x, hitbox.x + hitbox.w)) };
    auto dy { ball->pos.y - max(hitbox.y, min(ball->pos.y, hitbox.y + hitbox.h)) };

    record->world_position.x = ball->pos.x;
    record->world_position.y = ball->pos.y;
    
    return (dx * dx + dy * dy) < (ball->radius * ball->radius);
}

void update_collision(std::vector<Entity*> const entities, Ball *ball) {
    for(const auto &entity: entities) {
        if(entity == ball) continue;

        CollisionRecord left_paddle_coll = {};
        if(paddle_ball_collision(entity, ball, &left_paddle_coll)) {
            ball->vel *= {-1, 1};
        }
    }
}

void update_world(World *world, Ball *ball, std::vector<Entity*> const &entities, float dt) {
    switch(world->state) {
        case(GameGameState::CountDown): {

        } break;
        case(GameGameState::InGame): {
            ball->pos += ball->vel;

            if((ball->pos.x + ball->radius) > world->gamearea.x + world->gamearea.w) {
                ball->vel *= {-1, 1};
                ball->pos = ball->spawn_pos;
                world->player1_score++;
            }

            if ((ball->pos.x - ball->radius) < world->gamearea.x) {
                ball->pos = ball->spawn_pos;
                ball->vel *= {-1, 1};
                world->player2_score++;
            }
                
            if((ball->pos.y + ball->radius) > world->gamearea.y + world->gamearea.h || (ball->pos.y - ball->radius) < world->gamearea.y)
                ball->vel *= {1, -1};

            update_collision(entities, ball);
        } break;
        case(GameGameState::PlayerWon): {

        } break;
    }
}
