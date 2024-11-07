#include "enemy_types.h"
#include "constants.h"
#include <unordered_map>

std::unordered_map<EnemyType, int> enemy_sizes;
std::unordered_map<EnemyType, float> enemy_damage;
std::unordered_map<EnemyType, float> enemy_hp;
std::unordered_map<EnemyType, float> enemy_atime;

void initEnemySizes() {
    enemy_sizes[EnemyType::ENEMY_TYPE_SHIELD_BEARER] = 10;
    enemy_sizes[EnemyType::GHOUL] = 12;
    enemy_sizes[EnemyType::NECROMANCER] = 12;
    enemy_sizes[EnemyType::RAT] = 10;
    enemy_sizes[EnemyType::SPIDER] = 17;
    enemy_sizes[EnemyType::SKELETON] = 10;
    enemy_sizes[EnemyType::SKELETON_PRINCE] = 15;
    enemy_sizes[EnemyType::BUCK] = 10;
    enemy_sizes[EnemyType::WOLF] = 13;
    enemy_sizes[EnemyType::ZOMBIE1] = 11;
    enemy_sizes[EnemyType::ZOMBIE2] = 12;
    enemy_sizes[EnemyType::ZOMBIE3] = 12;
    enemy_sizes[EnemyType::ZOMBIE4] = 13;
    enemy_sizes[EnemyType::ZOMBIE5] = 13;
    enemy_sizes[EnemyType::ZOMBIE6] = 14;
    enemy_sizes[EnemyType::ZOMBIE_GIANT] = 22;

    enemy_damage[EnemyType::ENEMY_TYPE_SHIELD_BEARER] = 3;
    enemy_damage[EnemyType::GHOUL] = 6;
    enemy_damage[EnemyType::NECROMANCER] = 12;
    enemy_damage[EnemyType::RAT] = 5;
    enemy_damage[EnemyType::SPIDER] = 10;
    enemy_damage[EnemyType::SKELETON] = 3;
    enemy_damage[EnemyType::SKELETON_PRINCE] = 10;
    enemy_damage[EnemyType::BUCK] = 4;
    enemy_damage[EnemyType::WOLF] = 7;
    enemy_damage[EnemyType::ZOMBIE1] = 3;
    enemy_damage[EnemyType::ZOMBIE2] = 3;
    enemy_damage[EnemyType::ZOMBIE3] = 3;
    enemy_damage[EnemyType::ZOMBIE4] = 3;
    enemy_damage[EnemyType::ZOMBIE5] = 3;
    enemy_damage[EnemyType::ZOMBIE6] = 5;
    enemy_damage[EnemyType::ZOMBIE_GIANT] = 22;

    enemy_hp[EnemyType::ENEMY_TYPE_SHIELD_BEARER] = 30;
    enemy_hp[EnemyType::GHOUL] = 40;
    enemy_hp[EnemyType::NECROMANCER] = 60;
    enemy_hp[EnemyType::RAT] = 20;
    enemy_hp[EnemyType::SPIDER] = 40;
    enemy_hp[EnemyType::SKELETON] = 10;
    enemy_hp[EnemyType::SKELETON_PRINCE] = 20;
    enemy_hp[EnemyType::BUCK] = 10;
    enemy_hp[EnemyType::WOLF] = 20;
    enemy_hp[EnemyType::ZOMBIE1] = 10;
    enemy_hp[EnemyType::ZOMBIE2] = 11;
    enemy_hp[EnemyType::ZOMBIE3] = 12;
    enemy_hp[EnemyType::ZOMBIE4] = 13;
    enemy_hp[EnemyType::ZOMBIE5] = 15;
    enemy_hp[EnemyType::ZOMBIE6] = 17;
    enemy_hp[EnemyType::ZOMBIE_GIANT] = 100;

    enemy_atime[EnemyType::ENEMY_TYPE_SHIELD_BEARER] = 1;
    enemy_atime[EnemyType::GHOUL] = 1;
    enemy_atime[EnemyType::NECROMANCER] = .8;
    enemy_atime[EnemyType::RAT] = .8;
    enemy_atime[EnemyType::SPIDER] = 1;
    enemy_atime[EnemyType::SKELETON] = .7;
    enemy_atime[EnemyType::SKELETON_PRINCE] = .5;
    enemy_atime[EnemyType::BUCK] = 1;
    enemy_atime[EnemyType::WOLF] = .8;
    enemy_atime[EnemyType::ZOMBIE1] = 1;
    enemy_atime[EnemyType::ZOMBIE2] = 1;
    enemy_atime[EnemyType::ZOMBIE3] = 1;
    enemy_atime[EnemyType::ZOMBIE4] = 1;
    enemy_atime[EnemyType::ZOMBIE5] = 1;
    enemy_atime[EnemyType::ZOMBIE6] = 1;
    enemy_atime[EnemyType::ZOMBIE_GIANT] = 5;
}

int get_enemy_size(EnemyType type) {
    return enemy_sizes[type];
}

float get_enemy_hp(EnemyType type) {
    return enemy_hp[type];
}

float get_enemy_damage(EnemyType type) {
    return enemy_damage[type];
}

float get_enemy_attack_time(EnemyType type) {
    return enemy_atime[type];
}

std::vector<std::string> get_enemy_sprite_ids(EnemyType type) {
    std::vector<std::string> sprite_ids;
    switch (type) {
    case EnemyType::ENEMY_TYPE_SHIELD_BEARER:
        sprite_ids = {ENEMY1_ATTACKS_SPRITE_ID, ENEMY1_DEAD_SPRITE_ID,
                      ENEMY1_RUN_SPRITE_ID};
        break;
    case EnemyType::GHOUL:
        sprite_ids = {GHOUL_ATTACKS_SPRITE_ID, GHOUL_DEAD_SPRITE_ID,
                      GHOUL_RUN_SPRITE_ID};
        break;
    case EnemyType::NECROMANCER:
        sprite_ids = {NECROMANCER_ATTACKS_SPRITE_ID, NECROMANCER_DEAD_SPRITE_ID,
                      NECROMANCER_RUN_SPRITE_ID};
        break;
    case EnemyType::RAT:
        sprite_ids = {RAT_ATTACKS_SPRITE_ID, RAT_DEAD_SPRITE_ID,
                      RAT_RUN_SPRITE_ID};
        break;
    case EnemyType::SPIDER:
        sprite_ids = {SPIDER_ATTACKS_SPRITE_ID, SPIDER_DEAD_SPRITE_ID,
                      SPIDER_RUN_SPRITE_ID};
        break;
    case EnemyType::SKELETON:
        sprite_ids = {SKELETON_ATTACKS_SPRITE_ID, SKELETON_DEAD_SPRITE_ID,
                      SKELETON_RUN_SPRITE_ID};
        break;
    case EnemyType::SKELETON_PRINCE:
        sprite_ids = {SKELETON_PRINCE_ATTACKS_SPRITE_ID,
                      SKELETON_PRINCE_DEAD_SPRITE_ID,
                      SKELETON_PRINCE_RUN_SPRITE_ID};
        break;
    case EnemyType::BUCK:
        sprite_ids = {BUCK_ATTACKS_SPRITE_ID, BUCK_DEAD_SPRITE_ID,
                      BUCK_RUN_SPRITE_ID};
        break;
    case EnemyType::WOLF:
        sprite_ids = {WOLF_ATTACKS_SPRITE_ID, WOLF_DEAD_SPRITE_ID,
                      WOLF_RUN_SPRITE_ID};
        break;
    case EnemyType::ZOMBIE1:
        sprite_ids = {ZOMBIE1_ATTACKS_SPRITE_ID, ZOMBIE1_DEAD_SPRITE_ID,
                      ZOMBIE1_RUN_SPRITE_ID};
        break;
    case EnemyType::ZOMBIE2:
        sprite_ids = {ZOMBIE2_ATTACKS_SPRITE_ID, ZOMBIE2_DEAD_SPRITE_ID,
                      ZOMBIE2_RUN_SPRITE_ID};
        break;
    case EnemyType::ZOMBIE3:
        sprite_ids = {ZOMBIE3_ATTACKS_SPRITE_ID, ZOMBIE3_DEAD_SPRITE_ID,
                      ZOMBIE3_RUN_SPRITE_ID};
        break;
    case EnemyType::ZOMBIE4:
        sprite_ids = {ZOMBIE4_ATTACKS_SPRITE_ID, ZOMBIE4_DEAD_SPRITE_ID,
                      ZOMBIE4_RUN_SPRITE_ID};
        break;
    case EnemyType::ZOMBIE5:
        sprite_ids = {ZOMBIE5_ATTACKS_SPRITE_ID, ZOMBIE5_DEAD_SPRITE_ID,
                      ZOMBIE5_RUN_SPRITE_ID};
        break;
    case EnemyType::ZOMBIE6:
        sprite_ids = {ZOMBIE6_ATTACKS_SPRITE_ID, ZOMBIE6_DEAD_SPRITE_ID,
                      ZOMBIE6_RUN_SPRITE_ID};
        break;
    case EnemyType::ZOMBIE_GIANT:
        sprite_ids = {ZOMBIE_GIANT_ATTACKS_SPRITE_ID,
                      ZOMBIE_GIANT_DEAD_SPRITE_ID, ZOMBIE_GIANT_RUN_SPRITE_ID};
        break;
    }
    return sprite_ids;
}
