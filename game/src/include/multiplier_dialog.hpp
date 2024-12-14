#pragma once

#include "raylib.h"
#include "room.hpp"
#include "upgrade_manager.hpp"

class MultiplierDialog : public Room {
    public:
        MultiplierDialog();
        ~MultiplierDialog();

        void draw() override;
        bool update(float dt) override;
        bool isFinished() override;

    private:
        bool finished = false;
        std::vector<std::shared_ptr<UpgradeAction>> upgrades;
        Rectangle rect0;
        Rectangle rect1;
        Rectangle rect2;
        Rectangle rect3;
        Rectangle rect4;
        bool isPressed = false;
        void drawUpgrade(Rectangle rect,
                         std::shared_ptr<UpgradeAction> upgrade);
        void drawUpgradeText(Rectangle rect,
                             std::vector<std::string> display_strs);
        void handleClick(std::vector<Rectangle> rects);
};
