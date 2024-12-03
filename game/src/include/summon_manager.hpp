#pragma once

#include <queue>
#include <memory>
#include "summon_card.hpp"
#include "raylib.h"

struct ClickSlot {
        Rectangle rec;
        bool isActive;
        SummonCardType type;
        float start_x;

        ClickSlot(Rectangle rec, bool isActive, SummonCardType type,
                  float start_x)
            : rec(rec), isActive(isActive), type(type), start_x(start_x) {};
};

class SummonManager {
    public:
        SummonManager();
        ~SummonManager();
        void update(float dt);
        void draw();
        void addSummonCard(std::shared_ptr<SummonCard> card);
        bool enabled = true;

    private:
        // bool summonActivated = true;
        // float summonRT = 0.2f;
        // float summonTimer = 0;
        std::queue<std::shared_ptr<SummonCard>> summonCardsInfantry;
        std::queue<std::shared_ptr<SummonCard>> summonCardsArchery;
        std::queue<std::shared_ptr<SummonCard>> summonCardsWizardry;
        float summon_box_h = 0;
        int summon_font = 30;
        float summon_box_y = 0;
        RenderTexture2D summon_box;
        RenderTexture2D summon_chooser_box;
        float summon_width = 80;
        int totalCards = 0;
        bool summonChooserOpen = false;
        bool troopTypeChooserOpen = false;
        std::vector<ClickSlot *> clickSlots;
        void resetClickSlots();
        void initiliazeClickSlots();
        int last_ind = 0;
};
