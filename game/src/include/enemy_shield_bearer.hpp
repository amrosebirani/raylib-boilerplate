#pragma once

#include "defense_tower.hpp"
#include "enemy.hpp"

class EnemyShieldBearer : public Enemy {
    public:
        EnemyShieldBearer(float x, float y);
        ~EnemyShieldBearer();

        void draw() override;

        void update(float dt) override;

        bool isAlive() override;

        void die() override;
        void init() override;
        void cleanupData() override;

        void attack() override;

        void tryAttack(std::shared_ptr<GameObject> target) override;
        void buildingAttack(std::shared_ptr<Building> building) override;
        void towerAttack(std::shared_ptr<DefenseTower> tower) override;

        void takeAttack(float damage) override;

    private:
        bool alive = true;
        float mvspd = 45.0f;
        float hp = 60.0f;
};
