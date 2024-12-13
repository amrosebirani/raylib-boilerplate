#pragma once

#include "building.hpp"
class HouseBlock : public Building {
    public:
        HouseBlock(float x, float y, int level);
        ~HouseBlock();
        void repair(float repairAmount) override;
        void onUpgrade(int level) override;
        void draw() override;
        void update(float dt) override;
        void init() override;
        void cleanupData() override;
        bool isAlive() override;
        void die() override;

    private:
        float tributeRate;
};
