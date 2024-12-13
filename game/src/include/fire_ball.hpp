#pragma once
#include "animation.hpp"
#include "box2d/b2_body.h"
#include "collider_user_data.h"
#include "constants.h"
#include "game_object.h"

class FireBall : public GameObject {
    public:
        FireBall(float x, float y, Vector2 direction);
        ~FireBall();

        void draw() override;
        void update(float dt) override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
        float getDamage();

    private:
        std::shared_ptr<b2Body> collider;
        float mvspd = FIREBALL_BASE_MVSPD;
        float damage = 75.0f;
        Vector2 direction;
        float width = DEFENSE_TOWER_RADIUS * 1.2f;
        float height = DEFENSE_TOWER_RADIUS * 1.2f * 16.0f / 36;
        ColliderUserData *data;
        float ttl = 1.5f;
        float tt = 0.0f;
        bool alive = true;
        float rotation = 0.0f;
        Animation *anim;
};
