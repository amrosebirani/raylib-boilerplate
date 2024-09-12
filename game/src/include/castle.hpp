#pragma once

#include "collider_user_data.h"
#include "game_object.h"
#include "box2d/b2_body.h"
#include "progress_bar.hpp"
#include "raylib.h"
#include "constants.h"
#include "region.hpp"

class Castle : public GameObject {
    public:
        Castle(float x, float y, float health, int level);
        ~Castle();

        void update(float dt) override;
        void draw() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
        Vector2 getDimensions();

        enum CastleState {
            OPERATIONAL,
            OPERATIONAL_UNDER_ATTACK,
            UNOPERATIONAL,
            UNOPERATIONAL_UNDER_ATTACK,
            CAPTURED
        };

        CastleState getState();
        void hit(float damage);

    private:
        float health = 10000;
        float maxHealth = 10000;
        float height = CASTLE_HEIGHT;
        float width = 2176.0f * CASTLE_HEIGHT / 1516.0f;
        bool alive = true;
        int level = 1;
        std::shared_ptr<b2Body> collider;
        CastleState state;
        bool isAttacked = false;
        float changeAttackTimeout = 0;
        float changeAttackCounter = 0;
        ColliderUserData *data;
        Region *region;
        std::shared_ptr<ProgressBar> healthBar;
};
