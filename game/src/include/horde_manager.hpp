#pragma once

#include "timer.h"

class HordeManager {
    public:
        HordeManager();
        void update(float dt);
        void spawnHorde();
        void decreaseCount();

    private:
        int countToCheck = -1;
        Timer timer;
        int currentWave = 1;
};
