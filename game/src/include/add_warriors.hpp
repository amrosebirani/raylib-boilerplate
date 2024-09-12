#pragma once
#include "collectible.hpp"
#include "collider_user_data.h"
#include "box2d/b2_body.h"

class AddWarriors : public Collectible {
    public:
        AddWarriors(float x, float y);
        ~AddWarriors();
        void draw() override;
        void update(float dt) override;
        void collect() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;

    private:
        int warriorCount;
        bool alive = true;
        float circle_radius;
        ColliderUserData *data;
        std::shared_ptr<b2Body> collider;
};
