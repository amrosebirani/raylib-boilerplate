#include "summon_manager.hpp"
#include "archery_summon.hpp"
#include "firebase.hpp"
#include "platform.hpp"
#include "constants.h"
#include "globals.h"
#include "infantry_summon.hpp"
#include "magic_types.hpp"
#include "property_type.hpp"
#include "raylib.h"
#include "summon_card.hpp"
#include "utils.h"
#include "warrior_types.h"
#include "wizardry_summon.hpp"
#include <algorithm>
#include <iostream>

using std::string;

SummonManager::SummonManager() {
    float sh = GetScreenHeight();
    summonCircleY = sh - sh / 5;
    summonBoxY = sh;
    tweenVals = std::make_shared<std::unordered_map<string, float>>();
    (*tweenVals)["scy"] = 0;
    (*tweenVals)["sby"] = 0;
    summonCards.push_back(std::make_shared<SummonCard>(
        1, WarriorType::WARRIOR_TYPE_SPEARMAN, SummonCardType::INFANTRY));
    summonCards.push_back(std::make_shared<SummonCard>(
        1, WarriorType::WARRIOR_TYPE_SHIELD_BEARER, SummonCardType::INFANTRY));
    // summonCards.push_back(std::make_shared<SummonCard>(
    //     2, WarriorType::WARRIOR_TYPE_BERSERKER, SummonCardType::INFANTRY));
    // summonCards.push_back(std::make_shared<SummonCard>(
    //     2, WarriorType::WARRIOR_TYPE_JAVELINER, SummonCardType::ARCHERY));
    summonCards.push_back(std::make_shared<SummonCard>(
        1, WarriorType::WARRIOR_TYPE_ARCHER, SummonCardType::ARCHERY));
    // summonCards.push_back(std::make_shared<SummonCard>(
    //     2, WarriorType::WARRIOR_TYPE_AXEMAN, SummonCardType::ARCHERY));
    summonCards.push_back(std::make_shared<SummonCard>(
        1, WarriorType::WARRIOR_TYPE_CROSSBOWMAN, SummonCardType::ARCHERY));
    summonCards.push_back(
        std::make_shared<SummonCard>(1, MagicType::LIGHTNING_SPELL));
    setEndX();
    setCircleRects();
}

void SummonManager::setCircleRects() {
    if (!isPlatformAndroid()) return;
    getJoystick()->clearProhibitedRects();
    float sh = GetScreenHeight();
    float sw = GetScreenWidth();
    getJoystick()->setProhibitedRects(
        {{sw / 2 - sh / 10, sh - sh / 5, sh / 5, sh / 5}});
}

void SummonManager::setBoxRects() {
    if (!isPlatformAndroid()) return;
    getJoystick()->clearProhibitedRects();
    float sh = GetScreenHeight();
    float sw = GetScreenWidth();
    getJoystick()->setProhibitedRects({{
        0,
        sh - sh / 5,
        sw,
        sh / 5,
    }});
}

SummonManager::~SummonManager() {
}

void SummonManager::setEndX() {

    end_x = 15;
    end_x += 15 * summonCards.size();
    end_x += (GetScreenHeight() / 5.0f - 20) * summonCards.size();
}

void SummonManager::handleDesktopInput() {

    float sh = GetScreenHeight();
    if (!mousePressed && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(
                mousePos, {0, summonBoxY, GetScreenWidth() * 1.0f, sh / 5})) {
            mousePressed = true;
            startPos = mousePos;
            currentPos = mousePos;
            isDragging = false;
            closeTimerCounter = 0;
        }
    }
    if (mousePressed && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        currentPos = GetMousePosition();
        float swiped = startPos.x - currentPos.x;
        if (abs(swiped) > 0) {
            isDragging = true;
            closeTimerCounter = 0;
        }
        scrollIfNeeded(swiped);
        startPos = currentPos;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && mousePressed) {
        if (isDragging) {

            // do nothing
        } else {
            // handle click here
            handleClick(GetMousePosition());
            std::cout << "clicked" << std::endl;
        }

        mousePressed = false;
        isDragging = false;
    }
}

void SummonManager::handleMobileInput() {
    if (getJoystick()->isActive()) {
        return;
    }
    handleDesktopInput();
}

void SummonManager::update(float dt) {
    timer.update(dt);
    if (!enabled) {
        if (isOpen) {
            isOpen = false;
            transitToClose();
        }
        return;
    }
    if (isOpen) {
        closeTimerCounter += dt;
        if (closeTimerCounter > closeTimer) {
            isOpen = false;
            transitToClose();
            closeTimerCounter = 0;
        }
    }
    if (end_x - start_x < GetScreenWidth()) {
        shift_x = (GetScreenWidth() - (end_x - start_x)) / 2;
    } else {
        shift_x = 0;
    }
    float sh = GetScreenHeight();
    summonCircleY = sh - sh / 5 + sh / 5 * tweenVals->at("scy");
    summonBoxY = sh - sh / 5 * tweenVals->at("sby");
    if (!isOpen) {
        if (isPlatformAndroid()) {
            if (getJoystick()->isActive()) {
                return;
            }
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointCircle(
                    mousePos, {GetScreenWidth() / 2.0f, sh - sh / 10},
                    sh / 10)) {
                openPressed = true;
            }
        }
        if (IsMouseButtonReleased(
                MOUSE_LEFT_BUTTON)) { // && summonCards.size() > 0) {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointCircle(
                    mousePos, {GetScreenWidth() / 2.0f, sh - sh / 10},
                    sh / 10) &&
                openPressed) {
                isOpen = true;
                transitToOpen();
                // timer.after(
                // 10,
                // [this](float dt) {
                // isOpen = false;
                // transitToClose();
                // },
                // "auto_close");
            }
            openPressed = false;
        }
    }
    if (isOpen) {
        if (isPlatformWeb() || isPlatformDesktop()) {
            handleDesktopInput();
        } else {
            handleMobileInput();
        }
    }
}

void SummonManager::scrollIfNeeded(float swiped) {
    float act_start_x = start_x + trans_x;
    float act_end_x = end_x + trans_x;
    if (swiped > 0 && act_end_x > GetScreenWidth()) {
        swiped = std::min(swiped, act_end_x - GetScreenWidth());
        trans_x -= swiped;
    }
    if (swiped < 0 && act_start_x < 0) {
        swiped = -swiped;
        swiped = std::min(swiped, -act_start_x);
        trans_x += swiped;
    }
}

void SummonManager::draw() {
    if (!enabled) {
        return;
    }
    drawSummonCircle();
    drawSummonBox();
    drawSummonCards();
}

void SummonManager::addSummonCard(std::shared_ptr<SummonCard> card) {
    summonCards.push_back(card);
    setEndX();
}

void SummonManager::drawSummonCircle() {
    if (summonCards.size() == 0) {
        return;
    }
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    getSpriteHolder()->drawSprite(
        SUMMON_BUTTON, {sw / 2 - sh / 10, summonCircleY, sh / 5, sh / 5});
    float st_fs = sh / 30;
    DrawText("Summon", sw / 2 - MeasureText("Summon", sh / 30) / 2.0f,
             summonCircleY + sh / 10 - sh / 60, sh / 30, WHITE);
}

void SummonManager::drawSummonBox() {
    if (!isOpen) {
        return;
    }
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    DrawRectangleRec({0, summonBoxY, sw, sh / 5}, {0, 0, 0, 200});
    if (end_x - start_x > sw) {
        // draw a scroll bar
        DrawRectangleRec({0, summonBoxY - 20, sw, 20}, {0, 0, 0, 220});
        float scroll_width = sw * sw / (end_x - start_x);
        float scroll_start = (-trans_x) * sw / (end_x - start_x);
        DrawRectangleRec({scroll_start, summonBoxY - 20, scroll_width, 20},
                         {255, 255, 255, 200});
    }
}

void SummonManager::handleClick(Vector2 pos) {

    if (!isOpen) {
        return;
    }
    float init_x;
    if (shift_x > 0) {
        init_x = shift_x;
    } else {
        init_x = 15 + trans_x;
    }
    float sh = GetScreenHeight();
    for (auto &card : summonCards) {
        Rectangle rect = {init_x, summonBoxY + 10, sh / 5 - 20, sh / 5 - 20};
        if (CheckCollisionPointRec(pos, rect)) {
            // do something
            Vector2 pos = getContainer()->getFormPos();
            if (card->summon_type == SummonCardType::INFANTRY) {
                InfantrySummon *is = new InfantrySummon(pos, card);
                delete is;
            } else if (card->summon_type == SummonCardType::ARCHERY) {
                ArcherySummon *as = new ArcherySummon(pos, card);
                delete as;
            } else if (card->summon_type == SummonCardType::WIZARDRY) {
                WizardrySummon *ws = new WizardrySummon(pos, card);
                delete ws;
            }
            summonCards.erase(
                std::remove(summonCards.begin(), summonCards.end(), card),
                summonCards.end());
            setEndX();
            isOpen = false;
            transitToClose();
            break;
            // remove this card
            std::cout << getWarriorText(card->type) << std::endl;
        }
        init_x += 15;
        init_x += sh / 5 - 20;
    }
}

void SummonManager::drawSummonCards() {
    float init_x;
    if (shift_x > 0) {
        init_x = shift_x;
    } else {
        init_x = 15 + trans_x;
    }
    float sh = GetScreenHeight();
    for (auto &card : summonCards) {
        Rectangle rect = {init_x, summonBoxY + 10, sh / 5 - 20, sh / 5 - 20};
        if (card->summon_type == SummonCardType::INFANTRY ||
            card->summon_type == SummonCardType::ARCHERY) {
            WarriorType wt = card->type;
            draw_warrior_icon(wt, rect);
            std::string wtext = getWarriorText(wt);
            DrawText(wtext.c_str(),
                     rect.x + rect.width / 2 -
                         MeasureText(wtext.c_str(), 10) / 2.0f,
                     rect.y + rect.height, 10, WHITE);
            Vector2 dim = getBuildingSummonDim(card->summon_type ==
                                                       SummonCardType::INFANTRY
                                                   ? PropertyType::BARRACKS
                                                   : PropertyType::ARCHERY,
                                               card->level);
            std::string summon_text =
                TextFormat("%d x %d", (int)dim.x, (int)dim.y);
            DrawText(summon_text.c_str(),
                     rect.x + rect.width / 2 -
                         MeasureText(summon_text.c_str(), 10) / 2.0f,
                     rect.y + rect.height - 25, 10, WHITE);
        } else if (card->summon_type == SummonCardType::WIZARDRY) {
            MagicType mt = card->magic_type;
            draw_magic_icon(mt, rect);
            std::string mtext = getMagicText(mt);
            DrawText(mtext.c_str(),
                     rect.x + rect.width / 2 -
                         MeasureText(mtext.c_str(), 10) / 2.0f,
                     rect.y + rect.height, 10, WHITE);
        }
        getSpriteHolder()->drawSprite(SUMMON_CARD_FRAME, rect);
        init_x += 15;
        init_x += sh / 5 - 20;
    }
}

void SummonManager::transitToOpen() {
    timer.tween(.3, tweenVals, {{"scy", 1.0f}, {"sby", 1.0f}}, "in-out-cubic",
                []() {}, "", {});
    trans_x = 0;
    closeTimerCounter = 0;
    setBoxRects();
}

void SummonManager::transitToClose() {

    timer.tween(.3, tweenVals, {{"scy", 0.0f}, {"sby", 0.0f}}, "in-out-cubic",
                []() {}, "", {});
    setCircleRects();
}
