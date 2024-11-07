#pragma once

#include <string>
#include <vector>

enum class EnemyType {
    ENEMY_TYPE_SHIELD_BEARER,
    GHOUL,
    NECROMANCER,
    RAT,
    SPIDER,
    SKELETON,
    SKELETON_PRINCE,
    BUCK,
    WOLF,
    ZOMBIE1,
    ZOMBIE2,
    ZOMBIE3,
    ZOMBIE4,
    ZOMBIE5,
    ZOMBIE6,
    ZOMBIE_GIANT
};

int get_enemy_size(EnemyType type);
void initEnemySizes();
std::vector<std::string> get_enemy_sprite_ids(EnemyType type);
float get_enemy_damage(EnemyType type);
float get_enemy_hp(EnemyType type);
float get_enemy_attack_time(EnemyType type);
