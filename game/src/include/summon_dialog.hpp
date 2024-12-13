#pragma once
#include "building_data.hpp"
#include "panel.hpp"
#include "property_type.hpp"
#include "room.hpp"
#include "summon_slot.hpp"
#include <memory>
#include <vector>

class SummonDialog : public Room {
    public:
        SummonDialog(
            PropertyType type, int level,
            std::shared_ptr<std::vector<std::shared_ptr<SummonSlot>>> slots,
            BuildingData *buildingData);
        ~SummonDialog();
        void draw() override;
        void drawPortrait();
        void drawLandscape();
        bool update(float dt) override;
        bool isFinished() override;

    private:
        bool finished;
        std::shared_ptr<std::vector<std::shared_ptr<SummonSlot>>> slots;
        std::vector<std::shared_ptr<SummonSlot>> availableSlots;
        std::vector<std::shared_ptr<SummonSlot>> producingSlots;
        PropertyType type;
        int level;
        std::shared_ptr<Panel> panel;
        bool buttonPressed = false;
        BuildingData *buildingData;
        bool isPortrait = false;
        std::function<void()> unSelectAll;
};
