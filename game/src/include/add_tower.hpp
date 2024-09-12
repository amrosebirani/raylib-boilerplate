#pragma once
#include "collectible.hpp"
#include "collider_user_data.h"
#include "box2d/b2_body.h"
#include "tower_spawn.hpp"

class AddTower : public Collectible {
    public:
        AddTower(float x, float y);
        AddTower(float x, float y, TowerSpawnRing *ring);
        ~AddTower();
        void draw() override;
        void update(float dt) override;
        void collect() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;

    private:
        int archers;
        bool alive = true;
        float circle_radius;
        ColliderUserData *data;
        std::shared_ptr<b2Body> collider;
        TowerSpawnRing *ring;
};
