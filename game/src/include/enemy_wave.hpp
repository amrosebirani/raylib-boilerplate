#pragma once

#include "timer.h"
#include "game_object.h"

class EnemyWave {
    public:
        EnemyWave(float time, int count, std::shared_ptr<GameObject> castle);
        void update(float dt);
        void spawnEnemy();
        bool isFinishedWave();

    private:
        float time;
        int count;
        Timer timer;
        std::shared_ptr<GameObject> castle;
        bool isFinished = false;
};
