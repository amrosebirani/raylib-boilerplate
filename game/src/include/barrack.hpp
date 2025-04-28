#pragma once

#include "building.hpp"
#include "summon_card.hpp"
#include "summon_slot.hpp"
#include "warrior_types.h"
// #include <queue>

class Barrack : public Building {
    public:
        Barrack(float x, float y, int level);
        Barrack(std::ifstream &in);
        ~Barrack();
        void repair(float repairAmount) override;
        void draw() override;
        void update(float dt) override;
        void init() override;
        void cleanupData() override;
        bool isAlive() override;
        void die() override;
        void onUpgrade(int level) override;
        void pushSummonDialog() override;
        bool hasSummonCards();
        std::shared_ptr<SummonCard> getCard();
        void buildingObjectSave(std::ofstream &out) const override;
        GameObjectType getObjectType() override {
            return GameObjectType::BARRACK;
        }
        void Save(std::ofstream &out) const override {
            buildingObjectSave(out);
        };

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
