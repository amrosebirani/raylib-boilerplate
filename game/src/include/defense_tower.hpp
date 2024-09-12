#pragma once

#include "game_object.h"
#include "collider_user_data.h"
#include "box2d/b2_body.h"
#include "progress_bar.hpp"
#include <queue>
#include <memory>

class DefenseTower : public GameObject {
    public:
        DefenseTower(float x, float y, int archers);
        ~DefenseTower();
        void draw() override;
        void update(float dt) override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
        void addEnemy(std::shared_ptr<GameObject> enemy);
        std::queue<std::shared_ptr<GameObject>> enemies;
        void hit(float damage);

    private:
        int archers;
        bool alive = true;
        float circle_radius;
        ColliderUserData *data;
        std::shared_ptr<b2Body> collider;
        float health = 5000;
        float maxHealth = 5000;
        bool isAttacked = false;
        std::shared_ptr<ProgressBar> healthBar;
};
