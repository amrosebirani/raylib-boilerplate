
#include "archery.hpp"
#include "globals.h"
#include "property_type.hpp"
#include "raylib.h"
#include "summon_card.hpp"
#include "summon_dialog.hpp"
#include "utils.h"

Archery::Archery(float x, float y, int level)
    : Building(x, y, PropertyType::ARCHERY, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::ARCHERY);
    health = maxHealth;
}

bool Archery::hasSummonCards() {
    return false;
}

std::shared_ptr<SummonCard> Archery::getCard() {
    // if (producedSummonCards.size() > 0) {
    //     std::shared_ptr<SummonCard> sc = producedSummonCards.front();
    //     producedSummonCards.pop();
    //     return sc;
    // }
    return nullptr;
}

void Archery::init() {
    initiate();
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
    summonSlots = std::make_shared<std::vector<std::shared_ptr<SummonSlot>>>();
    //     summonTime = getBuildingProduceTime(PropertyType::ARCHERY, level);
    //     summonDim = getBuildingSummonDim(PropertyType::ARCHERY, level);
    //     summonTypes = getSummonChoices(PropertyType::ARCHERY, level);
    //     summonTimeCounter = 0;
    //     summonCardY = upgradePoint.y - ucr - 40 - 2 * ucr;
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
    drawSummonView();
    // if (producedSummonCards.size() > 0) {
    //
    //     int to_draw = 3;
    //     if (producedSummonCards.size() < 3) {
    //         to_draw = producedSummonCards.size();
    //     }
    //
    //     for (int i = 0; i < to_draw; i++) {
    //         DrawRectangleRounded({upgradePoint.x - ucr / 2 + i * 3,
    //                               summonCardY + 2.5f, ucr, 2 * ucr},
    //                              .5, 50, PURPLE);
    //         DrawRectangleRoundedLinesEx({upgradePoint.x - ucr / 2 + i * 3,
    //                                      summonCardY + 2.5f, ucr, 2 * ucr},
    //                                     .5, 50, 2, BLACK);
    //         // draw infantry icon here
    //         getSpriteHolder()->drawSprite(ARCHERY_ICON,
    //                                       {upgradePoint.x - ucr / 2 + i * 3,
    //                                        summonCardY + 2.5f, ucr, 2 * ucr},
    //                                       {0, 0}, 0);
    //     }
    //     // const char *tt = TextFormat("%d", producedSummonCards.size());
    //
    //     // float ttm = MeasureText(tt, 10);
    //     // DrawText(tt, upgradePoint.x - ttm / 2, summonCardY + 2.5f + ucr -
    //     5,
    //     // 10, WHITE);
    // }
}

bool Archery::isAlive() {
    return true;
}

void Archery::update(float dt) {
    timer->update(dt);
    awakenColliders(dt);
    // if (hasSummonCards()) {
    //     if (inContact) {
    //         summonExchTimer += dt;
    //         if (summonExchTimer > summonExchT) {
    //             int cc = producedSummonCards.size();
    //             if (cc > 3) cc = 3;
    //             cc -= 1;
    //             std::shared_ptr<SummonGain> sg =
    //             std::make_shared<SummonGain>(
    //                 Vector2{upgradePoint.x - ucr / 2 + cc * 3,
    //                         summonCardY + 2.5f},
    //                 Vector2{ucr, 2 * ucr}, getCard());
    //             getStateStack()->push(sg);
    //             summonExchTimer = 0;
    //         }
    //     }
    // }
    // if (level > 0) {
    //     summonTimeCounter += dt;
    //     if (summonTimeCounter > summonTime) {
    //         summonTimeCounter -= summonTime;
    //         std::shared_ptr<SummonCard> sc = std::make_shared<SummonCard>(
    //             summonDim.x, summonDim.y, summonTypes,
    //             SummonCardType::ARCHERY);
    //         producedSummonCards.push(sc);
    //     }
    // }
    for (auto &ss : *summonSlots) {
        ss->update(dt);
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
    // summonTime = getBuildingProduceTime(PropertyType::ARCHERY, level);
    // summonDim = getBuildingSummonDim(PropertyType::ARCHERY, level);
    // summonTypes = getSummonChoices(PropertyType::ARCHERY, level);
    // summonCardY = upgradePoint.y - ucr - 40 - 2 * ucr;
    // if (summonTimeCounter > summonTime) {
    //     summonTimeCounter = 0;
    //     std::shared_ptr<SummonCard> sc = std::make_shared<SummonCard>(
    //         summonDim.x, summonDim.y, summonTypes, SummonCardType::ARCHERY);
    //     producedSummonCards.push(sc);
    // }
    getWorldState()->addScore(600);
    if (level == 0) {
        summonSlots->clear();
    } else {
        if (summonSlots->size() < level) {
            for (int i = summonSlots->size(); i < level; i++) {
                summonSlots->push_back(std::make_shared<SummonSlot>(
                    summonSlots->size() + 1, level, PropertyType::ARCHERY,
                    SummonCardType::ARCHERY));
            }
        }
        for (auto &ss : *summonSlots) {
            ss->setLevel(level);
        }
    }
}

void Archery::pushSummonDialog() {
    if (level == 0) {
        return;
    }
    getStateStack()->push(std::make_shared<SummonDialog>(
        PropertyType::ARCHERY, level, summonSlots,
        getBuildingData(getBuildingId(PropertyType::ARCHERY, level))));
}
