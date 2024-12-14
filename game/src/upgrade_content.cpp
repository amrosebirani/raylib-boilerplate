#include "upgrade_content.hpp"
#include "constants.h"
#include <memory>

std::vector<std::string> UpgradeContent::get_display_strs(std::string key) {
    return upgrade_entities[key]->display_strs;
}

void UpgradeContent::initStats() {
    upgrade_entities[HERO_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE, std::vector<std::string>{"Hero", "Health"});
    upgrade_entities[HERO_DAMAGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE, std::vector<std::string>{"Hero", "Damage"});
    upgrade_entities[HERO_SPEED_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE, std::vector<std::string>{"Hero", "Speed"});
    upgrade_entities[HERO_ATTACK_SPEED_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Hero", "Attack", "Speed"});
    upgrade_entities[HERO_RESPAWN_TIME_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Hero", "Respawn", "Time"});

    upgrade_entities[HERO_UNIT_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Hero Unit", "Health"});
    upgrade_entities[HERO_UNIT_DAMAGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Hero Unit", "Damage"});
    upgrade_entities[HERO_UNIT_ATTACK_SPEED_M] =
        std::make_shared<UpgradeEntity>(
            1, 0, StatType::PERCENTAGE,
            std::vector<std::string>{"Hero Unit", "Attack", "Speed"});
    upgrade_entities[HERO_UNIT_RESPAWN_TIME_M] =
        std::make_shared<UpgradeEntity>(
            1, 0, StatType::PERCENTAGE,
            std::vector<std::string>{"Hero Unit", "Respawn", "Time"});

    upgrade_entities[GOLD_PICKUP_RANGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Gold", "Pickup", "Range"});

    upgrade_entities[HOUSE_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"House", "Health"});
    upgrade_entities[BUILDING_UPGRADE_COST_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Building", "Upgrade", "Cost"});
    upgrade_entities[TRIBUTE_RATE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Tribute", "Rate"});
    upgrade_entities[CASTLE_ATTACK_RANGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Castle", "Attack", "Range"});
    upgrade_entities[CASTLE_ATTACK_SPEED_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Castle", "Attack", "Speed"});
    upgrade_entities[CASTLE_DAMAGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Castle", "Damage"});
    upgrade_entities[CASTLE_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Castle", "Health"});
    upgrade_entities[BARRACKS_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Barracks", "Health"});
    upgrade_entities[ARCHERY_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Archery", "Health"});
    upgrade_entities[DEFENSE_TOWER_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Defense", "Tower", "Health"});
    upgrade_entities[WIZARDRY_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Wizardry", "Health"});
    upgrade_entities[BARRACKS_PRODUCTION_RATE_M] =
        std::make_shared<UpgradeEntity>(
            1, 0, StatType::PERCENTAGE,
            std::vector<std::string>{"Barracks", "Production", "Rate"});
    upgrade_entities[ARCHERY_PRODUCTION_RATE_M] =
        std::make_shared<UpgradeEntity>(
            1, 0, StatType::PERCENTAGE,
            std::vector<std::string>{"Archery", "Production", "Rate"});
    upgrade_entities[WIZARDRY_PRODUCTION_RATE_M] =
        std::make_shared<UpgradeEntity>(
            1, 0, StatType::PERCENTAGE,
            std::vector<std::string>{"Wizardry", "Production", "Rate"});
    upgrade_entities[BARRACKS_UNIT_COST_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Barracks", "Unit", "Cost"});
    upgrade_entities[ARCHERY_UNIT_COST_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Archery", "Unit", "Cost"});
    upgrade_entities[WIZARDRY_UNIT_COST_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Wizardry", "Unit", "Cost"});
    upgrade_entities[DEFENSE_TOWER_RANGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Defense", "Tower", "Range"});

    upgrade_entities[INFANTRY_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Infantry", "Health"});
    upgrade_entities[INFANTRY_DAMAGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Infantry", "Damage"});
    upgrade_entities[INFANTRY_ATTACK_SPEED_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Infantry", "Attack", "Speed"});
    upgrade_entities[INFANTRY_MOVEMENT_SPEED_M] =
        std::make_shared<UpgradeEntity>(
            1, 0, StatType::PERCENTAGE,
            std::vector<std::string>{"Infantry", "Movement", "Speed"});

    upgrade_entities[RANGED_UNIT_HP_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Ranged Unit", "Health"});
    upgrade_entities[RANGED_UNIT_DAMAGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Ranged Unit", "Damage"});
    upgrade_entities[RANGED_UNIT_ATTACK_SPEED_M] =
        std::make_shared<UpgradeEntity>(
            1, 0, StatType::PERCENTAGE,
            std::vector<std::string>{"Ranged Unit", "Attack", "Speed"});
    upgrade_entities[RANGED_UNIT_RANGE_M] = std::make_shared<UpgradeEntity>(
        1, 0, StatType::PERCENTAGE,
        std::vector<std::string>{"Ranged Unit", "Range"});
}

void UpgradeContent::initChances(float lm) {
    upgrade_entities[HERO_DODGE_CHANCE_M] = std::make_shared<UpgradeEntity>(
        1, lm, std::vector<std::string>{"Hero", "Dodge", "Chance"});

    upgrade_entities[HERO_UNIT_DODGE_CHANCE_M] =
        std::make_shared<UpgradeEntity>(
            1, lm, std::vector<std::string>{"Hero Unit", "Dodge", "Chance"});

    upgrade_entities[INFANTRY_DODGE_CHANCE_M] = std::make_shared<UpgradeEntity>(
        1, lm, std::vector<std::string>{"Infantry", "Dodge", "Chance"});
}

UpgradeContent::UpgradeContent() {
    initStats();
    initChances(1);
}

float UpgradeContent::get_stat(std::string key) {
    return upgrade_entities[key]->stat->value;
}

int UpgradeContent::get_stat_count(std::string key) {
    return upgrade_entities[key]->stat->valueCount;
}

bool UpgradeContent::get_chance(std::string key) {
    return upgrade_entities[key]->chance_list->next() == "true" ? true : false;
}

void UpgradeContent::update_stat(std::string key, float value,
                                 std::string tag) {
    upgrade_entities[key]->stat->increase(value, tag);
}

void UpgradeContent::update_stat_count(std::string key, int value,
                                       std::string tag) {
    upgrade_entities[key]->stat->increaseCount(value, tag);
}

void UpgradeContent::clear_stat(std::string key, std::string tag) {
    upgrade_entities[key]->stat->clearStat(tag);
}

void UpgradeContent::update_chance(std::string key, int value) {
    int existing_chance =
        upgrade_entities[key]->chance_list->chance_def[0].second;
    int new_chance = existing_chance + value;
    upgrade_entities[key] = std::make_shared<UpgradeEntity>(
        new_chance, 1, upgrade_entities[key]->display_strs);
}
