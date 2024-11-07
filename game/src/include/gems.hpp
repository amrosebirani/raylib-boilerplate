#pragma once
#include "collectible.hpp"
#include "collider_user_data.h"
#include "box2d/b2_body.h"
#include "gem_type.hpp"

class Gem : public Collectible {
    public:
        Gem(float x, float y, GemType type);
        ~Gem();
        void draw() override;
        void update(float dt) override;
        void collect() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;

    private:
        bool alive = true;
        ColliderUserData *data;
        std::shared_ptr<b2Body> collider;
        GemType type;
};
