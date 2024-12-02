#pragma once
#include "raylib.h"
#include "room.hpp"
#include <string>

class Loading : public Room {
    public:
        Loading();
        ~Loading();
        bool update(float dt) override;
        void draw() override;
        bool isFinished() override;

    private:
        bool finished = false;
        std::string loadingText;
        bool buildingSpritesInit = false;
        bool warriorSpritesInit = false;
        bool treeSpritesInit = false;
        bool townSpritesInit = false;
        bool enemySpritesInit = false;
        bool firstTime = true;
        bool soundsLoaded = false;
        Texture2D loadingL;
        Texture2D loadingP;
};
