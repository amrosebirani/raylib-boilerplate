#include "warrior_types.h"
#include "constants.h"
#include <unordered_map>

std::unordered_map<WarriorType, int> warrior_sizes;
std::unordered_map<WarriorType, float> warrior_hp;
std::unordered_map<WarriorType, float> warrior_damage;
std::unordered_map<WarriorType, float> warrior_atime;
std::unordered_map<WarriorType, float> warrior_in_damage_mult;

void initWarriorSizes() {
    warrior_sizes[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 25;
    warrior_sizes[WarriorType::WARRIOR_TYPE_SPEARMAN] = 12;
    warrior_sizes[WarriorType::WARRIOR_TYPE_ARCHER] = 15;
    warrior_sizes[WarriorType::WARRIOR_TYPE_JAVELINER] = 15;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_PIKEMAN] = 17;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_CAVALRY] = 20;
    warrior_sizes[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 12;
    warrior_sizes[WarriorType::WARRIOR_TYPE_BERSERKER] = 15;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_WIZARD] = 10;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_PRIEST] = 10;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_WAR_ELEPHANT] = 30;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_WAR_HORSE] = 20;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_CHARIOT] = 35;
    warrior_sizes[WarriorType::WARRIOR_TYPE_GIANT] = 40;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_CAVALRY_ARCHER] = 20;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_LIGHT_CAVALRY] = 20;

    warrior_hp[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 1000;
    // warrior_hp[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 60;
    warrior_hp[WarriorType::WARRIOR_TYPE_SPEARMAN] = 100;
    warrior_hp[WarriorType::WARRIOR_TYPE_ARCHER] = 70;
    warrior_hp[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 150;
    // warrior_hp[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 15;
    warrior_hp[WarriorType::WARRIOR_TYPE_BERSERKER] = 70;
    warrior_hp[WarriorType::WARRIOR_TYPE_JAVELINER] = 80;
    warrior_hp[WarriorType::WARRIOR_TYPE_GIANT] = 2000;

    warrior_damage[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 40;
    warrior_damage[WarriorType::WARRIOR_TYPE_SPEARMAN] = 10;
    warrior_damage[WarriorType::WARRIOR_TYPE_ARCHER] = 7;
    warrior_damage[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 8;
    warrior_damage[WarriorType::WARRIOR_TYPE_BERSERKER] = 10;
    warrior_damage[WarriorType::WARRIOR_TYPE_JAVELINER] = 8;
    warrior_damage[WarriorType::WARRIOR_TYPE_GIANT] = 60;

    warrior_atime[WarriorType::WARRIOR_TYPE_SWORDSMAN] = .7;
    warrior_atime[WarriorType::WARRIOR_TYPE_SPEARMAN] = 1.5;
    warrior_atime[WarriorType::WARRIOR_TYPE_ARCHER] = 1.5;
    warrior_atime[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 1;
    warrior_atime[WarriorType::WARRIOR_TYPE_BERSERKER] = .5;
    warrior_atime[WarriorType::WARRIOR_TYPE_JAVELINER] = 1.5;
    warrior_atime[WarriorType::WARRIOR_TYPE_GIANT] = 2;

    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 1.0;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_SPEARMAN] = 1.0;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_ARCHER] = 1.0;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = .9;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_BERSERKER] = 1.1;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_JAVELINER] = 1.1;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_GIANT] = 1.0;
}

int get_warrior_size(WarriorType type) {
    return warrior_sizes[type];
}

float get_warrior_hp(WarriorType type) {
    return warrior_hp[type];
}

float get_warrior_damage(WarriorType type) {
    return warrior_damage[type];
}

float get_warrior_attack_time(WarriorType type) {
    return warrior_atime[type];
}

float get_in_damage_multiplier(WarriorType type) {
    return warrior_in_damage_mult[type];
}

std::vector<std::string> get_warrior_sprite_ids(WarriorType type) {
    std::vector<std::string> sprite_ids;
    switch (type) {
    case WarriorType::WARRIOR_TYPE_SWORDSMAN:
        sprite_ids = {KNIGHT_IDLE_SPRITE_ID, KNIGHT_ATTACKS_SPRITE_ID,
                      KNIGHT_DEAD_SPRITE_ID, KNIGHT_RUN_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_SPEARMAN:
        sprite_ids = {SPEARMAN_IDLE_SPRITE_ID, SPEARMAN_ATTACKS_SPRITE_ID,
                      SPEARMAN_DEAD_SPRITE_ID, SPEARMAN_RUN_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_SHIELD_BEARER:
        sprite_ids = {FOOTMAN_IDLE_SPRITE_ID, FOOTMAN_ATTACKS_SPRITE_ID,
                      FOOTMAN_DEAD_SPRITE_ID, FOOTMAN_RUN_SPRITE_ID};
    default:
        break;
    }
    return sprite_ids;
}

std::unordered_map<Direction, int> get_direction_rows(WarriorType type) {
    std::unordered_map<Direction, int> direction_rows;
    direction_rows[Direction::NORTH] = 3;
    direction_rows[Direction::NORTH_EAST] = 7;
    direction_rows[Direction::EAST] = 2;
    direction_rows[Direction::SOUTH_EAST] = 6;
    direction_rows[Direction::SOUTH] = 0;
    direction_rows[Direction::SOUTH_WEST] = 4;
    direction_rows[Direction::WEST] = 1;
    direction_rows[Direction::NORTH_WEST] = 5;
    return direction_rows;
}
