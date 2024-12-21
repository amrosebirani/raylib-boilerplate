#include "upgrade_info.hpp"
#include <unordered_map>

std::unordered_map<PropertyType, std::vector<std::shared_ptr<UpgradeInfo>>>
    upgrade_info;

void setUpgradeInfo() {
    std::vector<std::shared_ptr<UpgradeInfo>> archer_info;
    std::shared_ptr<UpgradeInfo> archer_level_0 = std::make_shared<UpgradeInfo>(
        0, 1,
        std::vector<std::string>{"Enable Archers", "5 x 2 units/summon card",
                                 "1 Training Arena"});
    std::shared_ptr<UpgradeInfo> archer_level_1 = std::make_shared<UpgradeInfo>(
        1, 2,
        std::vector<std::string>{"Enable Crossbowmen", "+50% HP", "+20% damage",
                                 "+20% range", "6 x 2 units/summon card",
                                 "+1 Training Arena"});
    std::shared_ptr<UpgradeInfo> archer_level_2 = std::make_shared<UpgradeInfo>(
        2, 3,
        std::vector<std::string>{"Enable Javeliner", "+50% HP", "+30% damage",
                                 "+20% range", "7 x 3 units/summon card",
                                 "+30% fire rate", "+1 Training Arena"});
    archer_info.push_back(archer_level_0);
    archer_info.push_back(archer_level_1);
    archer_info.push_back(archer_level_2);
    upgrade_info.insert({PropertyType::ARCHERY, archer_info});

    std::vector<std::shared_ptr<UpgradeInfo>> barracks_info;

    std::shared_ptr<UpgradeInfo> barracks_level_0 =
        std::make_shared<UpgradeInfo>(
            0, 1,
            std::vector<std::string>{"Enable Spearmen", "Enable Shield Bearer",
                                     "4 x 2 units/summon card",
                                     "1 Training Arena"});
    std::shared_ptr<UpgradeInfo> barracks_level_1 =
        std::make_shared<UpgradeInfo>(
            1, 2,
            std::vector<std::string>{
                "Enable Shield Bearer", "+20% HP", "+20% damage",
                "+20% attack rate", "+10% dodge rate",
                "4 x 3 units/summon card", "+1 Training Arena"});

    std::shared_ptr<UpgradeInfo> barracks_level_2 =
        std::make_shared<UpgradeInfo>(
            2, 3,
            std::vector<std::string>{"Enable Axeman", "+25% HP", "+30% damage",
                                     "+20% attack rate", "+10% dodge rate",
                                     "5 x 3 units/summon card",
                                     "+1 Training Arena"});

    std::shared_ptr<UpgradeInfo> barracks_level_3 =
        std::make_shared<UpgradeInfo>(
            3, 4,
            std::vector<std::string>{
                "Enable Berserker", "+30% HP", "+40% damage",
                "+30% attack rate", "+10% dodge rate",
                "6 x 4 units/summon card", "+1 Training Arena"});
    barracks_info.push_back(barracks_level_0);
    barracks_info.push_back(barracks_level_1);
    barracks_info.push_back(barracks_level_2);
    barracks_info.push_back(barracks_level_3);

    upgrade_info.insert({PropertyType::BARRACKS, barracks_info});

    std::vector<std::shared_ptr<UpgradeInfo>> castle_info;

    std::shared_ptr<UpgradeInfo> castle_level_0 = std::make_shared<UpgradeInfo>(
        0, 1, std::vector<std::string>{"Enable Barracks", "Enable Archery"});
    std::shared_ptr<UpgradeInfo> castle_level_1 = std::make_shared<UpgradeInfo>(
        1, 2,
        std::vector<std::string>{"Enable Wizardry", "+100% Castle HP",
                                 "+20% attack range", "+50% damage",
                                 "+50% Hero HP", "+30% attack rate"});
    std::shared_ptr<UpgradeInfo> castle_level_2 = std::make_shared<UpgradeInfo>(
        2, 3,
        std::vector<std::string>{"Enable Lightning towers", "+100% Castle HP",
                                 "+20% attack range", "+50% damage",
                                 "+50% Hero HP", "+40% attack rate",
                                 "Enable Meteor attack"});

    castle_info.push_back(castle_level_0);
    castle_info.push_back(castle_level_1);
    castle_info.push_back(castle_level_2);

    upgrade_info.insert({PropertyType::CASTLE, castle_info});

    std::vector<std::shared_ptr<UpgradeInfo>> tower_info;

    std::shared_ptr<UpgradeInfo> tower_level_0 = std::make_shared<UpgradeInfo>(
        0, 1, std::vector<std::string>{"Raise Defense Tower", "+4 Archers"});
    std::shared_ptr<UpgradeInfo> tower_level_1 = std::make_shared<UpgradeInfo>(
        1, 2, std::vector<std::string>{"+100% HP", "+12 Crossbowmen"});

    tower_info.push_back(tower_level_0);
    tower_info.push_back(tower_level_1);

    upgrade_info.insert({PropertyType::DEFENSE_TOWER, tower_info});

    std::vector<std::shared_ptr<UpgradeInfo>> house_info;

    std::shared_ptr<UpgradeInfo> house_level_0 = std::make_shared<UpgradeInfo>(
        0, 1,
        std::vector<std::string>{"+1 worker", "50 tribute/min",
                                 "Workers repair buildings"});
    std::shared_ptr<UpgradeInfo> house_level_1 = std::make_shared<UpgradeInfo>(
        1, 2, std::vector<std::string>{"+1 worker", "100 tribute/min"});
    std::shared_ptr<UpgradeInfo> house_level_2 = std::make_shared<UpgradeInfo>(
        2, 3, std::vector<std::string>{"+1 worker", "150 tribute/min"});

    house_info.push_back(house_level_0);
    house_info.push_back(house_level_1);
    house_info.push_back(house_level_2);

    upgrade_info.insert({PropertyType::HOUSE, house_info});

    std::vector<std::shared_ptr<UpgradeInfo>> wizardry_info;

    std::shared_ptr<UpgradeInfo> wizardry_level_0 =
        std::make_shared<UpgradeInfo>(
            0, 1,
            std::vector<std::string>{"Fire spell", "Freeze spell", "+1 slot"});

    wizardry_info.push_back(wizardry_level_0);

    upgrade_info.insert({PropertyType::WIZARDRY, wizardry_info});

    std::vector<std::shared_ptr<UpgradeInfo>> lightning_tower_info;

    std::shared_ptr<UpgradeInfo> lightning_tower_level_0 =
        std::make_shared<UpgradeInfo>(
            0, 1, std::vector<std::string>{"Lightning strike attack"});

    lightning_tower_info.push_back(lightning_tower_level_0);

    upgrade_info.insert({PropertyType::LIGHTNING_TOWER, lightning_tower_info});
}

std::shared_ptr<UpgradeInfo> getUpgradeInfo(PropertyType type, int level) {
    return upgrade_info[type][level];
}
