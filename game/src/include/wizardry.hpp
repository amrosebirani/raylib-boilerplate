#pragma once

#include "building.hpp"

class Wizardry : public Building {
    public:
        Wizardry(float x, float y, int level);
        ~Wizardry();
        void repair(float repairAmount) override;
        void onUpgrade(int level) override;
        void update(float dt) override;
        void draw() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
};
