#pragma once

#include "enemy_types.h"
#include "raylib.h"

enum class HordeType {
    WOLF_AND_BUCK,
    RAT_AND_SPIDER,
    ZOMBIE,
    SKELETON,
    SHIELD_BEARER,
    GHOULD_AND_NECROMANCER,
};

struct HordeConfig {
        int width;
        int depth;
        EnemyType unitType;
        EnemyType leaderType;
        int leaderCount;
        bool hasLeader;
        float horde_angle;

        HordeConfig(int width, int depth, EnemyType unitType,
                    EnemyType leaderType, int leaderCount,
                    float horde_angle = 0, bool hasLeader = true)
            : width(width), depth(depth), unitType(unitType),
              leaderType(leaderType), leaderCount(leaderCount),
              horde_angle(horde_angle), hasLeader(hasLeader) {

              };
};

void initHordeConfigs();
HordeConfig *getHordeConfig(HordeType type);
Vector2 getHordeSizeBasedOnLevel(int level);
std::vector<HordeType> getHordeTypesForLevel(int level);
std::vector<HordeConfig *> getHordeConfigsForLevel(int level);
