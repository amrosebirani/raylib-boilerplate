#pragma once
#include "room.hpp"
#include "raylib.h"
#include "scoreboard.hpp"
#include <memory>
#include <string>

class GameOver : public Room {
    public:
        GameOver();
        ~GameOver();
        bool update(float dt) override;
        void draw() override;
        bool isFinished() override;
        void reset();
        void drawScoreBoard();
        void setScoreBoardVars();

    private:
        bool finished = false;
        Texture2D deadBody;
        Rectangle restartRect;
        std::string restartText = "Restart";
        float rtm;
        float rsf;
        float rth;
        Texture2D gameoverL;
        Texture2D gameoverP;
        std::shared_ptr<ScoreBoard> sb = nullptr;
        float sbs;
        float sbh;
        float sbf;
        float sbm;
        std::string scoreText;
        std::string killsText;
        std::string timeText;
        std::string waveText;
        float scoreTextM;
        float killsTextM;
        float timeTextM;
        float waveTextM;
};
