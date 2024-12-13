#include "horde_config.hpp"
#include "enemy_types.h"
#include "utils.h"
#include <unordered_map>

std::unordered_map<HordeType, HordeConfig *> base_horde_configs;
std::unordered_map<int, std::vector<HordeType>> horde_types_for_level;
std::vector<EnemyType> allZombieTypes = {
    EnemyType::ZOMBIE2, EnemyType::ZOMBIE3, EnemyType::ZOMBIE4,
    EnemyType::ZOMBIE5, EnemyType::ZOMBIE6,
};

void initHordeConfigs() {
    base_horde_configs[HordeType::WOLF_AND_BUCK] =
        new HordeConfig(0, 0, EnemyType::BUCK, EnemyType::WOLF, 3);
    base_horde_configs[HordeType::RAT_AND_SPIDER] =
        new HordeConfig(0, 0, EnemyType::RAT, EnemyType::SPIDER, 2);
    base_horde_configs[HordeType::ZOMBIE] =
        new HordeConfig(0, 0, EnemyType::ZOMBIE1, EnemyType::ZOMBIE_GIANT, 1);
    base_horde_configs[HordeType::SKELETON] = new HordeConfig(
        0, 0, EnemyType::SKELETON, EnemyType::SKELETON_PRINCE, 4);
    base_horde_configs[HordeType::SHIELD_BEARER] =
        new HordeConfig(0, 0, EnemyType::ZOMBIE1, EnemyType::ZOMBIE1, 0);
    base_horde_configs[HordeType::GHOULD_AND_NECROMANCER] =
        new HordeConfig(0, 0, EnemyType::GHOUL, EnemyType::NECROMANCER, 2);

    horde_types_for_level[1] = {HordeType::SKELETON, HordeType::SKELETON};
    horde_types_for_level[2] = {HordeType::SKELETON, HordeType::SKELETON,
                                HordeType::WOLF_AND_BUCK};
    horde_types_for_level[3] = {HordeType::SKELETON, HordeType::SKELETON,
                                HordeType::WOLF_AND_BUCK};
    ;
    horde_types_for_level[4] = {HordeType::WOLF_AND_BUCK,
                                HordeType::RAT_AND_SPIDER,
                                HordeType::SHIELD_BEARER};
    horde_types_for_level[5] = {HordeType::WOLF_AND_BUCK,
                                HordeType::RAT_AND_SPIDER,
                                HordeType::SHIELD_BEARER, HordeType::ZOMBIE};
    horde_types_for_level[6] = {HordeType::WOLF_AND_BUCK,
                                HordeType::RAT_AND_SPIDER,
                                HordeType::SHIELD_BEARER, HordeType::ZOMBIE};
    horde_types_for_level[7] = {
        HordeType::ZOMBIE, HordeType::GHOULD_AND_NECROMANCER,
        HordeType::RAT_AND_SPIDER, HordeType::WOLF_AND_BUCK};
    horde_types_for_level[8] = {HordeType::GHOULD_AND_NECROMANCER,
                                HordeType::ZOMBIE, HordeType::SHIELD_BEARER,
                                HordeType::SKELETON};
    horde_types_for_level[9] = {HordeType::GHOULD_AND_NECROMANCER,
                                HordeType::ZOMBIE, HordeType::SHIELD_BEARER,
                                HordeType::SKELETON, HordeType::WOLF_AND_BUCK};

    horde_types_for_level[10] = {HordeType::GHOULD_AND_NECROMANCER,
                                 HordeType::ZOMBIE, HordeType::SHIELD_BEARER,
                                 HordeType::SKELETON, HordeType::WOLF_AND_BUCK};
    horde_types_for_level[11] = {
        HordeType::GHOULD_AND_NECROMANCER, HordeType::ZOMBIE,
        HordeType::SHIELD_BEARER,          HordeType::ZOMBIE,
        HordeType::RAT_AND_SPIDER,         HordeType::SKELETON,
        HordeType::WOLF_AND_BUCK};
}

HordeConfig *getHordeConfig(HordeType type) {
    return base_horde_configs[type];
}

Vector2 getHordeSizeBasedOnLevel(int level) {
    switch (level) {
    case 1:
        return Vector2{9, 11};
    case 2:
        return Vector2{12, 7};
    case 3:
        return Vector2{17, 5};
    case 4:
        return Vector2{9, 15};
    case 5:
        return Vector2{10, 12};
    case 6:
        return Vector2{10, 11};
    case 7:
        return Vector2{9, 15};
    case 8:
        return Vector2{15, 11};
    case 9:
        return Vector2{15, 15};
    case 10:
        return Vector2{10, 22};
    case 11:
        return Vector2{20, 10};
    default:
        return Vector2{3, 3};
    }
}

std::vector<HordeType> getHordeTypesForLevel(int level) {
    return horde_types_for_level[level];
}

std::vector<HordeConfig *> getHordeConfigsForLevel(int level) {
    std::vector<HordeConfig *> horde_configs;
    float ra = randomFloatInRange(0, 2 * M_PI);
    std::vector<HordeType> horde_types = getHordeTypesForLevel(level);
    float ac = 2 * M_PI / horde_types.size();
    int i = 0;
    for (auto type : horde_types) {
        float angle = ra + ac * i;
        HordeConfig *config = getHordeConfig(type);
        HordeConfig *local_conf =
            new HordeConfig(0, 0, config->unitType, config->leaderType,
                            config->leaderCount, angle, true);
        Vector2 cc = getHordeSizeBasedOnLevel(level);
        local_conf->width = cc.x;
        local_conf->depth = cc.y;
        if (type == HordeType::ZOMBIE) {
            local_conf->unitType =
                allZombieTypes[rand() % allZombieTypes.size()];
        }
        horde_configs.push_back(local_conf);
        i++;
    }
    return horde_configs;
}
