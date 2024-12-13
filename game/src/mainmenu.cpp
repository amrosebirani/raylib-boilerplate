#include "mainmenu.hpp"
#include "constants.h"
#include "globals.h"
#include "raylib.h"

MainMenu::MainMenu() {
    level1 = LoadTexture(getAssetPath("textures/avanti.png"));
    level2 = LoadTexture(getAssetPath("textures/gandhara.png"));
    float mm = MeasureText("Start", 20);
    float sh = GetScreenHeight();
    float hh = sh / 2;
    start1Rect = {30 + hh / 2 - mm / 2 - 20, hh - 20, mm + 40, 40};
}

MainMenu::~MainMenu() {
    UnloadTexture(level1);
    UnloadTexture(level2);
}

bool MainMenu::isFinished() {
    return finished;
}

void MainMenu::reset() {
    finished = false;
}

void MainMenu::draw() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float hh = sh / 2;
    DrawText("Levels", 30, 30, 40, WHITE);

    DrawTexturePro(level1, {0, 0, level1.width * 1.0f, level1.height * 1.0f},
                   {30, hh / 2, hh, hh}, {0, 0}, 0, WHITE);
    DrawTexturePro(level2, {0, 0, level2.width * 1.0f, level2.height * 1.0f},
                   {30 + hh + 30, hh / 2, hh, hh}, {0, 0}, 0, WHITE);
    DrawRectangleRec({30, 1.5f * hh - 40, hh, 40}, {0, 0, 0, 200});
    DrawRectangleRec({30 + hh + 30, 1.5f * hh - 40, hh, 40}, {0, 0, 0, 200});
    DrawText("Avanti", 30 + hh / 2 - MeasureText("Avanti", 20), 1.5 * hh - 30,
             20, WHITE);
    DrawText("Gandhara", 30 + hh + 30 + hh / 2 - MeasureText("Gandhara", 20),
             1.5 * hh - 30, 20, WHITE);
    DrawRectangleRec({30 + hh + 30, hh / 2, hh, hh}, {0, 0, 0, 200});
    getSpriteHolder()->drawSprite(BUTTON_ICON, start1Rect);
    DrawText("Start", 30 + hh / 2 - MeasureText("Start", 20) / 2.0f, hh - 10,
             20, WHITE);
}

bool MainMenu::update(float dt) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, start1Rect)) {
            finished = true;
            // reinitializeGame();
            startGame();
        }
    }
    return false;
}
