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

std::vector<HordeType> horde_types_set_1 = {HordeType::SKELETON,
                                            HordeType::WOLF_AND_BUCK};
std::vector<HordeType> horde_types_set_2 = {
    HordeType::SKELETON, HordeType::WOLF_AND_BUCK, HordeType::RAT_AND_SPIDER};

std::vector<HordeType> horde_types_set_3 = {HordeType::WOLF_AND_BUCK,
                                            HordeType::RAT_AND_SPIDER,
                                            HordeType::SHIELD_BEARER};

std::vector<HordeType> horde_types_set_4 = {
    HordeType::WOLF_AND_BUCK, HordeType::RAT_AND_SPIDER,
    HordeType::SHIELD_BEARER, HordeType::ZOMBIE};
std::vector<HordeType> horde_types_set_5 = {
    HordeType::RAT_AND_SPIDER, HordeType::SHIELD_BEARER, HordeType::ZOMBIE,
    HordeType::GHOULD_AND_NECROMANCER};

HordeType getHordeTypeForLevel(int level) {
    if (level < 4) {
        return horde_types_set_1[rand() % horde_types_set_1.size()];
    }
    if (level < 9) {
        return horde_types_set_2[rand() % horde_types_set_2.size()];
    }
    if (level < 14) {
        return horde_types_set_3[rand() % horde_types_set_3.size()];
    }
    if (level < 21) {
        return horde_types_set_4[rand() % horde_types_set_4.size()];
    }
    return horde_types_set_5[rand() % horde_types_set_5.size()];
}

int getHordeCountForLevel(int level) {
    if (level < 3) {
        return 3;
    }
    if (level < 6) {
        return 4;
    }
    if (level < 9) {
        return 5;
    }
    if (level < 12) {
        return 6;
    }
    if (level < 15) {
        return 7;
    }
    if (level < 18) {
        return 8;
    }
    if (level < 21) {
        return 9;
    }
    if (level < 24) {
        return 10;
    }
    if (level < 27) {
        return 11;
    }
    return 12;
}

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
    if (level < 3) {
        return Vector2{12, 10};
    }
    if (level < 6) {
        return Vector2{14, 12};
    }
    if (level < 9) {
        return Vector2{16, 14};
    }
    if (level < 12) {
        return Vector2{18, 16};
    }
    if (level < 15) {
        return Vector2{20, 18};
    }
    if (level < 18) {
        return Vector2{22, 20};
    }
    return Vector2{24, 22};
}

std::vector<HordeType> getHordeTypesForLevel(int level) {
    return horde_types_for_level[level];
}

std::vector<HordeConfig *> getHordeConfigsForLevel(int level) {
    std::vector<HordeConfig *> horde_configs;
    float ra = randomFloatInRange(0, 2 * M_PI);
    HordeType ht = getHordeTypeForLevel(level);
    int hc = getHordeCountForLevel(level);
    float ac = 2 * M_PI / hc;
    for (int i = 0; i < hc; i++) {
        float angle = ra + ac * i;
        HordeConfig *config = getHordeConfig(ht);
        HordeConfig *local_conf =
            new HordeConfig(0, 0, config->unitType, config->leaderType,
                            config->leaderCount, angle, true);
        Vector2 cc = getHordeSizeBasedOnLevel(level);
        // add randomization in horde width and height
        local_conf->width = cc.x + getRandomIntInRange(-1, 3);
        local_conf->depth = cc.y + getRandomIntInRange(-1, 3);
        if (ht == HordeType::ZOMBIE) {
            local_conf->unitType =
                allZombieTypes[rand() % allZombieTypes.size()];
        }
        horde_configs.push_back(local_conf);
    }
    return horde_configs;
}
