#include "progress_bar.hpp"
#include "raylib.h"

ProgressBar::ProgressBar(float x, float y, float width, float height, float max,
                         float current, Color color)
    : x(x), y(y), width(width), height(height), max(max), current(current),
      color(color) {
}

void ProgressBar::setCurrent(float current) {
    this->current = current;
}

void ProgressBar::draw() {
    float r = current / max;
    float w = width * r;
    DrawRectangle(x, y, width, height, {BLACK.r, BLACK.g, BLACK.b, 155});
    DrawRectangle(x, y, w, height, color);
}

void ProgressBar::reset(float max, float current) {
    this->max = max;
    this->current = current;
}

void ProgressBar::setY(float y) {
    this->y = y;
}
