#pragma once

#include "timer.h"
#include "game_object.h"
#include "temp_chance_list.hpp"
#include "enemy_types.h"

class EnemyWave {
    public:
        EnemyWave(float time, int count, std::shared_ptr<GameObject> castle);
        ~EnemyWave();
        void update(float dt);
        void spawnEnemy();
        void createEnemy(float xd, float yd);
        bool isFinishedWave();

    private:
        float time;
        int count;
        Timer timer;
        std::shared_ptr<GameObject> castle;
        bool isFinished = false;
        TempChanceList<EnemyType> *enemy_chance_list;
};
