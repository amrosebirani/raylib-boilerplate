#pragma once

#include <string>
#include <vector>

enum class EnemyType {
    ENEMY_TYPE_SHIELD_BEARER,
};

int get_enemy_size(EnemyType type);
void initEnemySizes();
std::vector<std::string> get_enemy_sprite_ids(EnemyType type);
