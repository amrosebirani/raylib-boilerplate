#pragma once

#include "archer.hpp"
#include "defense_tower.hpp"
#include "enemy.hpp"
#include "raylib.h"

class ParameterizedEnemy : public Enemy {
    public:
        ParameterizedEnemy(float x, float y, EnemyType type);
        ParameterizedEnemy(std::ifstream &in);
        ~ParameterizedEnemy();

        void draw() override;
        void Save(std::ofstream &out) const override;

        void update(float dt) override;

        bool isAlive() override;

        void die() override;
        void init() override;
        void baseInit();
        void cleanupData() override;

        void attack() override;

        void tryAttack(std::shared_ptr<GameObject> target) override;
        void buildingAttack(std::shared_ptr<Building> building) override;
        void towerAttack(std::shared_ptr<DefenseTower> tower) override;
        void archerAttack(std::shared_ptr<Archer> archer) override;
        void setDirectionOfMovement();
        void setMarching(Vector2 breakOffPoint);
        void setTargetObj(std::shared_ptr<GameObject> target);
        void setTargetForAttack();
        void setCameraTarget(Vector2 tp);

        void hasCrossedMarchingPoint();
        void hasCrossedCameraTriggerPoint();

        void takeAttack(float damage) override;
        Color getColor() override {
            if (showWave) {
                return {RED.r, RED.g, RED.b, 160};
            } else {

                if (!isMarching) return {RED.r, RED.g, RED.b, 150};
            }
            return {0, 0, 0, 0};
        };

    private:
        bool alive = true;
        float hp = 60.0f;
        EnemyType type;
        Vector2 dirToMove;
        int updateCounter = 0;
        Vector2 cameraTriggerPoint;
        bool isLead = false;
        double breakOffDist = 0;
        double cameraTriggerDist = 0;
        Vector2 center;
        bool showWave = false;
};
