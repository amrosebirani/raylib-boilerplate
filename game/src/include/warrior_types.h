#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "direction.hpp"
#include "raylib.h"

enum class WarriorType {
    WARRIOR_TYPE_SWORDSMAN,
    WARRIOR_TYPE_SPEARMAN,
    WARRIOR_TYPE_ARCHER,
    WARRIOR_TYPE_JAVELINER,
    WARRIOR_TYPE_CROSSBOWMAN,
    // WARRIOR_TYPE_PIKEMAN,
    // WARRIOR_TYPE_CAVALRY,
    WARRIOR_TYPE_SHIELD_BEARER,
    WARRIOR_TYPE_BERSERKER,
    WARRIOR_TYPE_AXEMAN,
    WARRIOR_TYPE_KNIGHT,
    // WARRIOR_TYPE_WIZARD,
    // WARRIOR_TYPE_PRIEST,
    // WARRIOR_TYPE_WAR_ELEPHANT,
    // WARRIOR_TYPE_WAR_HORSE,
    // WARRIOR_TYPE_CHARIOT,
    WARRIOR_TYPE_GIANT,
    // WARRIOR_TYPE_CAVALRY_ARCHER,
    // WARRIOR_TYPE_LIGHT_CAVALRY,
};

int get_warrior_size(WarriorType type);
float get_warrior_damage(WarriorType type, bool inFormation = false);
float get_warrior_hp(WarriorType type, bool inFormation = false);
float get_warrior_attack_time(WarriorType type, bool inFormation = false);
float get_in_damage_multiplier(WarriorType type);
std::string getWarriorText(WarriorType type);
bool is_ranged_unit(WarriorType type);
bool is_infantry_unit(WarriorType type);
void initWarriorSizes();
std::vector<std::string> get_warrior_sprite_ids(WarriorType type);
std::unordered_map<Direction, int> get_direction_rows(WarriorType type);
int get_warrior_summon_card_cost(WarriorType type, int level);
void draw_warrior_icon(WarriorType type, Rectangle rect);
WarriorType get_random_infantry_type();
WarriorType get_random_ranged_type();
