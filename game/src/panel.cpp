#include "panel.hpp"
#include "constants.h"
#include "globals.h"

Panel::Panel(float x, float y, float width, float height, Color color) {
    this->left = x - width / 2;
    this->top = y - height / 2;
    this->width = width;
    this->height = height;
    this->color = color;
}

void Panel::draw() {
    // DrawRectangleRounded({left, top, width, height}, 0.5, 50, color);
    float scale = 2;
    float margin = 1.5 * scale;
    float twidth = 3 * scale;
    getSpriteHolder()->drawSprite(
        PANEL_SPRITE_ID, 0, {left - margin, top - margin, twidth, twidth});
    float right = left + width;
    getSpriteHolder()->drawSprite(
        PANEL_SPRITE_ID, 2, {right - margin, top - margin, twidth, twidth});
    float bottom = top + height;
    getSpriteHolder()->drawSprite(
        PANEL_SPRITE_ID, 6, {left - margin, bottom - margin, twidth, twidth});
    getSpriteHolder()->drawSprite(
        PANEL_SPRITE_ID, 8, {right - margin, bottom - margin, twidth, twidth});

    getSpriteHolder()->drawSprite(
        PANEL_SPRITE_ID, 1,
        {left + margin, top - margin, width - 2 * margin, twidth});
    getSpriteHolder()->drawSprite(
        PANEL_SPRITE_ID, 7,
        {left + margin, bottom - margin, width - 2 * margin, twidth});

    getSpriteHolder()->drawSprite(
        PANEL_SPRITE_ID, 3,
        {left - margin, top + margin, twidth, height - 2 * margin});
    getSpriteHolder()->drawSprite(
        PANEL_SPRITE_ID, 5,
        {right - margin, top + margin, twidth, height - 2 * margin});

    getSpriteHolder()->drawSprite(
        PANEL_BG,
        {left + margin, top + margin, width - 2 * margin, height - 2 * margin});
}

void Panel::update(float dt) {
    // nothing to update
}
