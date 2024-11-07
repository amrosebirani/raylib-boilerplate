#pragma once

#include "building.hpp"
#include "defense_tower.hpp"
#include "game_object.h"
#include <memory>
#include "box2d/b2_body.h"
#include "collider_user_data.h"
#include "direction.hpp"
#include "enemy_types.h"
#include "state_machine.hpp"

class Enemy : public GameObject {
    public:
        Enemy(float radius, float x, float y)
            : radius(radius), GameObject(x, y) {};
        virtual void attack() = 0;
        virtual void tryAttack(std::shared_ptr<GameObject> warrior) = 0;
        std::shared_ptr<b2Body> collider;
        ColliderUserData *collider_data;
        virtual void takeAttack(float damage) = 0;
        virtual void buildingAttack(std::shared_ptr<Building> building) = 0;
        virtual void towerAttack(std::shared_ptr<DefenseTower> tower) = 0;
        void initStates(EnemyType type);
        void stateUpdate(EnemyType type);
        Direction directionFacing = Direction::SOUTH;
        Direction previousDirectionFacing = Direction::SOUTH;
        bool isAttacking = false;
        float attackCooldown = 10.0f;
        float attackCooldownTracker = 0.0f;
        int canAttack = 0;
        float damage;
        StateMachine *stateMachine;
        void afterDie(EnemyType type);
        void throwBlood();
        std::shared_ptr<GameObject> targetObj = nullptr;
        bool isMarching = false;
        Vector2 breakoffPoint;
        Vector2 dirToMarch;
        void addContactAttack(std::shared_ptr<GameObject> cont);
        void removeContactAttack(std::shared_ptr<GameObject> cont);
        std::vector<std::shared_ptr<GameObject>> contactAttackUnits;
        void manageContactAttackUnits();
        float mvspd = 25.0f;

    private:
        float radius;
};
