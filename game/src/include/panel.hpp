#pragma once

#include "raylib.h"
#include "constants.h"

class Panel {
    public:
        void draw();
        void update(float dt);
        Panel(float x, float y, float width, float height,
              Color color = default_color);
        float left;
        float top;
        float width;
        float height;
        Color color;
};
