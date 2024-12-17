
#pragma once
#include "game_object.h"
#include <memory>
#include "box2d/b2_body.h"
#include "collider_user_data.h"
#include "raylib.h"
#include "state_machine.hpp"
#include "warrior_types.h"
#include "direction.hpp"

class Warrior : public GameObject {
    public:
        Warrior(float radius, float rel_x, float rel_y, bool inFormation = true)
            : radius(radius), GameObject(0, 0), rel_x(rel_x), rel_y(rel_y),
              inFormation(inFormation) {};
        Warrior(float radius, float x, float y, int i = 0);
        virtual void attack() = 0;
        void tryAttack(std::shared_ptr<GameObject> enemy);
        virtual void takeAttack(float damage) = 0;
        virtual ~Warrior();
        void initStates(WarriorType type);
        void stateUpdate(WarriorType type, float dt);
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
        int canAttack = 0;
        float damage;
        float in_damage_mult = 1.0f;
        void throwBlood();
        Color getColor() override {
            if (inFormation) {
                return {ORANGE.r, ORANGE.g, ORANGE.b, 159};
            }
            return {DARKBLUE.r, DARKBLUE.g, DARKBLUE.b, 150};
        };
        void addContactAttack(std::shared_ptr<GameObject> cont);
        void removeContactAttack(std::shared_ptr<GameObject> cont);
        std::vector<std::shared_ptr<GameObject>> contactAttackUnits;
        void manageContactAttackUnits();
        void doAttacking(float dt);
        bool inFormation = false;
        void addInRangeEnemyUnit(std::shared_ptr<GameObject> enemyUnit);
        void removeInRangeEnemyUnit(std::shared_ptr<GameObject> enemyUnit);
        ColliderUserData *sensorData;
        std::shared_ptr<b2Body> sensor;
        std::vector<std::shared_ptr<GameObject>> inRangeEnemyUnits;
        void setMovement(Vector2 dir);
        Vector2 dirToMove;
        float mvspd = 0;
        virtual WarriorType getType() {
            return WarriorType::WARRIOR_TYPE_KNIGHT;
        }

    private:
        float radius;
        void manageInRangeEnemyUnits();
};
