#pragma once
#include "collider_user_data.h"
#include "state_machine.hpp"
#include "game_object.h"
#include "state_params.hpp"
#include <memory>
#include "direction.hpp"
#include "box2d/b2_body.h"

class Archer : public GameObject {
    public:
        Archer(float rx, float ry, std::shared_ptr<GameObject> tower,
               bool operational = false);
        ~Archer();

        void draw() override;

        void update(float dt) override;

        bool isAlive() override;

        void die() override;
        void init() override;
        void cleanupData() override;
        void attack();
        void tryAttack(std::shared_ptr<GameObject> enemy);
        void takeAttack(float damage);
        Direction directionFacing = Direction::SOUTH;
        Direction previousDirectionFacing = Direction::SOUTH;
        Direction directionAttacking = Direction::SOUTH;
        void changeState(std::string stateName, StateParams *params);
        float damage = 75.0f;
        std::shared_ptr<GameObject> tower;
        float getAttackRange();
        unsigned char alpha;
        bool isOperational = false;
        void addEnemy(std::shared_ptr<GameObject> enemy);
        std::vector<std::shared_ptr<GameObject>> enemies;
        bool hasTower = false;
        std::shared_ptr<b2Body> collider;
        ColliderUserData *collider_data;

    private:
        StateMachine *stateMachine;
        bool alive = true;
        float attackRange = 80.0f;
        float hp = 0;
        std::shared_ptr<b2Body> enemySensor;
        ColliderUserData *sdata;
};
