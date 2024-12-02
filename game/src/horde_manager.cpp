#include "horde_manager.hpp"
#include "enemy_horde.hpp"
#include "globals.h"
#include "horde_config.hpp"
#include "timer.h"

HordeManager::HordeManager(int totalWaves) : totalWaves(totalWaves) {
    timer = Timer();
    timer.after(30, [this](float dt) { this->spawnHorde(); }, "");
}

void HordeManager::update(float dt) {
    if (victory) {
        return;
    }
    timer.update(dt);
    // std::cout << "countToCheck: " << countToCheck << std::endl;
    if (countToCheck == 0) {
        countToCheck = -1;
        currentWave++;
        if (currentWave > totalWaves) {
            victory = true;
            return;
        }
        getAudioManager()->switchBGM("normal");
        timer.after(39, [this](float dt) { this->spawnHorde(); }, "");
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
    float hh = getContainer()->getCastleHealth();
    if (hh < 0.5) {
        getAudioManager()->switchBGM("under_attack_fucked");
    } else {
        getAudioManager()->switchBGM("under_attack_in_control");
    }
}

void HordeManager::decreaseCount() {
    countToCheck--;
}

bool HordeManager::isWaveActive() {
    return countToCheck > 0;
}

bool HordeManager::isVictory() {
    return victory;
}
