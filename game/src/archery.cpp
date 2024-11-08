
#include "archery.hpp"
#include "constants.h"
#include "globals.h"
#include "property_type.hpp"
#include "raylib.h"
#include "summon_gain.hpp"
#include "utils.h"

Archery::Archery(float x, float y, int level)
    : Building(x, y, PropertyType::ARCHERY, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::ARCHERY);
    health = maxHealth;
}

bool Archery::hasSummonCards() {
    return producedSummonCards.size() > 0;
}

std::shared_ptr<SummonCard> Archery::getCard() {
    if (producedSummonCards.size() > 0) {
        std::shared_ptr<SummonCard> sc = producedSummonCards.front();
        producedSummonCards.pop();
        return sc;
    }
    return nullptr;
}

void Archery::init() {
    initiate();
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
    summonTime = getBuildingProduceTime(PropertyType::ARCHERY, level);
    summonDim = getBuildingSummonDim(PropertyType::ARCHERY, level);
    summonTypes = getSummonChoices(PropertyType::ARCHERY, level);
    summonTimeCounter = 0;
    summonCardY = upgradePoint.y - ucr - 40 - 2 * ucr;
}

void Archery::repair(float repairAmount) {
    health += repairAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Archery::draw() {
    // DrawConvexPolygon(auraPoints, DrawMode::FILL,
    // {ORANGE.r, ORANGE.g, ORANGE.b, 60});
    getSpriteHolder()->drawSprite(buildingData, x, y, perc_cover,
                                  buildingAlpha);
    drawUpgradeView(ORANGE);
    if (producedSummonCards.size() > 0) {

        int to_draw = 3;
        if (producedSummonCards.size() < 3) {
            to_draw = producedSummonCards.size();
        }

        for (int i = 0; i < to_draw; i++) {
            DrawRectangleRounded({upgradePoint.x - ucr / 2 + i * 3,
                                  summonCardY + 2.5f, ucr, 2 * ucr},
                                 .5, 50, PURPLE);
            DrawRectangleRoundedLinesEx({upgradePoint.x - ucr / 2 + i * 3,
                                         summonCardY + 2.5f, ucr, 2 * ucr},
                                        .5, 50, 2, BLACK);
            // draw infantry icon here
            getSpriteHolder()->drawSprite(ARCHERY_ICON,
                                          {upgradePoint.x - ucr / 2 + i * 3,
                                           summonCardY + 2.5f, ucr, 2 * ucr},
                                          {0, 0}, 0);
        }
        // const char *tt = TextFormat("%d", producedSummonCards.size());

        // float ttm = MeasureText(tt, 10);
        // DrawText(tt, upgradePoint.x - ttm / 2, summonCardY + 2.5f + ucr - 5,
        // 10, WHITE);
    }
}

bool Archery::isAlive() {
    return true;
}

void Archery::update(float dt) {
    timer.update(dt);
    awakenColliders(dt);
    if (hasSummonCards()) {
        if (inContact) {
            summonExchTimer += dt;
            if (summonExchTimer > summonExchT) {
                int cc = producedSummonCards.size();
                if (cc > 3) cc = 3;
                cc -= 1;
                std::shared_ptr<SummonGain> sg = std::make_shared<SummonGain>(
                    Vector2{upgradePoint.x - ucr / 2 + cc * 3,
                            summonCardY + 2.5f},
                    Vector2{ucr, 2 * ucr}, getCard());
                getStateStack()->push(sg);
                summonExchTimer = 0;
            }
        }
    }
    if (level > 0) {
        summonTimeCounter += dt;
        if (summonTimeCounter > summonTime) {
            summonTimeCounter -= summonTime;
            std::shared_ptr<SummonCard> sc = std::make_shared<SummonCard>(
                summonDim.x, summonDim.y, summonTypes);
            producedSummonCards.push(sc);
        }
    }
}

void Archery::cleanupData() {
    colliderCleanup();
}

void Archery::die() {
    alive = false;
    tributeGenerated = 0;
}

Archery::~Archery() {
}

void Archery::onUpgrade(int level) {
    summonTime = getBuildingProduceTime(PropertyType::ARCHERY, level);
    summonDim = getBuildingSummonDim(PropertyType::ARCHERY, level);
    summonTypes = getSummonChoices(PropertyType::ARCHERY, level);
    summonCardY = upgradePoint.y - ucr - 40 - 2 * ucr;
    if (summonTimeCounter > summonTime) {
        summonTimeCounter = 0;
        std::shared_ptr<SummonCard> sc =
            std::make_shared<SummonCard>(summonDim.x, summonDim.y, summonTypes);
        producedSummonCards.push(sc);
    }
}
