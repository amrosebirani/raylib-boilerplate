#pragma once
#include "room.hpp"
#include "raylib.h"

class MainMenu : public Room {
    public:
        MainMenu();
        ~MainMenu();
        bool update(float dt) override;
        void draw() override;
        bool isFinished() override;
        void reset();

    private:
        bool finished = false;
        Texture2D level1;
        Texture2D level2;
        Rectangle start1Rect;
};
