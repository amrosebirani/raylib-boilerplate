#include "victory.hpp"
#include "globals.h"
#include "raylib.h"
#include "utils.h"

Victory::Victory() {

    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    rtm = MeasureText(mainMenuText.c_str(), 30);
    if (sw > sh) {
        mainMenuRect = {sw / 2 - rtm / 2 - 30, .85f * sh, rtm + 60, 50};

    } else {
        mainMenuRect = {sw / 2 - rtm / 2 - 10, .8f * sh, rtm + 20, 50};
    }

    victoryL = LoadTexture(getAssetPath("textures/backgrounds/victory_l.png"));
    victoryP = LoadTexture(getAssetPath("textures/backgrounds/victory_p.png"));
}

bool Victory::update(float dt) {

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, mainMenuRect)) {
            finished = true;
            reinitializeGame();
        }
    }
    return true;
}

bool Victory::isFinished() {
    return finished;
}

void Victory::reset() {
    finished = false;
}

Victory::~Victory() {
    UnloadTexture(victoryL);
    UnloadTexture(victoryP);
}

void Victory::draw() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, BLACK);
    if (sw > sh) {
        drawFullScreenTextureWithAspect(victoryL);
    } else {
        drawFullScreenTextureWithAspect(victoryP);
    }

    DrawRectangleGradientV(0, sh * .67f, sw, GetScreenHeight() * .4,
                           {0, 0, 0, 20}, BLACK);
    DrawRectangleRoundedLinesEx(mainMenuRect, 0.5f, 50, 2, WHITE);
    DrawText(mainMenuText.c_str(),
             mainMenuRect.x + mainMenuRect.width / 2 - rtm / 2,
             mainMenuRect.y + 10, 30, WHITE);
}
