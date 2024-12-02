#pragma once
#include "room.hpp"
#include "raylib.h"
#include <string>

class Victory : public Room {
    public:
        Victory();
        ~Victory();
        bool update(float dt) override;
        void draw() override;
        bool isFinished() override;
        void reset();

    private:
        bool finished = false;
        Rectangle mainMenuRect;
        std::string mainMenuText = "Go To Main Menu";
        float rtm;
        Texture2D victoryL;
        Texture2D victoryP;
};
