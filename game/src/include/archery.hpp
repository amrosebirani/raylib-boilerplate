#pragma once

#include "building.hpp"
#include "summon_card.hpp"
#include "summon_slot.hpp"

class Archery : public Building {
    public:
        Archery(float x, float y, int level);
        ~Archery();
        void repair(float repairAmount) override;
        void onUpgrade(int level) override;
        void pushSummonDialog() override;
        void draw() override;
        void update(float dt) override;
        void init() override;
        void cleanupData() override;
        bool isAlive() override;
        void die() override;
        bool hasSummonCards();
        std::shared_ptr<SummonCard> getCard();

    private:
        // float summonTime;
        // float summonTimeCounter;
        // Vector2 summonDim;
        std::vector<WarriorType> summonTypes;
        // std::queue<std::shared_ptr<SummonCard>> producedSummonCards;
        // float summonCardY = 0;
        // float summonExchT = 1.1f;
        // float summonExchTimer = 0;
        std::shared_ptr<std::vector<std::shared_ptr<SummonSlot>>> summonSlots;
};
