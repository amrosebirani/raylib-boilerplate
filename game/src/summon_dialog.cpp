#include "summon_dialog.hpp"
#include "globals.h"
#include "raylib.h"
#include "summon_slot.hpp"

SummonDialog::SummonDialog(
    PropertyType type, int level,
    std::shared_ptr<std::vector<std::shared_ptr<SummonSlot>>> slots,
    BuildingData *buildingData)
    : type(type), level(level), slots(slots), finished(false),
      buildingData(buildingData) {

    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    if (sw > sh) {
        float width = 9.0f * sw / 10;
        float height = sh;
        panel = std::make_shared<Panel>(sw / 2, sh / 2, width, height);
        isPortrait = false;
    } else {
        float width = sw * 4.0f / 5;
        float height = 5 * sh / 6;
        panel = std::make_shared<Panel>(sw / 2, sh / 2, width, height);
        isPortrait = true;
    }
    getWorldState()->setPopupActive(true);
    unSelectAll = [this]() {
        for (auto &slot : *this->slots) {
            slot->setSelected(false);
        }
    };
}

SummonDialog::~SummonDialog() {
}

void SummonDialog::draw() {
    if (GetScreenWidth() > GetScreenHeight()) {
        drawLandscape();
    } else {
        drawPortrait();
    }
}

void SummonDialog::drawLandscape() {

    DrawRectangleRec({0, 0, GetScreenWidth() * 1.0f, GetScreenHeight() * 1.0f},
                     {0, 0, 0, 200});
    panel->draw();
    getSpriteHolder()->drawSprite(CLEAR_ICON, {panel->left + panel->width - 100,
                                               panel->top + 20, 80, 80});
    float sw = GetScreenWidth();

    float mm = MeasureText(buildingData->building_text.c_str(), 30);
    float y = panel->top + 20;
    DrawText(buildingData->building_text.c_str(), sw / 2 - mm / 2, y, 30,
             WHITE);
    std::string levelT = "Level " + std::to_string(level);
    float ml = MeasureText(levelT.c_str(), 15);
    y += 35;

    DrawText(levelT.c_str(), sw / 2 - ml / 2, y, 15, WHITE);
    y += 30;
    float detail_y = y;
    float m_end = panel->left + panel->width * .33f;

    DrawText("Training Grounds", panel->left + panel->width * .11f, y, 20,
             WHITE);

    y += 30;

    if (availableSlots.size() > 0) {
        DrawText("Available", panel->left + panel->width * .11f, y, 15, WHITE);
        y += 30;

        for (auto &slot : availableSlots) {
            slot->drawSlotAvailableMaster(panel, y);
            y += 75;
            if (slot->isSelected()) {
                slot->drawSlotAvailableDetail(panel, detail_y);
            }
        }
    }
    if (producingSlots.size() > 0) {
        DrawText("In Use", panel->left + panel->width * .11f, y, 15, WHITE);
        y += 30;
        for (auto &slot : producingSlots) {
            slot->drawSlotProducingMaster(panel, y);
            y += 75;
            if (slot->isSelected()) {
                slot->drawSlotProducingDetail(panel, detail_y);
            }
        }
    }
    // getSpriteHolder()->drawSprite(
    // BUTTON_ICON, {sw / 2 - panel->width / 4,
    // panel->top + panel->height - 100, panel->width / 2, 80});
    // DrawText("Exit", sw / 2 - MeasureText("Exit", 20) / 2.0f,
    // panel->top + panel->height - 70, 20, BLACK);
}

void SummonDialog::drawPortrait() {
    DrawRectangleRec({0, 0, GetScreenWidth() * 1.0f, GetScreenHeight() * 1.0f},
                     {0, 0, 0, 200});
    panel->draw();

    getSpriteHolder()->drawSprite(CLEAR_ICON, {panel->left + panel->width - 100,
                                               panel->top + 20, 80, 80});
    float sw = GetScreenWidth();

    float mm = MeasureText(buildingData->building_text.c_str(), 30);
    float y = panel->top + 20;
    DrawText(buildingData->building_text.c_str(), sw / 2 - mm / 2, y, 30,
             WHITE);
    std::string levelT = "Level " + std::to_string(level);

    float ml = MeasureText(levelT.c_str(), 15);
    y += 35;

    DrawText(levelT.c_str(), sw / 2 - ml / 2, y, 15, WHITE);

    y += 30;

    DrawLine(sw / 2 - panel->width / 2 + 10, y, sw / 2 + panel->width / 2 - 10,
             y, WHITE);
    y += 20;

    if (availableSlots.size() > 0) {
        DrawText("Available Training Grounds",
                 sw / 2 - MeasureText("Available Training Grounds", 20) / 2.0f,
                 y, 20, WHITE);

        y += 30;
        for (auto &slot : availableSlots) {
            slot->drawSlotAvailable(panel, y);
            y += 95;
        }

        // y -= 20;

        // DrawLine(sw / 2 - panel->width / 2 + 10, y,
        // sw / 2 + panel->width / 2 - 10, y, WHITE);
    }

    if (producingSlots.size() > 0) {
        DrawText("Training Grounds in Use",
                 sw / 2 - MeasureText("Training Grounds in Use", 20) / 2.0f, y,
                 20, WHITE);

        y += 30;
        for (auto &slot : producingSlots) {
            slot->drawSlotProducing(panel, y);
            y += 85;
        }
        // y += 20;

        // DrawLine(sw / 2 - panel->width / 2 + 10, y,
        // sw / 2 + panel->width / 2 - 10, y, WHITE);
    }
    getSpriteHolder()->drawSprite(
        BUTTON_ICON, {sw / 2 - panel->width / 4,
                      panel->top + panel->height - 100, panel->width / 2, 80});
    DrawText("Exit", sw / 2 - MeasureText("Exit", 20) / 2.0f,
             panel->top + panel->height - 70, 20, BLACK);
}

bool SummonDialog::update(float dt) {
    if (finished) {
        return false;
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse,
                                   {GetScreenWidth() / 2.0f - panel->width / 4,
                                    panel->top + panel->height - 100,
                                    panel->width / 2, 80}) &&
            isPortrait) {
            buttonPressed = true;
        }
        if (CheckCollisionPointRec(mouse, {panel->left + panel->width - 100,
                                           panel->top + 20, 80, 80})) {
            buttonPressed = true;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse,
                                   {GetScreenWidth() / 2.0f - panel->width / 4,
                                    panel->top + panel->height - 100,
                                    panel->width / 2, 80}) &&
            buttonPressed && isPortrait) {
            finished = true;
            getWorldState()->setPopupActive(false);
        }
        if (CheckCollisionPointRec(mouse, {panel->left + panel->width - 100,
                                           panel->top + 20, 80, 80}) &&
            buttonPressed) {
            finished = true;
            getWorldState()->setPopupActive(false);
        }
        buttonPressed = false;
    }
    availableSlots.clear();
    producingSlots.clear();
    for (auto &slot : *slots) {
        slot->update(dt);
        if (!slot->getIsProducing()) {
            availableSlots.push_back(slot);
            if (isPortrait) {
                slot->handleClick();
            }
        } else {
            producingSlots.push_back(slot);
        }
        if (!isPortrait) {
            slot->handleLandscapeClick(unSelectAll);
        }
    }
    return false;
}

bool SummonDialog::isFinished() {
    return finished;
}
