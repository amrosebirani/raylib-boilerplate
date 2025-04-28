#pragma once

#include "building.hpp"
#include "constants.h"
#include "enemy.hpp"

class LightningTower : public Building {
    public:
        LightningTower(float x, float y, int level);
        LightningTower(std::ifstream &in);
        ~LightningTower();
        void repair(float repairAmount) override;
        void onUpgrade(int level) override;
        void draw() override;
        void update(float dt) override;
        void init() override;
        void cleanupData() override;
        bool isAlive() override;
        void die() override;
        void addEnemy(std::shared_ptr<GameObject> enemy);
        void removeEnemy(std::shared_ptr<GameObject> enemy);
        std::vector<std::shared_ptr<GameObject>> enemies;
        void hitLightningBolt(std::shared_ptr<Enemy> enemy);
        void buildingObjectSave(std::ofstream &out) const override;
        GameObjectType getObjectType() override {
            return GameObjectType::LIGHTNING_TOWER;
        }
        void Save(std::ofstream &out) const override {
            buildingObjectSave(out);
        };

    private:
        std::shared_ptr<b2Body> enemySensor;
        ColliderUserData *sdata;
        float damage = LIGHTNING_TOWER_DAMAGE;
        float cooldown_time = LIGHTNING_TOWER_ATIME;
        float cooldown_time_counter = 0;
        Vector2 startPoint = {138, 30};
        float xs, ys;
};
