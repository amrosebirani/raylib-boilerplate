#include "horde_manager.hpp"
#include "enemy_horde.hpp"
#include "horde_config.hpp"
#include "timer.h"

HordeManager::HordeManager() {
    timer = Timer();
    timer.after(1, [this](float dt) { this->spawnHorde(); }, "");
}

void HordeManager::update(float dt) {
    timer.update(dt);
    if (countToCheck == 0) {
        countToCheck = -1;
        currentWave++;
        if (currentWave > 11) {
            return;
        }
        timer.after(0, [this](float dt) { this->spawnHorde(); }, "");
    }
}

void HordeManager::spawnHorde() {
    std::vector<HordeConfig *> hordeConfigs =
        getHordeConfigsForLevel(currentWave);
    int totalEnemyCount = 0;
    for (auto &hordeConfig : hordeConfigs) {
        totalEnemyCount +=
            hordeConfig->width * hordeConfig->depth + hordeConfig->leaderCount;
        EnemyHorde *eh = new EnemyHorde(hordeConfig);
        delete eh;
    }
    countToCheck = totalEnemyCount;
}

void HordeManager::decreaseCount() {
    countToCheck--;
}
