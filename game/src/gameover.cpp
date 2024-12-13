#include "gameover.hpp"
#include "game.h"
#include "globals.h"
#include "raylib.h"
#include "utils.h"

GameOver::GameOver() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    rtm = MeasureText(restartText.c_str(), 30);
    if (sw > sh) {
        restartRect = {sw / 2 - rtm / 2 - 30, .85f * sh, rtm + 60, 50};

    } else {
        restartRect = {sw / 2 - rtm / 2 - 10, .8f * sh, rtm + 20, 50};
    }

    gameoverL =
        LoadTexture(getAssetPath("textures/backgrounds/gameover_l.png"));
    gameoverP =
        LoadTexture(getAssetPath("textures/backgrounds/gameover_p.png"));
}

bool GameOver::update(float dt) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, restartRect)) {
            finished = true;
            reinitializeGame();
        }
    }
    return true;
}

bool GameOver::isFinished() {
    return finished;
}

void GameOver::reset() {
    finished = false;
}

GameOver::~GameOver() {

    // UnloadTexture(deadBody);
    UnloadTexture(gameoverL);
    UnloadTexture(gameoverP);
}

void GameOver::draw() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, BLACK);
    if (sw > sh) {
        drawFullScreenTextureWithAspect(gameoverL);
    } else {
        drawFullScreenTextureWithAspect(gameoverP);
    }

    DrawRectangleGradientV(0, sh * .67f, sw, GetScreenHeight() * .4,
                           {0, 0, 0, 20}, BLACK);
    DrawRectangleRoundedLinesEx(restartRect, 0.5f, 50, 2, WHITE);
    DrawText(restartText.c_str(),
             restartRect.x + restartRect.width / 2 - rtm / 2,
             restartRect.y + 10, 30, WHITE);
    DrawText("Game Over", sw / 2 - MeasureText("Game Over", 70) / 2.0f,
             sh / 2 - 35, 70, WHITE);
}
