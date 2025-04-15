#include "wizardry.hpp"
#include "globals.h"
#include "property_type.hpp"
#include "utils.h"

Wizardry::Wizardry(float x, float y, int level)
    : Building(x, y, PropertyType::WIZARDRY, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::WIZARDRY);
    health = maxHealth;
}

bool Wizardry::hasSummonCards() {
    return false;
}

void Wizardry::init() {
    initiate();
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
    summonSlots = std::make_shared<std::vector<std::shared_ptr<SummonSlot>>>();
}

std::shared_ptr<SummonCard> Wizardry::getCard() {
    return nullptr;
}

void Wizardry::repair(float repairAmount) {
    health += repairAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Wizardry::draw() {
    // DrawConvexPolygon(auraPoints, DrawMode::FILL, {RED.r, RED.g, RED.b, 60});
    getSpriteHolder()->drawSprite(buildingData, x, y, perc_cover,
                                  buildingAlpha);
    drawUpgradeView(RED);
    drawSummonView();
}

bool Wizardry::isAlive() {
    return true;
}

void Wizardry::update(float dt) {
    timer->update(dt);
    awakenColliders(dt);
    for (auto &slot : *summonSlots) {
        slot->update(dt);
    }
}

void Wizardry::cleanupData() {
    colliderCleanup();
}

void Wizardry::die() {
    alive = false;
    tributeGenerated = 0;
}

Wizardry::~Wizardry() {
}

void Wizardry::onUpgrade(int level) {
    getWorldState()->addScore(700);

    if (level == 0) {
        summonSlots->clear();
    } else {
        if (summonSlots->size() < level) {
            for (int i = summonSlots->size(); i < level; i++) {
                summonSlots->push_back(std::make_shared<SummonSlot>(
                    summonSlots->size() + 1, level, PropertyType::WIZARDRY,
                    SummonCardType::WIZARDRY));
            }
        }
        for (auto &ss : *summonSlots) {
            ss->setLevel(level);
        }
    }
}

void Wizardry::pushSummonDialog() {
    // std::shared_ptr<SummonDialog> sd = std::make_shared<SummonDialog>(
    //     x, y, PropertyType::WIZARDRY, level);
    // sd->init();
    // getDialogHolder()->pushDialog(sd);
}
