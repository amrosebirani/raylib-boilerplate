#pragma once
#include <memory>
#include <functional>
#include <string>

struct Upgrade {
        // std::vector<std::shared_ptr<UpgradeItem>> upgrades;
        int current_level = 0;
        bool is_exhausted = false;
        bool isPercent = true;
        bool isCount = false;
        bool isChance = false;
        int total_levels = 10;
        int rate_of_change = 10;
        std::string multiplier_id;

        Upgrade(int total_levels, int rate_of_change, std::string multiplier_id)
            : total_levels(total_levels), rate_of_change(rate_of_change),
              multiplier_id(multiplier_id) {
        }
        Upgrade(int total_levels, int rate_of_change, std::string multiplier_id,
                bool isChance)
            : total_levels(total_levels), rate_of_change(rate_of_change),
              multiplier_id(multiplier_id), isChance(isChance) {
            isPercent = false;
        }
        Upgrade(bool isCount, int rate_of_change, std::string multiplier_id)
            : isCount(isCount), rate_of_change(rate_of_change),
              multiplier_id(multiplier_id) {
            isPercent = false;
        }
};

struct UpgradeAction {
        std::shared_ptr<Upgrade> upgrade;
        std::function<void()> action;
};

class UpgradeManager {
    public:
        UpgradeManager();

        std::vector<std::shared_ptr<UpgradeAction>> getUpgrades();

        void update();
        std::shared_ptr<UpgradeAction>
        getUpgrade(std::vector<std::shared_ptr<Upgrade>> upgrades);
        std::shared_ptr<UpgradeAction>
        getSummonCardUpgrade(int cards, std::string card_type);
        std::shared_ptr<UpgradeAction> getRandomSummonCardUpgrade(int cards);

    private:
        std::vector<std::string> hero_upgrades;
        std::vector<std::string> barrack_upgrades;
        std::vector<std::string> archer_upgrades;
        std::vector<std::string> castle_upgrades;
        std::vector<std::string> other_building_upgrades;
        std::vector<std::string> chance_upgrades;
        std::vector<std::string> wizardry_upgrades;

        std::vector<std::shared_ptr<Upgrade>> hero_u;
        std::vector<std::shared_ptr<Upgrade>> barrack_u;
        std::vector<std::shared_ptr<Upgrade>> archer_u;
        std::vector<std::shared_ptr<Upgrade>> castle_u;
        std::vector<std::shared_ptr<Upgrade>> other_building_u;
        std::vector<std::shared_ptr<Upgrade>> chance_u;
        std::vector<std::shared_ptr<Upgrade>> wizardry_u;

        std::vector<std::vector<std::shared_ptr<Upgrade>>> activated_upgrades;
};
