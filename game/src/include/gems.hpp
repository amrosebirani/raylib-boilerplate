#pragma once
#include "collectible.hpp"
#include "collider_user_data.h"
#include "box2d/b2_body.h"
#include "game_object_types.h"
#include "gem_type.hpp"

class Gem : public Collectible {
    public:
        Gem(float x, float y, GemType type);
        Gem(std::ifstream &in);
        ~Gem();
        void draw() override;
        void update(float dt) override;
        void collect() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
        GameObjectType getObjectType() override {
            return GameObjectType::GEM;
        }
        void Save(std::ofstream &os) const override;

    private:
        bool alive = true;
        ColliderUserData *data;
        std::shared_ptr<b2Body> collider;
        GemType type;
};
