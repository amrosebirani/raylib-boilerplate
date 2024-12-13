#pragma once
#include "raylib.h"
#include <vector>

class Joystick {
    public:
        Joystick(Vector2 pos);
        ~Joystick();
        void update(float dt);
        void draw();
        Vector2 getDirection();
        void setProhibitedRects(std::vector<Rectangle> rects);
        void clearProhibitedRects();
        bool isActive();

    private:
        Vector2 position;
        Vector2 stickPosition;
        Vector2 direction;
        bool active;
        float jr;
        float jbr;
        std::vector<Rectangle> prohibitedRects;
};
