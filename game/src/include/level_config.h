#pragma once
#include <vector>
#include "warrior_types.h"

struct OrbitConfig {
        WarriorType type;
        int count;
};

struct LevelConfig {
        int level;
        std::vector<OrbitConfig *> orbits;
        WarriorType key_warrior;
        WarriorType expansion_warrior;
};

void initLevels();
LevelConfig *get_level_config(int level);
