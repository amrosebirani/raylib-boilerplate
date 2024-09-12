#pragma once

#include "warrior.hpp"

class Swordsman : public Warrior {
    public:
        Swordsman(float rx, float ry);
        ~Swordsman();

        void draw() override;

        void update(float dt) override;

        bool isAlive() override;

        void die() override;

        void attack() override;
        void tryAttack(std::shared_ptr<GameObject> target) override;
        void takeAttack(float damage) override;
        void init() override;
        void cleanupData() override;

    private:
        bool alive = true;
        float hp = 500;
};
