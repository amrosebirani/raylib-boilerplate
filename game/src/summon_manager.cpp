#include "summon_manager.hpp"
#include "archery_summon.hpp"
#include "globals.h"
#include "infantry_summon.hpp"
#include "raylib.h"

using std::string;

SummonManager::SummonManager() {
    summon_box_h = 6 * (summon_font) + 7 * summon_font / 4.0f;
    summon_box_y = GetScreenHeight() / 1.5f - summon_box_h / 2.0f;
    summon_box = LoadRenderTexture(summon_width, summon_box_h);
    std::vector<string> summon_l = {"S", "U", "M", "M", "O", "N"};
    BeginTextureMode(summon_box);
    ClearBackground(BLANK);
    DrawRectangleRounded({0, 0, summon_width, summon_box_h}, .5, 50, PURPLE);
    for (int i = 0; i < summon_l.size(); i++) {
        float w = MeasureText(summon_l[i].c_str(), summon_font);
        DrawText(summon_l[i].c_str(), summon_width * .75 - w / 2,
                 summon_font * .25f + i * summon_font * 1.25f, summon_font,
                 WHITE);
    }
    EndTextureMode();
    initiliazeClickSlots();
}

SummonManager::~SummonManager() {
    for (auto &cs : clickSlots) {
        delete cs;
    }
}

void SummonManager::initiliazeClickSlots() {
    float start_x = summon_width / 2 + 20;
    for (int i = 0; i < 4; i++) {
        Rectangle rec = {start_x, summon_box_y, summon_width, summon_box_h};
        ClickSlot *cs =
            new ClickSlot(rec, false, SummonCardType::INFANTRY, start_x);
        clickSlots.push_back(cs);
        start_x += summon_width + 20;
    }
}

void SummonManager::resetClickSlots() {
    for (auto &cs : clickSlots) {
        cs->isActive = false;
    }
}

void SummonManager::update(float dt) {
    if (!enabled) {
        summonChooserOpen = false;
        troopTypeChooserOpen = false;
        return;
    }
    // if (!summonActivated) {
    //     summonTimer += dt;
    //     if (summonTimer >= summonRT) {
    //         summonActivated = true;
    //         summonTimer = 0;
    //     }
    // }
    //

    totalCards = summonCardsInfantry.size() + summonCardsArchery.size() +
                 summonCardsWizardry.size();

    // if (summonActivated && IsKeyPressed(KEY_J) && !summonCards.empty()) {
    //     summonActivated = false;
    //     auto card = summonCards.front();
    //     summonCards.pop();
    //     Vector2 pos = getContainer()->getFormPos();
    //     InfantrySummon *inf = new InfantrySummon(pos, card);
    //     delete inf;
    // }
    //
    if (totalCards > 0 && !summonChooserOpen &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(
                mousePos,
                {0, summon_box_y, summon_width / 2.0f, summon_box_h})) {
            summonChooserOpen = true;
            resetClickSlots();
            int slot = 0;
            if (summonCardsInfantry.size() > 0) {

                clickSlots[slot]->isActive = true;
                clickSlots[slot]->type = SummonCardType::INFANTRY;
                slot++;
            }
            if (summonCardsArchery.size() > 0) {
                clickSlots[slot]->isActive = true;
                clickSlots[slot]->type = SummonCardType::ARCHERY;
                slot++;
            }
            if (summonCardsWizardry.size() > 0) {
                clickSlots[slot]->isActive = true;
                clickSlots[slot]->type = SummonCardType::WIZARDRY;
            }
        }
    }
    if (summonChooserOpen) {
        for (auto &cs : clickSlots) {
            if (cs->isActive && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, cs->rec)) {
                    switch (cs->type) {
                    case SummonCardType::INFANTRY: {
                        auto card = summonCardsInfantry.front();
                        summonCardsInfantry.pop();
                        Vector2 pos = getContainer()->getFormPos();
                        InfantrySummon *inf = new InfantrySummon(pos, card);
                        delete inf;
                        break;
                    }
                    case SummonCardType::ARCHERY:
                        // summonCardsArchery.pop();
                        {

                            auto card = summonCardsArchery.front();
                            summonCardsArchery.pop();
                            Vector2 pos = getContainer()->getFormPos();
                            ArcherySummon *inf = new ArcherySummon(pos, card);
                            delete inf;
                            break;
                        }
                    case SummonCardType::WIZARDRY:
                        // summonCardsWizardry.pop();
                        break;
                    }
                    summonChooserOpen = false;
                    resetClickSlots();
                }
            }
        }
        ClickSlot *cs = clickSlots[last_ind];
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            CheckCollisionPointRec(GetMousePosition(), cs->rec)) {
            summonChooserOpen = false;
            resetClickSlots();
        }
    }
}

void SummonManager::draw() {
    if (summonChooserOpen) {
        float last_start_x;
        last_ind = 0;
        for (auto &cs : clickSlots) {
            if (cs->isActive) {
                DrawRectangleRounded(cs->rec, .5, 50, PURPLE);
                DrawRectangleRoundedLinesEx(cs->rec, .5, 50, 2, BLACK);
                std::string sprite_id;
                int count;
                switch (cs->type) {
                case SummonCardType::INFANTRY:
                    sprite_id = INFANTRY_ICON;
                    count = summonCardsInfantry.size();
                    break;
                case SummonCardType::ARCHERY:
                    sprite_id = ARCHERY_ICON;
                    count = summonCardsArchery.size();
                    break;
                case SummonCardType::WIZARDRY:
                    sprite_id = WIZARDRY_ICON;
                    count = summonCardsWizardry.size();
                    break;
                }
                getSpriteHolder()->drawSprite(sprite_id, cs->rec, {0, 0}, 0);
                DrawCircle(cs->start_x + summon_width, summon_box_y,
                           summon_width / 4, WHITE);
                float w = MeasureText(TextFormat("%d", count), 20);
                DrawText(TextFormat("%d", count),
                         cs->start_x + summon_width - w / 2, summon_box_y - 10,
                         20, BLACK);
                last_ind++;
            }
        }
        ClickSlot *cs = clickSlots[last_ind];
        getSpriteHolder()->drawSprite(UI_ICONS, 879, cs->rec, {0, 0}, 0);
    } else {

        DrawTextureRec(summon_box.texture, {0, 0, summon_width, -summon_box_h},
                       {-summon_width * .5f, summon_box_y}, WHITE);
        if (totalCards > 0) {
            DrawCircle(summon_width * .5f, summon_box_y, summon_width / 4,
                       WHITE);
            float w = MeasureText(TextFormat("%d", totalCards), 20);
            DrawText(TextFormat("%d", totalCards), summon_width * .5f - w / 2,
                     summon_box_y - 10, 20, BLACK);
        }
    }
}

void SummonManager::addSummonCard(std::shared_ptr<SummonCard> card) {
    switch (card->summon_type) {
    case SummonCardType::INFANTRY:
        summonCardsInfantry.push(card);
        break;
    case SummonCardType::ARCHERY:
        summonCardsArchery.push(card);
        break;
    case SummonCardType::WIZARDRY:
        summonCardsWizardry.push(card);
        break;
    }
}
