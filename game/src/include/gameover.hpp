#pragma once
#include "room.hpp"
#include "raylib.h"
#include <string>

class GameOver : public Room {
    public:
        GameOver();
        ~GameOver();
        bool update(float dt) override;
        void draw() override;
        bool isFinished() override;
        void reset();

    private:
        bool finished = false;
        Texture2D deadBody;
        Rectangle restartRect;
        std::string restartText = "Restart";
        float rtm;
        Texture2D gameoverL;
        Texture2D gameoverP;
};
