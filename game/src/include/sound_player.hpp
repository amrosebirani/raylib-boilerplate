#pragma once
#include <string>

class SoundPlayer {
    public:
        SoundPlayer(std::string sound);
        ~SoundPlayer() {};
        void update(float dt);
        void play();
        void stop();

    private:
        std::string sound;
        float playTill;
        float soundDuration;
};
