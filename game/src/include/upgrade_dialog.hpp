#pragma once
#include "building_data.hpp"
#include "panel.hpp"
#include "property_type.hpp"
#include "room.hpp"
#include "upgrade_info.hpp"
#include <functional>
#include <memory>

class UpgradeDialog : public Room {
    public:
        UpgradeDialog(PropertyType type, int level, int coingToUpgrade,
                      std::function<void()> dialogUpgradeCallback,
                      BuildingData *buildingData);
        ~UpgradeDialog();

        void draw() override;
        bool update(float dt) override;
        bool isFinished() override;

    private:
        bool finished = false;
        PropertyType type;
        int level;
        int coingToUpgrade;
        std::shared_ptr<Panel> panel;
        std::shared_ptr<UpgradeInfo> upgradeInfo;
        std::function<void()> dialogUpgradeCallback;
        BuildingData *buildingData;
        bool upgradePressed = false;
};
