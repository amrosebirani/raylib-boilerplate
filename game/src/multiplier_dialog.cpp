#include "multiplier_dialog.hpp"
#include "constants.h"
#include "globals.h"
#include "raylib.h"

MultiplierDialog::MultiplierDialog() {
    upgrades = getContainer()->getUpgradeManager()->getUpgrades();
    if (upgrades.empty()) {
        finished = true;
    }
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    if (sw > sh) {
        float hh = sh / 3;
        float y = sh / 3;
        rect0 = {sw / 6 - hh / 2, y, hh, hh};
        rect1 = {sw / 2 - hh / 2, y, hh, hh};
        rect2 = {5 * sw / 6 - hh / 2, y, hh, hh};
        rect3 = {sw / 4 - hh / 2, y, hh, hh};
        rect4 = {3 * sw / 4 - hh / 2, y, hh, hh};
    } else {
        float ww = sw / 2;
        float x = sw / 4;

        rect0 = {x, sh / 6 - ww / 2, ww, ww};
        rect1 = {x, 3 * sh / 6 - ww / 2, ww, ww};
        rect2 = {x, 5 * sh / 6 - ww / 2, ww, ww};
        rect3 = {x, sh / 4 - ww / 2, ww, ww};
        rect4 = {x, 3 * sh / 4 - ww / 2, ww, ww};
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        isLocked = true;
    }
}

MultiplierDialog::~MultiplierDialog() {
}

bool MultiplierDialog::isFinished() {

    return finished;
}

void MultiplierDialog::draw() {
    DrawRectangleRec({0, 0, GetScreenWidth() * 1.0f, GetScreenHeight() * 1.0f},
                     {0, 0, 0, 225});
    const char *txt = "Select an upgrade!";
    float mmt = MeasureText(txt, 50);
    DrawText(txt, GetScreenWidth() / 2.0f - mmt / 2, GetScreenHeight() * .15f,
             50, WHITE);
    if (upgrades.size() == 3) {
        getSpriteHolder()->drawSprite(UPGRADE_CARD, rect0);
        getSpriteHolder()->drawSprite(UPGRADE_CARD, rect1);
        getSpriteHolder()->drawSprite(UPGRADE_CARD, rect2);
        drawUpgrade(rect0, upgrades[0]);
        drawUpgrade(rect1, upgrades[1]);
        drawUpgrade(rect2, upgrades[2]);
    }
    if (upgrades.size() == 2) {
        getSpriteHolder()->drawSprite(UPGRADE_CARD, rect3);
        getSpriteHolder()->drawSprite(UPGRADE_CARD, rect4);
        drawUpgrade(rect3, upgrades[0]);
        drawUpgrade(rect4, upgrades[1]);
    }

    if (upgrades.size() == 1) {
        getSpriteHolder()->drawSprite(UPGRADE_CARD, rect1);
        drawUpgrade(rect1, upgrades[0]);
    }
}

void MultiplierDialog::drawUpgrade(Rectangle rect,
                                   std::shared_ptr<UpgradeAction> upgrade) {

    int roe = upgrade->upgrade->rate_of_change;
    std::string ht = TextFormat(
        "%d%s", abs(roe),
        upgrade->upgrade->isPercent || upgrade->upgrade->isChance ? "%" : "");
    float fs = rect.height / 4;
    float mm = MeasureText(ht.c_str(), fs);
    float tw = fs * 1.6 + mm;
    float start_x = rect.x + rect.width / 2 - tw / 2;
    float y = rect.y + rect.height * .2f;
    std::string icon_sprite = roe > 0 ? UPG_ICON : ARROW_BOTTOM;

    getSpriteHolder()->drawSpriteWithColor(
        icon_sprite, {start_x, y - fs * .3f, fs * 1.6f, fs * 1.6f}, DARKGREEN);
    start_x += fs * 1.6;
    DrawText(ht.c_str(), start_x, y, fs, DARKGREEN);
    std::vector<std::string> display_strs =
        getContainer()->getUpgradeContent()->get_display_strs(
            upgrade->upgrade->multiplier_id);
    drawUpgradeText(rect, display_strs);
}

void MultiplierDialog::drawUpgradeText(Rectangle rect,
                                       std::vector<std::string> display_strs) {
    if (display_strs.size() == 0) {
        return;
    }
    float fs = rect.height / 12;
    if (display_strs.size() == 1) {
        float mm = MeasureText(display_strs[0].c_str(), fs);
        DrawText(display_strs[0].c_str(), rect.x + rect.width / 2 - mm / 2,
                 rect.y + rect.height - rect.height * .25f, fs, WHITE);
        return;
    }
    float start_y = rect.y + rect.height - rect.height * .4f;
    for (auto &str : display_strs) {
        float mm = MeasureText(str.c_str(), fs);
        DrawText(str.c_str(), rect.x + rect.width / 2 - mm / 2, start_y, fs,
                 WHITE);
        start_y += fs;
    }
}

void MultiplierDialog::handleClick(std::vector<Rectangle> rects) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        for (auto &rect : rects) {
            if (CheckCollisionPointRec(mouse, rect)) {
                isPressed = true;
            }
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isPressed) {
        Vector2 mouse = GetMousePosition();
        for (int i = 0; i < rects.size(); i++) {
            if (CheckCollisionPointRec(mouse, rects[i])) {
                upgrades[i]->action();
                finished = true;
                break;
            }
        }
        isPressed = false;
    }
}

bool MultiplierDialog::update(float dt) {
    if (finished) {
        return false;
    }
    if (upgrades.size() == 0) {
        finished = true;
        return false;
    }
    if (isLocked && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isLocked = false;
    }
    if (isLocked) {
        return false;
    }
    if (upgrades.size() == 1) {
        handleClick({rect1});
    }
    if (upgrades.size() == 2) {
        handleClick({rect3, rect4});
    }
    if (upgrades.size() == 3) {
        handleClick({rect0, rect1, rect2});
    }
    return false;
}
