#pragma once
#include <memory>

struct EnemyWaveConfig {
        float time;
        int count;
        float delayAfterWave;
};

std::shared_ptr<EnemyWaveConfig> getEnemyWaveConfig(int wave_count);
