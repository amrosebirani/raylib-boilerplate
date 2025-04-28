#pragma once
#include "game_object.h"

class ScoreFlash : public GameObject {
    public:
        ScoreFlash(float x, float y, int score)
            : GameObject(x, y), score(score) {};
        void update(float dt) override;
        void draw() override;
        bool isAlive() override;
        void die() override;
        void init() override;
        void cleanupData() override;
        GameObjectType getObjectType() override {
            return GameObjectType::NON_TYPE;
        };
        void Save(std::ofstream &out) const override {
            // No need to save score flash
        };

    private:
        int score;
        float ttl = 1.0f;
        float tt = 0;
        float vy = -20.0f;
        bool alive = true;
};
