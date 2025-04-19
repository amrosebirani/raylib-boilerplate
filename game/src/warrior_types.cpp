#include "warrior_types.h"
#include "constants.h"
#include "globals.h"
#include "magic_types.hpp"
#include "utils.h"
#include <unordered_map>

std::unordered_map<WarriorType, int> warrior_sizes;
std::unordered_map<WarriorType, float> warrior_hp;
std::unordered_map<WarriorType, float> warrior_damage;
std::unordered_map<WarriorType, float> warrior_atime;
std::unordered_map<WarriorType, float> warrior_in_damage_mult;

void initWarriorSizes() {
    warrior_sizes[WarriorType::WARRIOR_TYPE_KNIGHT] = 25;
    warrior_sizes[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 14;
    warrior_sizes[WarriorType::WARRIOR_TYPE_SPEARMAN] = 13;
    warrior_sizes[WarriorType::WARRIOR_TYPE_ARCHER] = 12;
    warrior_sizes[WarriorType::WARRIOR_TYPE_JAVELINER] = 12;
    warrior_sizes[WarriorType::WARRIOR_TYPE_CROSSBOWMAN] = 12;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_PIKEMAN] = 17;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_CAVALRY] = 20;
    warrior_sizes[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 12;
    warrior_sizes[WarriorType::WARRIOR_TYPE_BERSERKER] = 15;
    warrior_sizes[WarriorType::WARRIOR_TYPE_AXEMAN] = 16;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_WIZARD] = 10;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_PRIEST] = 10;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_WAR_ELEPHANT] = 30;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_WAR_HORSE] = 20;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_CHARIOT] = 35;
    warrior_sizes[WarriorType::WARRIOR_TYPE_GIANT] = 40;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_CAVALRY_ARCHER] = 20;
    // warrior_sizes[WarriorType::WARRIOR_TYPE_LIGHT_CAVALRY] = 20;

    warrior_hp[WarriorType::WARRIOR_TYPE_KNIGHT] = 1000;
    warrior_hp[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 70;
    warrior_hp[WarriorType::WARRIOR_TYPE_SPEARMAN] = 70;
    warrior_hp[WarriorType::WARRIOR_TYPE_ARCHER] = 50;
    warrior_hp[WarriorType::WARRIOR_TYPE_JAVELINER] = 60;
    warrior_hp[WarriorType::WARRIOR_TYPE_CROSSBOWMAN] = 60;
    warrior_hp[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 100;
    // warrior_hp[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 15;
    warrior_hp[WarriorType::WARRIOR_TYPE_BERSERKER] = 50;
    warrior_hp[WarriorType::WARRIOR_TYPE_AXEMAN] = 80;
    warrior_hp[WarriorType::WARRIOR_TYPE_GIANT] = 2000;

    warrior_damage[WarriorType::WARRIOR_TYPE_KNIGHT] = 40;
    warrior_damage[WarriorType::WARRIOR_TYPE_SPEARMAN] = 10;
    warrior_damage[WarriorType::WARRIOR_TYPE_ARCHER] = 7;
    warrior_damage[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 8;
    warrior_damage[WarriorType::WARRIOR_TYPE_BERSERKER] = 20;
    warrior_damage[WarriorType::WARRIOR_TYPE_JAVELINER] = 13;
    warrior_damage[WarriorType::WARRIOR_TYPE_AXEMAN] = 15;
    warrior_damage[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 15;
    warrior_damage[WarriorType::WARRIOR_TYPE_GIANT] = 60;
    warrior_damage[WarriorType::WARRIOR_TYPE_CROSSBOWMAN] = 15;

    warrior_atime[WarriorType::WARRIOR_TYPE_KNIGHT] = .7;
    warrior_atime[WarriorType::WARRIOR_TYPE_SPEARMAN] = 1.5;
    warrior_atime[WarriorType::WARRIOR_TYPE_ARCHER] = 1.5;
    warrior_atime[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = 1;
    warrior_atime[WarriorType::WARRIOR_TYPE_BERSERKER] = .5;
    warrior_atime[WarriorType::WARRIOR_TYPE_JAVELINER] = 1.8;
    warrior_atime[WarriorType::WARRIOR_TYPE_AXEMAN] = 1.1;
    warrior_atime[WarriorType::WARRIOR_TYPE_SWORDSMAN] = .9;
    warrior_atime[WarriorType::WARRIOR_TYPE_GIANT] = 2;
    warrior_atime[WarriorType::WARRIOR_TYPE_CROSSBOWMAN] = 2.0;

    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_KNIGHT] = 1.0;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_SPEARMAN] = 1.0;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_ARCHER] = 1.0;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_SHIELD_BEARER] = .7;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_BERSERKER] = 1.1;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_JAVELINER] = 1.1;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_AXEMAN] = .8;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_SWORDSMAN] = 1.0;
    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_CROSSBOWMAN] = 1.0;

    warrior_in_damage_mult[WarriorType::WARRIOR_TYPE_GIANT] = 1.0;
}

int get_warrior_size(WarriorType type) {
    return warrior_sizes[type];
}

float get_warrior_hp(WarriorType type, bool inFormation) {
    float mm = 1;
    if (inFormation) {
        if (type == WarriorType::WARRIOR_TYPE_KNIGHT) {
            mm = getContainer()->getUpgradeContent()->get_stat(HERO_HP_M);
        } else {
            mm = getContainer()->getUpgradeContent()->get_stat(HERO_UNIT_HP_M);
        }
    } else {
        if (is_infantry_unit(type)) {
            mm = getContainer()->getUpgradeContent()->get_stat(INFANTRY_HP_M);
        }
        if (is_ranged_unit(type)) {
            mm =
                getContainer()->getUpgradeContent()->get_stat(RANGED_UNIT_HP_M);
        }
    }
    return warrior_hp[type] * mm;
}

bool is_infanty_unit(WarriorType type) {
    if (type == WarriorType::WARRIOR_TYPE_SWORDSMAN ||
        type == WarriorType::WARRIOR_TYPE_SPEARMAN ||
        type == WarriorType::WARRIOR_TYPE_SHIELD_BEARER ||
        type == WarriorType::WARRIOR_TYPE_BERSERKER ||
        type == WarriorType::WARRIOR_TYPE_KNIGHT ||
        type == WarriorType::WARRIOR_TYPE_AXEMAN) {
        return true;
    }
    return false;
}

bool is_ranged_unit(WarriorType type) {
    if (type == WarriorType::WARRIOR_TYPE_ARCHER ||
        type == WarriorType::WARRIOR_TYPE_JAVELINER ||
        type == WarriorType::WARRIOR_TYPE_CROSSBOWMAN) {
        return true;
    }
    return false;
}

float get_warrior_damage(WarriorType type, bool inFormation) {
    float mm = 1;
    if (inFormation) {
        if (type == WarriorType::WARRIOR_TYPE_KNIGHT) {
            mm = getContainer()->getUpgradeContent()->get_stat(HERO_DAMAGE_M);
        } else {
            mm = getContainer()->getUpgradeContent()->get_stat(
                HERO_UNIT_DAMAGE_M);
        }
    } else {
        if (is_infantry_unit(type)) {
            mm = getContainer()->getUpgradeContent()->get_stat(
                INFANTRY_DAMAGE_M);
        }
        if (is_ranged_unit(type)) {
            mm = getContainer()->getUpgradeContent()->get_stat(
                RANGED_UNIT_DAMAGE_M);
        }
    }
    return warrior_damage[type] * mm;
}

float get_warrior_attack_time(WarriorType type, bool inFormation) {
    float mm = 1;
    if (inFormation) {
        if (type == WarriorType::WARRIOR_TYPE_KNIGHT) {
            mm = getContainer()->getUpgradeContent()->get_stat(
                HERO_ATTACK_SPEED_M);
        } else {
            mm = getContainer()->getUpgradeContent()->get_stat(
                HERO_UNIT_ATTACK_SPEED_M);
        }
    } else {
        if (is_infantry_unit(type)) {
            mm = getContainer()->getUpgradeContent()->get_stat(
                INFANTRY_ATTACK_SPEED_M);
        }
        if (is_ranged_unit(type)) {
            mm = getContainer()->getUpgradeContent()->get_stat(
                RANGED_UNIT_ATTACK_SPEED_M);
        }
    }
    return warrior_atime[type] / mm;
}

float get_in_damage_multiplier(WarriorType type) {
    return warrior_in_damage_mult[type];
}

std::vector<std::string> get_warrior_sprite_ids(WarriorType type) {
    std::vector<std::string> sprite_ids;
    switch (type) {
    case WarriorType::WARRIOR_TYPE_KNIGHT:
        sprite_ids = {KNIGHT_IDLE_SPRITE_ID, KNIGHT_ATTACKS_SPRITE_ID,
                      KNIGHT_DEAD_SPRITE_ID, KNIGHT_RUN_SPRITE_ID,
                      KNIGHT_BUST_SPRITE_ID, KNIGHT_PAPERDOLL_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_SPEARMAN:
        sprite_ids = {SPEARMAN_IDLE_SPRITE_ID, SPEARMAN_ATTACKS_SPRITE_ID,
                      SPEARMAN_DEAD_SPRITE_ID, SPEARMAN_RUN_SPRITE_ID,
                      SPEARMAN_BUST_SPRITE_ID, SPEARMAN_PAPERDOLL_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_SHIELD_BEARER:
        sprite_ids = {FOOTMAN_IDLE_SPRITE_ID, FOOTMAN_ATTACKS_SPRITE_ID,
                      FOOTMAN_DEAD_SPRITE_ID, FOOTMAN_RUN_SPRITE_ID,
                      FOOTMAN_BUST_SPRITE_ID, FOOTMAN_PAPERDOLL_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_ARCHER:
        sprite_ids = {ARCHER_IDLE_SPRITE_ID, ARCHER_ATTACKS_SPRITE_ID,
                      ARCHER_IDLE_SPRITE_ID, ARCHER_IDLE_SPRITE_ID,
                      ARCHER_BUST_SPRITE_ID, ARCHER_BUST_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_BERSERKER:
        sprite_ids = {BERSERKER_IDLE_SPRITE_ID, BERSERKER_ATTACKS_SPRITE_ID,
                      BERSERKER_DEAD_SPRITE_ID, BERSERKER_RUN_SPRITE_ID,
                      BERSERKER_BUST_SPRITE_ID, BERSERKER_PAPERDOLL_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_JAVELINER:
        sprite_ids = {JAVELINER_IDLE_SPRITE_ID, JAVELINER_ATTACKS_SPRITE_ID,
                      JAVELINER_DEAD_SPRITE_ID, JAVELINER_RUN_SPRITE_ID,
                      JAVELINER_BUST_SPRITE_ID, JAVELINER_PAPERDOLL_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_AXEMAN:
        sprite_ids = {AXEMAN_IDLE_SPRITE_ID, AXEMAN_ATTACKS_SPRITE_ID,
                      AXEMAN_DEAD_SPRITE_ID, AXEMAN_RUN_SPRITE_ID,
                      AXEMAN_BUST_SPRITE_ID, AXEMAN_PAPERDOLL_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_CROSSBOWMAN:
        sprite_ids = {
            CROSSBOWMAN_IDLE_SPRITE_ID, CROSSBOWMAN_ATTACKS_SPRITE_ID,
            CROSSBOWMAN_DEAD_SPRITE_ID, CROSSBOWMAN_RUN_SPRITE_ID,
            CROSSBOWMAN_BUST_SPRITE_ID, CROSSBOWMAN_PAPERDOLL_SPRITE_ID};
        break;
    case WarriorType::WARRIOR_TYPE_SWORDSMAN:
        sprite_ids = {SWORDSMAN_IDLE_SPRITE_ID, SWORDSMAN_ATTACKS_SPRITE_ID,
                      SWORDSMAN_DEAD_SPRITE_ID, SWORDSMAN_RUN_SPRITE_ID,
                      SWORDSMAN_BUST_SPRITE_ID, SWORDSMAN_PAPERDOLL_SPRITE_ID};
    default:
        break;
    }
    return sprite_ids;
}

int get_warrior_summon_card_cost(WarriorType type, int level) {
    switch (type) {
    case WarriorType::WARRIOR_TYPE_SWORDSMAN:
        return 80;
    case WarriorType::WARRIOR_TYPE_SPEARMAN:
        return 50;
    case WarriorType::WARRIOR_TYPE_SHIELD_BEARER:
        return 50;
    case WarriorType::WARRIOR_TYPE_BERSERKER:
        return 70;
    case WarriorType::WARRIOR_TYPE_JAVELINER:
        return 60;
    case WarriorType::WARRIOR_TYPE_ARCHER:
        return 50;
    case WarriorType::WARRIOR_TYPE_AXEMAN:
        return 80;
    case WarriorType::WARRIOR_TYPE_CROSSBOWMAN:
        return 60;
    default:
        return 100;
    }
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

std::string getWarriorText(WarriorType type) {
    switch (type) {
    case WarriorType::WARRIOR_TYPE_SWORDSMAN:
        return "Swordsman";
    case WarriorType::WARRIOR_TYPE_SPEARMAN:
        return "Spearman";
    case WarriorType::WARRIOR_TYPE_SHIELD_BEARER:
        return "ShieldBearer";
    case WarriorType::WARRIOR_TYPE_BERSERKER:
        return "Berserker";
    case WarriorType::WARRIOR_TYPE_JAVELINER:
        return "Javeliner";
    case WarriorType::WARRIOR_TYPE_ARCHER:
        return "Archer";
    case WarriorType::WARRIOR_TYPE_AXEMAN:
        return "Axeman";
    case WarriorType::WARRIOR_TYPE_CROSSBOWMAN:
        return "CrossBowman";
    default:
        return "Unknown";
    }
}

void draw_warrior_icon(WarriorType type, Rectangle rect) {
    int sprite_no = 0;
    switch (type) {
    case WarriorType::WARRIOR_TYPE_SPEARMAN:
        sprite_no = 0;
        break;
    case WarriorType::WARRIOR_TYPE_SHIELD_BEARER:
        sprite_no = 1;
        break;
    case WarriorType::WARRIOR_TYPE_AXEMAN:
        sprite_no = 2;
        break;
    case WarriorType::WARRIOR_TYPE_CROSSBOWMAN:
        sprite_no = 3;
        break;
    case WarriorType::WARRIOR_TYPE_ARCHER:
        sprite_no = 4;
        break;
    case WarriorType::WARRIOR_TYPE_BERSERKER:
        sprite_no = 5;
        break;
    case WarriorType::WARRIOR_TYPE_SWORDSMAN:
        sprite_no = 6;
        break;
    case WarriorType::WARRIOR_TYPE_JAVELINER:
        sprite_no = 0;
        break;
    default:
        sprite_no = 0;
        break;
    }
    getSpriteHolder()->drawSprite(WARRIOR_ICONS, sprite_no, rect);
}

WarriorType get_random_infantry_type() {
    std::vector<WarriorType> all_inf = {
        WarriorType::WARRIOR_TYPE_AXEMAN, WarriorType::WARRIOR_TYPE_SPEARMAN,
        WarriorType::WARRIOR_TYPE_BERSERKER,
        WarriorType::WARRIOR_TYPE_SWORDSMAN,
        WarriorType::WARRIOR_TYPE_SHIELD_BEARER};
    int ind = getRandomIntInRange(0, 4);
    return all_inf[ind];
}

WarriorType get_random_ranged_type() {
    std::vector<WarriorType> all_ran = {WarriorType::WARRIOR_TYPE_JAVELINER,
                                        WarriorType::WARRIOR_TYPE_ARCHER,
                                        WarriorType::WARRIOR_TYPE_CROSSBOWMAN};
    int ind = getRandomIntInRange(0, 2);
    return all_ran[ind];
}
