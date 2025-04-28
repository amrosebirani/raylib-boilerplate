#pragma once

#include "building.hpp"
#include "game_object.h"
#include <vector>
#include <memory>
#include <fstream>

class AttackTower : public Building {
    public:
        AttackTower(float x, float y, int level);
        AttackTower(std::ifstream &in);
        ~AttackTower();
        void repair(float repairAmount) override;
        void onUpgrade(int level) override;
        void draw() override;
        void update(float dt) override;
        void init() override;
        void cleanupData() override;
        bool isAlive() override;
        void die() override;
        void buildingObjectSave(std::ofstream &out) const override;
        void addEnemy(std::shared_ptr<GameObject> enemy);
        std::vector<std::shared_ptr<GameObject>> enemies;
        void setEnemySensor();
        GameObjectType getObjectType() override {
            return GameObjectType::ATTACK_TOWER;
        }
        void Save(std::ofstream &out) const override {
            buildingObjectSave(out);
        };

    private:
        std::shared_ptr<b2Body> enemySensor;
        ColliderUserData *sdata;
        std::vector<std::shared_ptr<GameObject>> archers;
        int previous_level;
        unsigned char previousAlpha;
        void setArchers(int level, bool operational = false);
        float previousRangeM = 1;
};
