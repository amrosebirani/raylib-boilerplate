#pragma once
#include "raylib.h"

class Joystick {
    public:
        Joystick(Vector2 pos);
        ~Joystick();
        void update(float dt);
        void draw();
        Vector2 getDirection();

    private:
        Vector2 position;
        Vector2 stickPosition;
        Vector2 direction;
        bool active;
        float jr;
        float jbr;
};
