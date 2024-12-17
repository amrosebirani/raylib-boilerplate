#pragma once
#include "warrior.hpp"

class WarriorPara : public Warrior {
    public:
        WarriorPara(float rx, float ry, WarriorType type, bool inFormation);
        WarriorPara(float x, float y, WarriorType type);
        ~WarriorPara();

        void draw() override;

        void update(float dt) override;

        bool isAlive() override;

        void die() override;

        void attack() override;
        void takeAttack(float damage) override;
        void init() override;
        void cleanupData() override;
        WarriorType getType() override {
            return type;
        }

    private:
        bool alive = true;
        float hp = 0;
        WarriorType type;
};
