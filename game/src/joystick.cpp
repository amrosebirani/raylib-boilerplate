#include "joystick.hpp"
#include "constants.h"
#include "draft.h"
#include "globals.h"
#include "raylib.h"
#include "raymath.h"
// #include <iostream>

Joystick::Joystick(Vector2 pos) : position(pos) {
    stickPosition = pos;
    direction = Vector2{0, 0};
    active = false;
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float ss = (sw > sh) ? sw : sh;
    jr = ss * JOYSTICK_RADIUS;
    jbr = ss * JOYSTICK_BASE_RADIUS;
    prohibitedRects = std::vector<Rectangle>();
}

void Joystick::update(float dt) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) ||
        IsGestureDetected(GESTURE_TAP)) {
        if (IsGestureDetected(GESTURE_PINCH_IN) ||
            IsGestureDetected(GESTURE_PINCH_OUT)) {
            active = false;
            return;
        }
        Vector2 touchPos = GetMousePosition();
        // std::cout << "touch Pos" << touchPos.x << " " << touchPos.y
        // << std::endl;

        if (!active) {
            // float distToBase = Vector2Distance(touchPos, position);
            // if (distToBase <= jbr) {
            // active = true;
            // }
            bool prohibited = false;
            for (auto &rect : prohibitedRects) {
                if (CheckCollisionPointRec(touchPos, rect)) {
                    prohibited = true;
                    break;
                }
            }
            if (prohibited) {
                active = false;
                return;
            }
            active = true;
            stickPosition = touchPos;
            position = touchPos;
        }

        if (active) {
            // std::cout << "active" << std::endl;
            if (getViewCamera()->getTarget() == nullptr) {
                getViewCamera()->follow(getContainer()->getFollowObject());
            }
            Vector2 delta = Vector2Subtract(touchPos, position);
            float distance = Vector2Length(delta);

            if (distance > jr) {
                delta = Vector2Scale(Vector2Normalize(delta), jr);
            }

            stickPosition = Vector2Add(position, delta);
            direction = Vector2Normalize(delta);
        }
    } else {
        // std::cout << "not active" << std::endl;
        active = false;
        stickPosition = position;
        direction = Vector2{0, 0};
    }
}

bool Joystick::isActive() {
    return active;
}

void Joystick::setProhibitedRects(std::vector<Rectangle> rects) {
    prohibitedRects = rects;
}

void Joystick::clearProhibitedRects() {
    float ss = prohibitedRects.size();
    if (prohibitedRects.size() > 0) prohibitedRects.clear();
}

void Joystick::draw() {
    if (!active) return;
    DraftCircle(position.x, position.y, jr, 100, DrawMode::LINE,
                Fade(WHITE, 0.6f), 6.0f);
    // DrawCircleLinesV(position, jr, Fade(DARKGRAY, 0.3f));
    DrawCircleV(stickPosition, jr / 3, Fade(LIGHTGRAY, 0.3));
}

Joystick::~Joystick() {
}

Vector2 Joystick::getDirection() {
    return direction;
}
