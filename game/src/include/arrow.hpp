#pragma once
#include "constants.h"
#include "game_object.h"
#include "raylib.h"
#include <memory>
#include "box2d/b2_body.h"
#include "collider_user_data.h"

class Arrow : public GameObject {
    public:
        Arrow(float x, float y, Vector2 direction);
        ~Arrow();

        void draw() override;
        void update(float dt) override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
        float getDamage();

    private:
        std::shared_ptr<b2Body> collider;
        float mvspd = ARROW_BASE_MVSPD;
        float damage = 75.0f;
        Vector2 direction;
        float radius = DEFENSE_TOWER_RADIUS * 0.6f;
        float height = 2.0f;
        float width = DEFENSE_TOWER_RADIUS * 1.2f;
        ColliderUserData *data;
        float ttl = 1.5f;
        float tt = 0.0f;
        bool alive = true;
        float rotation = 0.0f;
};
