#pragma once
#include <string>
#include "box2d/b2_world_callbacks.h"
#include "game_object.h"

struct ColliderUserData {
        enum Type {
            Warrior,
            Archer,
            WarriorSensor,
            Enemy,
            Collectable,
            Projectile,
            Castle,
            Formation,
            DefenseTower,
            TreePatch,
            Building,
            BuildingSensor,
            SummonSensor,
            AttackTowerSensor,
            LightningTowerSensor,
            ArcherSensor,
            CastleSensor,
            FireBall,
        };
        Type type;
        std::string sub_type;
        std::shared_ptr<GameObject> obj;
};

class MyContactListener : public b2ContactListener {
        void extracted();
        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;
};
