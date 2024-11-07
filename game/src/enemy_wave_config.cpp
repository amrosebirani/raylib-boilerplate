#include "enemy_wave_config.hpp"

std::shared_ptr<EnemyWaveConfig> getEnemyWaveConfig(int wave_count) {
    std::shared_ptr<EnemyWaveConfig> ewc = std::make_shared<EnemyWaveConfig>();
    ewc->time = std::min(wave_count * 28, 256);
    ewc->count = std::min(wave_count * 10 + 30, 400);
    ewc->delayAfterWave = 40.0f;
    return ewc;
}
