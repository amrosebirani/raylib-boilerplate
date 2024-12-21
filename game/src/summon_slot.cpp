#include "summon_slot.hpp"
#include "constants.h"
#include "firebase.hpp"
#include "globals.h"
#include "raylib.h"
#include "summon_card.hpp"
#include "utils.h"
#include "warrior_types.h"
#include "platform.hpp"

SummonSlot::SummonSlot(int index, int level, PropertyType type,
                       SummonCardType stype)
    : level(level), type(type), summon_type(stype), index(index) {
    warriorTypes = getSummonChoices(type, level);
    setProduceCost();
}

void SummonSlot::setProduceCost() {
    float mm = 1;
    if (summon_type == SummonCardType::INFANTRY) {
        mm =
            getContainer()->getUpgradeContent()->get_stat(BARRACKS_UNIT_COST_M);
    }
    if (summon_type == SummonCardType::ARCHERY) {
        mm = getContainer()->getUpgradeContent()->get_stat(ARCHERY_UNIT_COST_M);
    }
    if (summon_type == SummonCardType::WIZARDRY) {
        mm =
            getContainer()->getUpgradeContent()->get_stat(WIZARDRY_UNIT_COST_M);
    }

    produceCost =
        get_warrior_summon_card_cost(warriorTypes[selectedIndex], level) * mm;
}

SummonSlot::~SummonSlot() {
}

void SummonSlot::setSelected(bool selected) {
    this->selected = selected;
}

bool SummonSlot::isSelected() {
    return selected;
}

void SummonSlot::drawSlotAvailableMaster(std::shared_ptr<Panel> panel,
                                         float y) {
    Color bgColor = {0, 0, 0, 200};
    if (selected) {
        bgColor = WHITE;
    }
    Color textColor = WHITE;
    if (selected) {
        textColor = DARKBLUE;
    }
    float ww = panel->width * .22f;
    Rectangle rect = {panel->left + panel->width * .05f, y, ww, 60};
    masterRect = rect;
    DrawRectangleRec(rect, bgColor);
    std::string tt = TextFormat("Arena %d", index);
    float ttm = MeasureText(tt.c_str(), 40);
    DrawText(tt.c_str(), rect.x + rect.width / 2 - ttm / 2, y + 10, 40,
             textColor);
}

void SummonSlot::drawSlotProducingDetail(std::shared_ptr<Panel> panel,
                                         float y) {
    float ph = panel->height * .6f;
    std::string sprite_id =
        get_warrior_sprite_ids(warriorTypes[selectedIndex])[5];
    Rectangle rect = {panel->left + panel->width * .66f - ph / 2,
                      panel->top + 40, ph, ph};
    getSpriteHolder()->drawSprite(sprite_id, rect);
    std::string wText = getWarriorText(warriorTypes[selectedIndex]);
    float wtm = MeasureText(wText.c_str(), 30);
    DrawText(wText.c_str(), panel->left + panel->width * .66f - wtm / 2,
             panel->top + 40 + ph + 10, 30, WHITE);
    Vector2 sdim = getBuildingSummonDim(type, level);
    std::string dimText = TextFormat("%d x %d", (int)sdim.x, (int)sdim.y);
    float dtm = MeasureText(dimText.c_str(), 20);
    DrawText(dimText.c_str(), panel->left + panel->width * .66f - dtm / 2,
             panel->top + 40 + ph + 10 + 40, 20, WHITE);
    progressBar = std::make_shared<ProgressBar>(
        panel->left + panel->width * .66f - panel->width * .11f,
        panel->top + ph + 130, panel->width * .22f, 60, produceTime,
        produceTimeCounter);
    progressBar->draw();
}

void SummonSlot::drawSlotAvailableDetail(std::shared_ptr<Panel> panel,
                                         float y) {
    float cy = panel->top + panel->height / 2;
    float sx = panel->left + panel->width * .33f;
    leftCaretRect = {sx + 20, cy - 40, 80, 80};
    rightCaretRect = {panel->left + panel->width - 100, cy - 40, 80, 80};
    getSpriteHolder()->drawSpriteWithColor(
        ARROW_PREV, leftCaretRect, selectedIndex == 0 ? DARKGRAY : WHITE);
    getSpriteHolder()->drawSpriteWithColor(
        ARROW_NEXT, rightCaretRect,
        selectedIndex == warriorTypes.size() - 1 ? DARKGRAY : WHITE);
    float ph = panel->height * .6f;
    std::string sprite_id =
        get_warrior_sprite_ids(warriorTypes[selectedIndex])[5];
    Rectangle rect = {panel->left + panel->width * .66f - ph / 2,
                      panel->top + 40, ph, ph};
    getSpriteHolder()->drawSprite(sprite_id, rect);
    std::string wText = getWarriorText(warriorTypes[selectedIndex]);
    float wtm = MeasureText(wText.c_str(), 30);
    DrawText(wText.c_str(), panel->left + panel->width * .66f - wtm / 2,
             panel->top + 40 + ph + 10, 30, WHITE);
    Vector2 sdim = getBuildingSummonDim(type, level);
    std::string dimText = TextFormat("%d x %d", (int)sdim.x, (int)sdim.y);
    float dtm = MeasureText(dimText.c_str(), 20);
    DrawText(dimText.c_str(), panel->left + panel->width * .66f - dtm / 2,
             panel->top + 40 + ph + 10 + 40, 20, WHITE);
    startPRect = {panel->left + panel->width * .66f - panel->width * .11f,
                  panel->top + ph + 130, panel->width * .22f, 80};
    getSpriteHolder()->drawSprite(BUTTON_ICON, startPRect);
    const char *tt = TextFormat("%d", produceCost);
    float ttm = MeasureText(tt, 30);
    float tw = ttm + 50;
    float mr = startPRect.width / 2 - tw / 2;
    DrawText(TextFormat("%d", produceCost), startPRect.x + mr,
             startPRect.y + 25, 30, BLACK);
    getSpriteHolder()->drawSprite(
        GOLD_ICON,
        {startPRect.x + startPRect.width - mr - 30, startPRect.y + 25, 30, 30});
    if (getWorldState()->getCoins() < produceCost) {
        DrawRectangleRec(startPRect, {0, 0, 0, 200});
    }
}

void SummonSlot::drawSlotProducingMaster(std::shared_ptr<Panel> panel,
                                         float y) {

    Color bgColor = {0, 0, 0, 200};
    if (selected) {
        bgColor = WHITE;
    }
    Color textColor = WHITE;
    if (selected) {
        textColor = DARKBLUE;
    }

    float ww = panel->width * .22f;
    progressBar =
        std::make_shared<ProgressBar>(panel->left + panel->width * .05f, y, ww,
                                      60, produceTime, produceTimeCounter);
    progressBar->draw();

    Rectangle rect = {panel->left + panel->width * .05f, y, ww, 60};
    masterRect = rect;
    DrawRectangleRec(rect, bgColor);
    std::string tt = TextFormat("Arena %d", index);
    float ttm = MeasureText(tt.c_str(), 40);
    DrawText(tt.c_str(), rect.x + rect.width / 2 - ttm / 2, y + 10, 40,
             textColor);
}

void SummonSlot::update(float dt) {
    if (isProducing) {
        produceTimeCounter += dt;
        if (produceTimeCounter > produceTime) {
            std::shared_ptr<SummonCard> card =
                std::make_shared<SummonCard>(level, warriorType, summon_type);
            std::string card_type;
            switch (card->summon_type) {
            case SummonCardType::ARCHERY:
                card_type = "archery";
                break;
            case SummonCardType::INFANTRY:
                card_type = "infantry";
                break;
            case SummonCardType::WIZARDRY:
                card_type = "wizardry";
                break;
            };
            sendFirebaseEvent("SummonCardCreated",
                              {{"card_type", card_type},
                               {"level", TextFormat("%d", card->level)},
                               {"wtype", getWarriorText(card->type)},
                               {"magic_type", getMagicText(card->magic_type)}});
            getWorldState()->summon_manager->addSummonCard(card);
            isProducing = false;
            selectedIndex = 0;
        }
    }
}

void SummonSlot::startProducing(WarriorType wtype) {
    isProducing = true;
    produceTime = getBuildingProduceTime(type, level);
    produceTimeCounter = 0;
    warriorType = wtype;
    getWorldState()->removeCoins(produceCost);
}

float SummonSlot::getProgress() {
    return produceTimeCounter / produceTime;
}

void SummonSlot::setLevel(int level) {
    this->level = level;
    warriorTypes = getSummonChoices(type, level);
}

void SummonSlot::drawSlotAvailable(std::shared_ptr<Panel> panel, float y) {
    DrawRectangleRec({panel->left + 15, y, panel->width - 30, 85},
                     {0, 0, 0, 200});
    float start_x = panel->left + 15 + 10;
    float vy = y + 2.5;
    clickRects.clear();
    int index = 0;
    for (auto &wt : warriorTypes) {
        // std::string sprite_id = get_warrior_sprite_ids(wt)[4];
        Rectangle rect = {start_x, vy, 80, 80};
        clickRects.push_back(rect);
        DrawRectangleRec(rect, WHITE);
        draw_warrior_icon(wt, rect);
        // getSpriteHolder()->drawSprite(sprite_id, rect);
        if (selectedIndex >= 0 && selectedIndex == index) {
            DrawRectangleLinesEx(rect, 3, YELLOW);
        }
        index++;
        start_x += 85;
    }
    if (selectedIndex >= 0) {
        float bt_width = MeasureText(TextFormat("%d", produceCost), 20);
        bt_width += 10;
        bt_width += 20;
        bt_width += 10;
        startProduceRect = {panel->left + panel->width - 25 - bt_width, vy,
                            bt_width, 80};
        getSpriteHolder()->drawSprite(BUTTON_ICON, startProduceRect);
        DrawText(TextFormat("%d", produceCost), startProduceRect.x + 5,
                 startProduceRect.y + 30, 20, BLACK);
        getSpriteHolder()->drawSprite(
            GOLD_ICON, {startProduceRect.x + startProduceRect.width - 5 - 20,
                        static_cast<float>(startProduceRect.y + 30), 20, 20});
        if (getWorldState()->getCoins() < produceCost) {
            DrawRectangleRec(startProduceRect, {0, 0, 0, 200});
        }
    }
}

void SummonSlot::drawSlotProducing(std::shared_ptr<Panel> panel, float y) {

    DrawRectangleRec({panel->left + 15, y, panel->width - 30, 75},
                     {0, 0, 0, 150});
    float start_x = panel->left + 15 + 10;
    float vy = y + 5;
    WarriorType wt = warriorType;
    // std::string sprite_id = get_warrior_sprite_ids(wt)[4];
    Rectangle rect = {start_x, vy, 65, 65};
    DrawRectangleRec(rect, WHITE);
    draw_warrior_icon(wt, rect);
    // getSpriteHolder()->drawSprite(sprite_id, rect);
    progressBar =
        std::make_shared<ProgressBar>(start_x + 80, vy + 10, panel->width - 135,
                                      45, produceTime, produceTimeCounter);
    progressBar->draw();
}

void SummonSlot::handleLandscapeClick(std::function<void()> unSelectAll) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, masterRect)) {
            mpressed = true;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && mpressed) {

        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, masterRect)) {
            unSelectAll();
            selected = true;
        }
        mpressed = false;
    }
    if (isProducing || !selected) return;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, leftCaretRect)) {
            lpressed = true;
        }
        if (CheckCollisionPointRec(mp, rightCaretRect)) {
            rpressed = true;
        }
        if (CheckCollisionPointRec(mp, startPRect)) {
            spressed = true;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && lpressed) {
        Vector2 mp = GetMousePosition();
        if (selectedIndex > 0 && CheckCollisionPointRec(mp, leftCaretRect)) {
            selectedIndex--;
            setProduceCost();
        }
        lpressed = false;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && rpressed) {
        Vector2 mp = GetMousePosition();
        if (selectedIndex < warriorTypes.size() - 1 &&
            CheckCollisionPointRec(mp, rightCaretRect)) {
            selectedIndex++;
            setProduceCost();
        }
        rpressed = false;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && spressed) {
        Vector2 mp = GetMousePosition();
        if (getWorldState()->getCoins() >= produceCost &&
            CheckCollisionPointRec(mp, startPRect)) {
            startProducing(warriorTypes[selectedIndex]);
        }
        spressed = false;
    }
    if (isPlatformAndroid()) {
        if (IsGestureDetected(GESTURE_SWIPE_LEFT)) {
            if (selectedIndex < warriorTypes.size() - 1) {
                selectedIndex++;
                setProduceCost();
            }
        }
        if (IsGestureDetected(GESTURE_SWIPE_RIGHT)) {
            if (selectedIndex > 0) {
                selectedIndex--;
                setProduceCost();
            }
        }
    }
}

void SummonSlot::handleClick() {
    if (isProducing) {
        return;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        int index = 0;
        for (auto &rect : clickRects) {
            if (CheckCollisionPointRec(mp, rect)) {
                selectedIndex = index;
                setProduceCost();
                return;
            }
            index++;
        }
        if (selectedIndex >= 0 &&
            CheckCollisionPointRec(mp, startProduceRect)) {
            if (getWorldState()->getCoins() < produceCost) {
                return;
            }
            startProducing(warriorTypes[selectedIndex]);
        }
    }
}
