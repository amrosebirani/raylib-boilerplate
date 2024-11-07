#pragma once

#include "warrior.hpp"

class ShieldBearer : public Warrior {
    public:
        ShieldBearer(float rx, float ry);
        ~ShieldBearer();

        void draw() override;

        void update(float dt) override;

        bool isAlive() override;

        void die() override;
        void init() override;
        void cleanupData() override;

        void attack() override;

        void takeAttack(float damage) override;

    private:
        bool alive = true;
        float hp = 300.0f;
};
