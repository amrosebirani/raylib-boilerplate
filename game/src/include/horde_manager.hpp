#pragma once

#include "timer.h"

class HordeManager {
    public:
        HordeManager(int totalWaves = 11);
        void update(float dt);
        void spawnHorde();
        void decreaseCount();
        bool isWaveActive();
        std::string getWaveText() {
            return "Wave: " + std::to_string(currentWave) + "/" +
                   std::to_string(totalWaves);
        }
        bool isVictory();

    private:
        int countToCheck = -1;
        Timer timer;
        int currentWave = 1;
        int totalWaves;
        bool victory = false;
};
