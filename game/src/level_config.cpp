#include "level_config.h"
#include "warrior_types.h"
#include <unordered_map>

std::unordered_map<int, LevelConfig *> level_configs;

void initLevels() {
    LevelConfig *lv = new LevelConfig();
    lv->level = 1;
    lv->key_warrior = WarriorType::WARRIOR_TYPE_SWORDSMAN;
    lv->expansion_warrior = WarriorType::WARRIOR_TYPE_SPEARMAN;
    OrbitConfig *orb11 = new OrbitConfig();
    orb11->type = WarriorType::WARRIOR_TYPE_SHIELD_BEARER;
    orb11->count = 6;
    lv->orbits.push_back(orb11);
    level_configs[1] = lv;
}

LevelConfig *get_level_config(int level) {
    return level_configs[level];
}
