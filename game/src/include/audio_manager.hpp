#pragma once

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>
#include "raylib.h"
#include "sound_player.hpp"
#include "timer.h"

class AudioManager {
    public:
        AudioManager();

        void update(float dt);
        void updateAudio();
        void init();
        void initAudio();
        void finish();
        bool areSoundsLoaded();
        void startBGM();
        void play(std::string sound);
        void stopBGM();
        Sound getSound(std::string sound);
        void playRandomCombatSound();
        void playBuildingHitSound();
        void playRandomSwishSound();
        void switchBGM(std::string next, float fadeTime = 1.0f);
        void fadeVolume(std::string tag_name, float duration,
                        float target_volume);
        void fadeOut(Music source, float duration, float volume);
        void fadeIn(std::string name,
                    std::unordered_map<std::string, float> opts);
        void cleanup();
        bool isSoundPlaying(std::string sound);
        void playSound(std::string sound);
        void stopSound(std::string sound);
        float getSoundDuration(std::string sound);
        Music BGM = {0};
        float timeTrack = 0;

    private:
        bool soundsLoaded = false;
        std::deque<std::string> sounds_to_load;
        std::unordered_map<std::string, Sound> sounds;
        std::unordered_map<std::string, std::shared_ptr<SoundPlayer>> players;
        std::vector<std::string> songs;
        Timer timer;
        bool bgmInit = false;
        std::string currentBGM = "normal";
};
