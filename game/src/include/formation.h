#pragma once
#include "constants.h"
#include "warrior.hpp"
#include "warrior_types.h"
#include <vector>
#include <memory>
#include "raylib.h"
#include "box2d/b2_body.h"
#include "direction.hpp"

class Formation {
    public:
        Formation(int level, float x, float y, float starter_radius,
                  float orbit_margin);
        ~Formation();
        class FormationOrbit {
            public:
                FormationOrbit(float starter_radius, WarriorType type,
                               int totalSlots = 5);
                FormationOrbit(float starter_radius, WarriorType type,
                               int totalSlots, int activatedSlots);
                void update(float dt, Vector2 origin, bool isIdle,
                            Direction directionFacing);
                void draw();

                class WarriorSlot {
                    public:
                        WarriorSlot(float x, float y,
                                    std::shared_ptr<Warrior> warrior)
                            : x(x), y(y), warrior(warrior) {};
                        void update(float dt, Vector2 origin, bool isIdle,
                                    Direction directionFacing);
                        void draw();
                        std::shared_ptr<Warrior> warrior;
                        float x;
                        float y;
                        void respawnWarrior(WarriorType type);

                    private:
                        bool respawn = false;
                        float respawnTime = 0;
                        float respawnTracker = 0;
                        WarriorType respawntype;
                };
                std::vector<WarriorSlot> slots;
                WarriorType type;
                float starter_radius;

            private:
                float starter_angle;
                int totalSlots = 0;
        };

        void update(float dt);
        void draw();
        void
        getGameObjects(std::vector<std::shared_ptr<GameObject>> &gameObjects);
        std::shared_ptr<Warrior> getKeyWarrior();
        float x;
        float y;
        void appendWarriors(int count);
        float getFormationRadii();
        bool isKeyWarriorAlive();
        float getMvSpd();
        b2Fixture *addFormationFixture(b2FixtureDef *fixtureDef);
        void removeFormationFixture(b2Fixture *fixture);
        void initOrbits();
        Direction directionFacing = Direction::SOUTH;
        bool isIdle = true;

    private:
        std::vector<FormationOrbit *> orbits;
        int level;
        float starter_radius = 0;
        float orbit_margin = 2;
        float mvspd = FORMATION_BASE_MVSPD;
        std::shared_ptr<Warrior> keyWarrior;
        std::shared_ptr<b2Body> collider;
        ColliderUserData *collider_data;
        void keyBoardMove();
        void joyStickMove();
        Vector2 dir_to_move;
};
