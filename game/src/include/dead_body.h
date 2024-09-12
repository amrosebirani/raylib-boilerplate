#pragma once
#include "game_object.h"
#include "enemy_types.h"
#include "warrior_types.h"
#include "raylib.h"

class DeadBody : public GameObject {
    public:
        DeadBody(float x, float y, bool isWarrior, EnemyType enemyType,
                 WarriorType warriorType);
        void draw() override;
        void update(float dt) override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;

    private:
        bool alive = true;
        bool isWarrior;
        EnemyType enemyType;
        WarriorType warriorType;
        Color drawColor = WHITE;
        float total_time = 0.0f;
        float size;
        std::string sprite_id;
        int frame_no = 0;
};
