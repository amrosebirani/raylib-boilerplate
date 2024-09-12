#include "enemy_types.h"
#include "constants.h"
#include <unordered_map>

std::unordered_map<EnemyType, int> enemy_sizes;

void initEnemySizes() {
    enemy_sizes[EnemyType::ENEMY_TYPE_SHIELD_BEARER] = 10;
}

int get_enemy_size(EnemyType type) {
    return enemy_sizes[type];
}

std::vector<std::string> get_enemy_sprite_ids(EnemyType type) {
    std::vector<std::string> sprite_ids;
    switch (type) {
    case EnemyType::ENEMY_TYPE_SHIELD_BEARER:
        sprite_ids = {ENEMY1_ATTACKS_SPRITE_ID, ENEMY1_DEAD_SPRITE_ID,
                      ENEMY1_RUN_SPRITE_ID};
        break;
    }
    return sprite_ids;
}
