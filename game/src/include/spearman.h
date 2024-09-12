#pragma once

#include "warrior.hpp"

class Spearman : public Warrior {
    public:
        Spearman(float rx, float ry);
        ~Spearman();

        void draw() override;

        void update(float dt) override;

        bool isAlive() override;

        void die() override;
        void init() override;
        void cleanupData() override;

        void attack() override;
        void tryAttack(std::shared_ptr<GameObject> target) override;
        void takeAttack(float damage) override;

    private:
        bool alive = true;
        float hp = 200;
};
