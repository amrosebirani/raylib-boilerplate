#pragma once

#include "property_type.hpp"
#include "timer.h"

class HordeManager {
    public:
        HordeManager(int totalWaves = 11);
        HordeManager(std::ifstream &in);
        void update(float dt);
        void spawnHorde();
        void decreaseCount();
        bool isWaveActive();
        int getCurrentWave() {
            return currentWave;
        }
        std::string getWaveText() {
            if (totalWaves == -1) {
                return "Wave: " + std::to_string(currentWave);
            }
            return "Wave: " + std::to_string(currentWave) + "/" +
                   std::to_string(totalWaves);
        }
        bool isVictory();
        void launchTutBoxes();
        void launchTutorial(PropertyType type, std::string text,
                            std::string sprite_id);
        void Save(std::ofstream &out) const;

    private:
        int countToCheck = -1;
        Timer timer;
        int currentWave = 1;
        int totalWaves;
        bool victory = false;
};
