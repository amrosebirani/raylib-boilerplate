#include "upgrade_dialog.hpp"
#include "constants.h"
#include "globals.h"
#include "raylib.h"
#include "utils.h"
#include <algorithm>

UpgradeDialog::UpgradeDialog(PropertyType type, int level, int coingToUpgrade,
                             std::function<void()> dialogUpgradeCallback,
                             BuildingData *buildingData)
    : type(type), level(level), coingToUpgrade(coingToUpgrade),
      dialogUpgradeCallback(dialogUpgradeCallback), buildingData(buildingData) {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    if (sw > sh) {
        float width = sw / 2.5;
        float height = sh;
        panel = std::make_shared<Panel>(sw / 2, sh / 2, width, height);
    } else {
        float width = sw * 4.0f / 5;
        float height = 2 * sh / 3;
        panel = std::make_shared<Panel>(sw / 2, sh / 2, width, height);
    }
    upgradeInfo = getUpgradeInfo(type, level - 1);
    getWorldState()->setPopupActive(true);
}

void UpgradeDialog::draw() {
    DrawRectangleRec({0, 0, GetScreenWidth() * 1.0f, GetScreenHeight() * 1.0f},
                     {0, 0, 0, 200});
    panel->draw();

    getSpriteHolder()->drawSprite(
        CLEAR_ICON, {panel->left + panel->width - 70, panel->top + 20, 50, 50});
    float sw = GetScreenWidth();

    float mm = MeasureText(buildingData->building_text.c_str(), 30);
    float y = panel->top + 20;
    DrawText(buildingData->building_text.c_str(), sw / 2 - mm / 2, y, 30,
             WHITE);
    std::string levelT = "Level " + std::to_string(upgradeInfo->next_level);

    float ml = MeasureText(levelT.c_str(), 15);
    y += 35;

    DrawText(levelT.c_str(), sw / 2 - ml / 2, y, 15, WHITE);

    // draw the building here now
    y += 150;
    getSpriteHolder()->drawSprite(buildingData, sw / 2, y,
                                  getUIPercentCover(type, level), 255, 200);

    y += 90;

    float draw_height = panel->top + panel->height - 100 - y - 10;
    int draw_lines = upgradeInfo->key_points.size();
    int fs = draw_height / (draw_lines * (1.5f));
    fs = std::min(fs, 50);

    for (auto &upgrade : upgradeInfo->key_points) {
        DrawText(upgrade.c_str(),
                 sw / 2 - MeasureText(upgrade.c_str(), fs) / 2.0f, y, fs,
                 WHITE);
        y += fs * 1.5f;
    }

    if (getWorldState()->getCoins() < coingToUpgrade) {
        DrawText("Not enough coins to upgrade",
                 sw / 2 - MeasureText("Not enough coins to upgrade", 15) / 2.0f,
                 panel->top + panel->height - 40, 15, RED);
    } else {
        getSpriteHolder()->drawSprite(BUTTON_ICON,
                                      {sw / 2 - panel->width / 4,
                                       panel->top + panel->height - 100,
                                       panel->width / 2, 80});
        DrawText("Upgrade", sw / 2 - MeasureText("Upgrade", 40) / 2.0f,
                 panel->top + panel->height - 80, 40, BLACK);
    }
}

bool UpgradeDialog::update(float dt) {
    if (finished) {
        return false;
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {

        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse,
                                   {GetScreenWidth() / 2.0f - panel->width / 4,
                                    panel->top + panel->height - 100,
                                    panel->width / 2, 80})) {
            upgradePressed = true;
        }
        if (CheckCollisionPointRec(mouse, {panel->left + panel->width - 100,
                                           panel->top, 100, 200})) {
            upgradePressed = true;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse,
                                   {GetScreenWidth() / 2.0f - panel->width / 4,
                                    panel->top + panel->height - 100,
                                    panel->width / 2, 80}) &&
            upgradePressed) {
            dialogUpgradeCallback();
            finished = true;
            getWorldState()->setPopupActive(false);
        }
        if (CheckCollisionPointRec(mouse, {panel->left + panel->width - 100,
                                           panel->top, 100, 200}) &&
            upgradePressed) {
            finished = true;
            getWorldState()->setPopupActive(false);
        }
        upgradePressed = false;
    }
    return false;
}

bool UpgradeDialog::isFinished() {
    return finished;
}

UpgradeDialog::~UpgradeDialog() {
}
