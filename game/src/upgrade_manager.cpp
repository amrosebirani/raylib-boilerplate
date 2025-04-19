#include "upgrade_manager.hpp"
#include "constants.h"
#include "globals.h"
#include "magic_types.hpp"
#include "summon_card.hpp"
#include "utils.h"
#include "warrior_types.h"

UpgradeManager::UpgradeManager() {

    hero_upgrades = {HERO_HP_M,
                     HERO_DAMAGE_M,
                     HERO_SPEED_M,
                     HERO_ATTACK_SPEED_M,
                     HERO_RESPAWN_TIME_M,
                     HERO_UNIT_HP_M,
                     HERO_UNIT_DAMAGE_M,
                     HERO_UNIT_ATTACK_SPEED_M,
                     HERO_UNIT_RESPAWN_TIME_M,
                     GOLD_PICKUP_RANGE_M};

    barrack_upgrades = {
        BARRACKS_HP_M,
        BARRACKS_PRODUCTION_RATE_M,
        BARRACKS_UNIT_COST_M,
        INFANTRY_HP_M,
        INFANTRY_DAMAGE_M,
        INFANTRY_ATTACK_SPEED_M,
        INFANTRY_MOVEMENT_SPEED_M,
    };

    archer_upgrades = {
        ARCHERY_HP_M,         ARCHERY_PRODUCTION_RATE_M,
        ARCHERY_UNIT_COST_M,  RANGED_UNIT_HP_M,
        RANGED_UNIT_DAMAGE_M, RANGED_UNIT_ATTACK_SPEED_M,
        RANGED_UNIT_RANGE_M,
    };

    wizardry_upgrades = {
        WIZARDRY_HP_M,
        WIZARDRY_PRODUCTION_RATE_M,
        WIZARDRY_UNIT_COST_M,
    };

    castle_upgrades = {
        CASTLE_HP_M,
        CASTLE_DAMAGE_M,
        CASTLE_ATTACK_SPEED_M,
        CASTLE_ATTACK_RANGE_M,
    };

    other_building_upgrades = {
        HOUSE_HP_M,         BUILDING_UPGRADE_COST_M, TRIBUTE_RATE_M,
        DEFENSE_TOWER_HP_M, DEFENSE_TOWER_RANGE_M,

    };

    chance_upgrades = {
        HERO_DODGE_CHANCE_M,
        HERO_UNIT_DODGE_CHANCE_M,
        INFANTRY_DODGE_CHANCE_M,
    };

    for (auto &upgrade : hero_upgrades) {
        hero_u.emplace_back(std::make_shared<Upgrade>(10, 10, upgrade));
    }
    hero_u[4]->total_levels = 5;
    hero_u[4]->rate_of_change = -10;
    hero_u[8]->total_levels = 5;
    hero_u[8]->rate_of_change = -5;

    for (auto &upgrade : barrack_upgrades) {
        barrack_u.emplace_back(std::make_shared<Upgrade>(10, 10, upgrade));
    }

    barrack_u[2]->total_levels = 5;
    barrack_u[2]->rate_of_change = -10;

    for (auto &upgrade : archer_upgrades) {
        archer_u.emplace_back(std::make_shared<Upgrade>(10, 10, upgrade));
    }

    archer_u[2]->total_levels = 5;
    archer_u[2]->rate_of_change = -10;

    for (auto &upgrade : wizardry_upgrades) {
        wizardry_u.emplace_back(std::make_shared<Upgrade>(10, 10, upgrade));
    }

    wizardry_u[2]->total_levels = 5;
    wizardry_u[2]->rate_of_change = -10;

    for (auto &upgrade : castle_upgrades) {
        castle_u.emplace_back(std::make_shared<Upgrade>(10, 10, upgrade));
    }

    for (auto &upgrade : other_building_upgrades) {
        other_building_u.emplace_back(
            std::make_shared<Upgrade>(10, 10, upgrade));
    }
    other_building_u[1]->total_levels = 5;
    other_building_u[1]->rate_of_change = -10;

    for (auto &upgrade : chance_upgrades) {
        chance_u.emplace_back(std::make_shared<Upgrade>(20, 5, upgrade, true));
    }

    activated_upgrades.push_back(hero_u);
    activated_upgrades.push_back(castle_u);
    activated_upgrades.push_back(other_building_u);
    activated_upgrades.push_back(barrack_u);
    activated_upgrades.push_back(archer_u);
    activated_upgrades.push_back(chance_u);
    activated_upgrades.push_back(wizardry_u);
}

std::vector<std::shared_ptr<UpgradeAction>> UpgradeManager::getUpgrades() {
    std::vector<std::shared_ptr<UpgradeAction>> upgrades;
    update();
    if (activated_upgrades.size() == 0) {
        return upgrades;
    }
    if (activated_upgrades.size() >= 3) {
        upgrades.push_back(getUpgrade(activated_upgrades[0]));
        upgrades.push_back(getUpgrade(activated_upgrades[1]));
        float cc = randomFloatInRange(0, 1);
        if (cc < .2) {
            upgrades.push_back(getRandomSummonCardUpgrade(1));
        } else {

            int final_ind =
                getRandomIntInRange(2, activated_upgrades.size() - 1);
            upgrades.push_back(getUpgrade(activated_upgrades[final_ind]));
        }
    }
    if (activated_upgrades.size() == 2) {
        upgrades.push_back(getUpgrade(activated_upgrades[0]));
        upgrades.push_back(getUpgrade(activated_upgrades[1]));
        update();
        upgrades.push_back(getRandomSummonCardUpgrade(1));
    }
    if (activated_upgrades.size() == 1) {
        upgrades.push_back(getUpgrade(activated_upgrades[0]));
        update();
        if (activated_upgrades.size() == 0) {
            upgrades.push_back(getRandomSummonCardUpgrade(1));
            upgrades.push_back(getRandomSummonCardUpgrade(2));
            return upgrades;
        }
        upgrades.push_back(getUpgrade(activated_upgrades[0]));
        update();
        if (activated_upgrades.size() == 0) {
            upgrades.push_back(getRandomSummonCardUpgrade(2));
            return upgrades;
        }
        upgrades.push_back(getUpgrade(activated_upgrades[0]));
    }

    return upgrades;
}

std::shared_ptr<UpgradeAction>
UpgradeManager::getRandomSummonCardUpgrade(int cards) {

    float cc1 = randomFloatInRange(0, 1);
    std::string card_type = INFANTRY_SUMMON_CARD_U;
    if (cc1 < .33) {
        card_type = INFANTRY_SUMMON_CARD_U;
    } else if (cc1 < .66) {
        card_type = ARCHERY_SUMMON_CARD_U;
    } else {
        card_type = MAGIC_SUMMON_CARD_U;
    }
    return getSummonCardUpgrade(cards, card_type);
}

std::shared_ptr<UpgradeAction>
UpgradeManager::getSummonCardUpgrade(int cards, std::string card_type) {
    std::shared_ptr<Upgrade> upg =
        std::make_shared<Upgrade>(true, cards, card_type);
    std::shared_ptr<UpgradeAction> uact = std::make_shared<UpgradeAction>();
    uact->upgrade = upg;
    uact->action = [this, upg]() {
        int cards_to_add = upg->rate_of_change;
        for (int i = 0; i < cards_to_add; i++) {

            if (upg->multiplier_id == INFANTRY_SUMMON_CARD_U) {
                getWorldState()->summon_manager->addSummonCard(
                    std::make_shared<SummonCard>(1, get_random_infantry_type(),
                                                 SummonCardType::INFANTRY));
            }
            if (upg->multiplier_id == ARCHERY_SUMMON_CARD_U) {
                getWorldState()->summon_manager->addSummonCard(
                    std::make_shared<SummonCard>(1, get_random_ranged_type(),
                                                 SummonCardType::ARCHERY));
            }
            if (upg->multiplier_id == MAGIC_SUMMON_CARD_U) {
                getWorldState()->summon_manager->addSummonCard(
                    std::make_shared<SummonCard>(1,
                                                 MagicType::LIGHTNING_SPELL));
            }
        }
    };
    return uact;
}

std::shared_ptr<UpgradeAction>
UpgradeManager::getUpgrade(std::vector<std::shared_ptr<Upgrade>> upgrades) {
    int total = upgrades.size();
    int rand_upgrade = rand() % total;
    std::shared_ptr<UpgradeAction> uact = std::make_shared<UpgradeAction>();
    std::shared_ptr<Upgrade> upg = upgrades[rand_upgrade];
    uact->upgrade = upg;
    uact->action = [this, upg]() {
        if (upg->isPercent) {
            getContainer()->getUpgradeContent()->update_stat(
                upg->multiplier_id, upg->rate_of_change * 1.0f);
        } else if (upg->isChance) {
            getContainer()->getUpgradeContent()->update_chance(
                upg->multiplier_id, upg->rate_of_change);
        } else {
            getContainer()->getUpgradeContent()->update_stat(
                upg->multiplier_id, upg->rate_of_change);
        }
        upg->current_level++;
        if (upg->current_level == upg->total_levels) {
            upg->is_exhausted = true;
        }
    };

    return uact;
}

void UpgradeManager::update() {
    std::vector<size_t> list_to_remove;
    int ind_u = 0;
    for (auto &upgrade : activated_upgrades) {
        std::vector<size_t> to_remove;
        for (size_t i = 0; i < upgrade.size(); i++) {
            if (upgrade[i]->is_exhausted) {
                to_remove.push_back(i);
            }
        }
        for (size_t i = to_remove.size(); i > 0; --i) {
            size_t index = to_remove[i - 1];
            upgrade.erase(upgrade.begin() + index);
        }
        if (upgrade.size() == 0) {
            list_to_remove.push_back(ind_u);
        }
        ind_u++;
    }
    for (size_t i = list_to_remove.size(); i > 0; --i) {
        size_t index = list_to_remove[i - 1];

        activated_upgrades.erase(activated_upgrades.begin() + index);
    }
}
