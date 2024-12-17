#pragma once

#include "constants.h"
#include "game_object.h"
#include "raylib.h"

class LightningLine : public GameObject {
    public:
        LightningLine(float x1, float y1, float x2, float y2,
                      Color c1 = boost_color, Color c2 = default_color);
        void update(float dt) override;
        void draw() override;
        bool isAlive() override;
        void die() override;
        void generateLightning();
        void init() override;
        void cleanupData() override;

    private:
        bool alive = true;
        float x1, y1, x2, y2;
        std::vector<std::pair<Vector2, Vector2>> segments;
        float maximumOffset = 25;
        float generationCount = 5;
        Color c1;
        Color c2;
};
