#pragma once

#include "panel.hpp"
#include "progress_bar.hpp"
#include "property_type.hpp"
#include "raylib.h"
#include "summon_card.hpp"
#include "warrior_types.h"
#include <functional>
#include <memory>
class SummonSlot {
    public:
        SummonSlot(int index, int level, PropertyType type,
                   SummonCardType stype = SummonCardType::INFANTRY);
        ~SummonSlot();

        void update(float dt);
        void startProducing(WarriorType wtype);
        float getProgress();
        void setLevel(int level);
        bool getIsProducing() {
            return isProducing;
        }
        WarriorType getWarriorType() {
            return warriorType;
        }
        std::vector<Rectangle> clickRects;
        Rectangle startProduceRect;
        std::vector<WarriorType> warriorTypes;
        void drawSlotAvailable(std::shared_ptr<Panel> panel, float y);
        void drawSlotProducing(std::shared_ptr<Panel> panel, float y);
        void handleClick();
        void drawSlotAvailableMaster(std::shared_ptr<Panel> panel, float y);
        void drawSlotProducingMaster(std::shared_ptr<Panel> panel, float y);
        void drawSlotAvailableDetail(std::shared_ptr<Panel> panel, float y);
        void drawSlotProducingDetail(std::shared_ptr<Panel> panel, float y);
        void setSelected(bool selected);
        bool isSelected();
        void handleLandscapeClick(std::function<void()> unSelectAll);

    private:
        int level;
        PropertyType type;
        SummonCardType summon_type;
        bool isProducing = false;
        float produceTime;
        float produceTimeCounter;
        WarriorType warriorType;
        int selectedIndex = 0;
        int produceCost;
        std::shared_ptr<ProgressBar> progressBar;
        bool selected = false;
        int index;
        int warriorIndex = 0;
        Rectangle masterRect;
        Rectangle leftCaretRect;
        Rectangle rightCaretRect;
        Rectangle startPRect;
        bool mpressed = false;
        bool lpressed = false;
        bool rpressed = false;
        bool spressed = false;
};
