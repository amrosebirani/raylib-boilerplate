#include "progress_bar.hpp"
#include "raylib.h"

ProgressBar::ProgressBar(float x, float y, float width, float height, float max,
                         float current)
    : x(x), y(y), width(width), height(height), max(max), current(current) {
}

void ProgressBar::setCurrent(float current) {
    this->current = current;
}

void ProgressBar::draw() {
    float r = current / max;
    float w = width * r;
    DrawRectangle(x, y, width, height, BLACK);
    DrawRectangle(x, y, w, height, GREEN);
}
