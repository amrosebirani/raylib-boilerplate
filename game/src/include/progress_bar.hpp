#pragma once
#include "raylib.h"

class ProgressBar {
    public:
        ProgressBar(float x, float y, float width, float height, float max,
                    float current, Color color = GREEN);
        void draw();
        void setCurrent(float current);
        void reset(float max, float current);
        void setY(float y);

    private:
        float x;
        float y;
        float width;
        float height;
        float max;
        float current;
        Color color;
};
