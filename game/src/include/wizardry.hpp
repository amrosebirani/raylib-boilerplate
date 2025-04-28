#pragma once

#include "building.hpp"
#include "magic_types.hpp"
#include "summon_slot.hpp"

class Wizardry : public Building {
    public:
        Wizardry(float x, float y, int level);
        Wizardry(std::ifstream &in);
        ~Wizardry();
        void repair(float repairAmount) override;
        void onUpgrade(int level) override;
        void pushSummonDialog() override;
        void update(float dt) override;
        void draw() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
        bool hasSummonCards();
        std::shared_ptr<SummonCard> getCard();
        void buildingObjectSave(std::ofstream &out) const override;
        GameObjectType getObjectType() override {
            return GameObjectType::WIZARDRY;
        }
        void Save(std::ofstream &out) const override {
            buildingObjectSave(out);
        };

    private:
        std::vector<MagicType> summonTypes;
        std::shared_ptr<std::vector<std::shared_ptr<SummonSlot>>> summonSlots;
};
