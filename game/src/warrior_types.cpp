#include "warrior_types.h"
#include "constants.h"
#include <unordered_map>

std::unordered_map<WarriorType, int> warrior_sizes;

void initWarriorSizes() {
    warrior_sizes[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 15;
    warrior_sizes[WarriorType::WARRIOR_TYPE_SPEARMAN] = 12;
    warrior_sizes[WarriorType::WARRIOR_TYPE_ARCHER] = 10;
    warrior_sizes[WarriorType::WARRIOR_TYPE_JAVELINER] = 15;
    warrior_sizes[WarriorType::WARRIOR_TYPE_PIKEMAN] = 17;
    warrior_sizes[WarriorType::WARRIOR_TYPE_CAVALRY] = 20;
    warrior_sizes[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 12;
    warrior_sizes[WarriorType::WARRIOR_TYPE_BERSERKER] = 15;
    warrior_sizes[WarriorType::WARRIOR_TYPE_WIZARD] = 10;
    warrior_sizes[WarriorType::WARRIOR_TYPE_PRIEST] = 10;
    warrior_sizes[WarriorType::WARRIOR_TYPE_WAR_ELEPHANT] = 30;
    warrior_sizes[WarriorType::WARRIOR_TYPE_WAR_HORSE] = 20;
    warrior_sizes[WarriorType::WARRIOR_TYPE_CHARIOT] = 35;
    warrior_sizes[WarriorType::WARRIOR_TYPE_GIANT] = 40;
    warrior_sizes[WarriorType::WARRIOR_TYPE_CAVALRY_ARCHER] = 20;
    warrior_sizes[WarriorType::WARRIOR_TYPE_LIGHT_CAVALRY] = 20;
}

int get_warrior_size(WarriorType type) {
    return warrior_sizes[type];
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
