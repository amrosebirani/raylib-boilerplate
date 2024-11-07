#pragma once

#include "building.hpp"
#include "raylib.h"
#include "constants.h"

class Castle : public Building {
    public:
        Castle(float x, float y, float health, int level);
        ~Castle();

        void repair(float repairAmount) override;
        void onUpgrade(int level) override;
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
        float height = CASTLE_HEIGHT;
        float width = 2176.0f * CASTLE_HEIGHT / 1516.0f;
        bool alive = true;
        int level = 1;
        CastleState state;
        bool isAttacked = false;
        float changeAttackTimeout = 0;
        float changeAttackCounter = 0;
};
