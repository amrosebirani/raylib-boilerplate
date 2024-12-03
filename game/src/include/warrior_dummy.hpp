#pragma once

#include "game_object.h"
#include "state_machine.hpp"
#include "warrior_types.h"

class WarriorDummy : public GameObject {
    public:
        WarriorDummy(float radius, float rel_x, float rel_y, WarriorType type)
            : radius(radius), GameObject(0, 0), rel_x(rel_x), rel_y(rel_y),
              type(type) {};
        ~WarriorDummy();
        void stateUpdate(float dt);
        Direction directionFacing = Direction::SOUTH;
        Direction previousDirectionFacing = Direction::SOUTH;
        StateMachine *stateMachine;
        bool isIdle;

        Color getColor() override {
            return {ORANGE.r, ORANGE.g, ORANGE.b, 159};
        };
        float rel_x;
        float rel_y;
        Vector2 dirToMove;
        float mvspd = 0;

        void draw() override;
        void update(float dt) override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;

    private:
        float radius;
        bool alive = true;
        WarriorType type;
};
