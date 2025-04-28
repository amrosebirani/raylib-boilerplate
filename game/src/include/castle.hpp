#pragma once

#include "building.hpp"
#include "raylib.h"
#include "constants.h"

class Castle : public Building {
    public:
        Castle(float x, float y, float health, int level);
        Castle(std::ifstream &in);
        ~Castle();

        void repair(float repairAmount) override;
        void onUpgrade(int level) override;
        void update(float dt) override;
        void draw() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
        Vector2 getDimensions();

        enum CastleState {
            OPERATIONAL,
            OPERATIONAL_UNDER_ATTACK,
            UNOPERATIONAL,
            UNOPERATIONAL_UNDER_ATTACK,
            CAPTURED
        };

        CastleState getState();
        void hit(float damage);
        void addEnemy(std::shared_ptr<GameObject> enemy);
        std::vector<std::shared_ptr<GameObject>> enemies;
        void bowAttack();
        void fireBallAttack();
        void setEnemySensor();
        void buildingObjectSave(std::ofstream &out) const override;
        GameObjectType getObjectType() override {
            return GameObjectType::CASTLE;
        }
        void Save(std::ofstream &out) const override {
            buildingObjectSave(out);
        };

    private:
        float height = CASTLE_HEIGHT;
        float width = 2176.0f * CASTLE_HEIGHT / 1516.0f;
        bool alive = true;
        CastleState state;
        bool isAttacked = false;
        float changeAttackTimeout = 0;
        float changeAttackCounter = 0;
        std::shared_ptr<b2Body> enemySensor;
        ColliderUserData *esdata;
        float bowAttackTime = CASTLE_BOW_ATTACK_TIME;
        float bowAttackCounter = 0;
        float fireBallAttackTime = CASTLE_FIREBALL_ATTACK_TIME;
        float fireBallAttackCounter = 0;
        float previousRangeM = 1;
};
