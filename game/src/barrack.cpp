#include "barrack.hpp"
#include "globals.h"
#include "property_type.hpp"
#include "raylib.h"
#include "utils.h"

Barrack::Barrack(float x, float y, int level)
    : Building(x, y, PropertyType::BARRACKS, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::BARRACKS);
    health = maxHealth;
}

bool Barrack::hasSummonCards() {
    return producedSummonCards.size() > 0;
}

std::shared_ptr<SummonCard> Barrack::getCard() {
    if (producedSummonCards.size() > 0) {
        std::shared_ptr<SummonCard> sc = producedSummonCards.front();
        producedSummonCards.pop();
        return sc;
    }
    return nullptr;
}

void Barrack::init() {
    initiate();
    timer.every(
        1, [this](float dt) { this->tributeGenerated += .1; }, 0, []() {}, "");
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
    summonTime = getBuildingProduceTime(PropertyType::BARRACKS, level);
    summonDim = getBuildingSummonDim(PropertyType::BARRACKS, level);
    summonTypes = getSummonChoices(PropertyType::BARRACKS, level);
    summonTimeCounter = 0;
    summonCardY = upgradePoint.y - ucr - 40 - 2 * ucr;
}

void Barrack::repair(float repairAmount) {
    health += repairAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Barrack::draw() {
    // DrawConvexPolygon(auraPoints, DrawMode::FILL, {RED.r, RED.g, RED.b, 60});
    getSpriteHolder()->drawSprite(buildingData, x, y, perc_cover,
                                  buildingAlpha);
    drawUpgradeView(RED);

    if (producedSummonCards.size() > 0) {

        DrawRectangleRounded(
            {upgradePoint.x - ucr / 2, summonCardY + 2.5f, ucr, 2 * ucr}, .5,
            50, PURPLE);
        const char *tt = TextFormat("%d", producedSummonCards.size());
        float ttm = MeasureText(tt, 10);
        DrawText(tt, upgradePoint.x - ttm / 2, summonCardY + 2.5f + ucr - 5, 10,
                 WHITE);
    }
}

bool Barrack::isAlive() {
    return true;
}

void Barrack::update(float dt) {
    timer.update(dt);
    awakenColliders(dt);
    if (hasSummonCards()) {
        if (inContact) {
            summonExchTimer += dt;
            if (summonExchTimer > summonExchT) {
                getContainer()->summon_manager->addSummonCard(getCard());
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

void Barrack::cleanupData() {
    colliderCleanup();
}

void Barrack::die() {
    alive = false;
    tributeGenerated = 0;
}

Barrack::~Barrack() {
}

void Barrack::onUpgrade(int level) {
    summonTime = getBuildingProduceTime(PropertyType::BARRACKS, level);
    summonDim = getBuildingSummonDim(PropertyType::BARRACKS, level);
    summonTypes = getSummonChoices(PropertyType::BARRACKS, level);
    summonCardY = upgradePoint.y - ucr - 40 - 2 * ucr;
    if (summonTimeCounter > summonTime) {
        summonTimeCounter = 0;
        std::shared_ptr<SummonCard> sc =
            std::make_shared<SummonCard>(summonDim.x, summonDim.y, summonTypes);
        producedSummonCards.push(sc);
    }
}
