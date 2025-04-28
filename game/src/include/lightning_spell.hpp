#pragma once

#include "box2d/b2_body.h"
#include "collider_user_data.h"
#include "spell.hpp"
#include <memory>

class LightningSpell : public Spell {

    public:
        LightningSpell(int level, float x, float y);
        ~LightningSpell();
        void execute() override;
        void addEnemy(std::shared_ptr<GameObject> enemy) override;
        std::vector<std::shared_ptr<GameObject>> enemies;

        void draw() override;
        void update(float dt) override;
        bool isAlive() override;
        void die() override;
        void cleanupData() override;
        void init() override;
        void Save(std::ofstream &os) const override {};
        GameObjectType getObjectType() override {
            return GameObjectType::SPELL;
        };

    private:
        std::shared_ptr<b2Body> enemySensor;
        ColliderUserData *sdata;
        int updateCalled = 0;
        bool alive = true;
};
