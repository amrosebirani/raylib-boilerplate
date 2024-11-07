#include "wizardry.hpp"
#include "globals.h"
#include "property_type.hpp"
#include "utils.h"

Wizardry::Wizardry(float x, float y, int level)
    : Building(x, y, PropertyType::WIZARDRY, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::WIZARDRY);
    health = maxHealth;
}

void Wizardry::init() {
    initiate();
    timer.every(
        1, [this](float dt) { this->tributeGenerated += .1; }, 0, []() {}, "");
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
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
}

bool Wizardry::isAlive() {
    return true;
}

void Wizardry::update(float dt) {
    timer.update(dt);
    awakenColliders(dt);
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
}
