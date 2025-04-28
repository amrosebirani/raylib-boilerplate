#pragma once

#include <memory>
#include <unordered_map>
#include "summon_card.hpp"
#include "raylib.h"
#include "timer.h"

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
        SummonManager(std::ifstream &in);
        void save(std::ofstream &out) const;
        ~SummonManager();
        void setBasics();
        void update(float dt);
        void draw();
        void addSummonCard(std::shared_ptr<SummonCard> card);
        bool enabled = true;
        void transitToOpen();
        void transitToClose();

    private:
        // bool summonActivated = true;
        // float summonRT = 0.2f;
        // float summonTimer = 0;
        // std::queue<std::shared_ptr<SummonCard>> summonCardsInfantry;
        // std::queue<std::shared_ptr<SummonCard>> summonCardsArchery;
        // std::queue<std::shared_ptr<SummonCard>> summonCardsWizardry;
        std::vector<std::shared_ptr<SummonCard>> summonCards;
        bool isOpen = false;
        Timer timer;
        float summonCircleY;
        float summonBoxY;
        void drawSummonCircle();
        void drawSummonBox();
        void drawSummonCards();
        float trans_x = 0;
        float start_x = 0;
        bool isDragging = false;
        bool mousePressed = false;
        float dragThreshold = 5.0f;
        Vector2 startPos = {0, 0};
        Vector2 currentPos = {0, 0};
        float end_x = 0;
        float shift_x = 0;
        float leftHidden = 0;
        float rightHIdden = 0;
        void setEndX();
        void scrollIfNeeded(float swiped);
        void handleDesktopInput();
        void handleMobileInput();
        void handleClick(Vector2 pos);
        void setCircleRects();
        void setBoxRects();
        bool openPressed = false;
        float closeTimer = 5;
        float closeTimerCounter = 0;
        std::shared_ptr<std::unordered_map<std::string, float>> tweenVals;
        // bool summonChooserOpen = false;
        // bool troopTypeChooserOpen = false;
        // std::vector<ClickSlot *> clickSlots;
        // void resetClickSlots();
        // void initiliazeClickSlots();
        // int last_ind = 0;
};
