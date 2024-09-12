#pragma once

class ProgressBar {
    public:
        ProgressBar(float x, float y, float width, float height, float max,
                    float current);
        void draw();
        void setCurrent(float current);

    private:
        float x;
        float y;
        float width;
        float height;
        float max;
        float current;
};
