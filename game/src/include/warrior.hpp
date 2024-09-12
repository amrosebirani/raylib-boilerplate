
#pragma once
#include "game_object.h"
#include <memory>
#include "box2d/b2_body.h"
#include "collider_user_data.h"
#include "state_machine.hpp"
#include "warrior_types.h"
#include "direction.hpp"

class Warrior : public GameObject {
    public:
        Warrior(float radius, float rel_x, float rel_y)
            : radius(radius), GameObject(0, 0), rel_x(rel_x), rel_y(rel_y) {};
        virtual void attack() = 0;
        virtual void tryAttack(std::shared_ptr<GameObject> enemy) = 0;
        virtual void takeAttack(float damage) = 0;
        void initStates(WarriorType type);
        void stateUpdate(WarriorType type);
        void afterDie(WarriorType type);
        float rel_x;
        float rel_y;
        std::shared_ptr<b2Body> collider;
        ColliderUserData *collider_data;
        b2Fixture *fixture;
        Direction directionFacing = Direction::SOUTH;
        Direction directionAttacking = Direction::SOUTH;
        Direction previousDirectionFacing = Direction::SOUTH;
        StateMachine *stateMachine;
        bool isIdle;
        bool isAttacking = false;
        float attackCooldown = 1.0f;
        float attackCooldownTracker = 0.0f;
        bool canAttack = true;
        float damage;
        void throwBlood();

    private:
        float radius;
};
