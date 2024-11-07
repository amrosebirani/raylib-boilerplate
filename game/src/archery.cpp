
#include "archery.hpp"
#include "globals.h"
#include "property_type.hpp"
#include "raylib.h"
#include "utils.h"

Archery::Archery(float x, float y, int level)
    : Building(x, y, PropertyType::ARCHERY, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::ARCHERY);
    health = maxHealth;
}

void Archery::init() {
    initiate();
    timer.every(
        1, [this](float dt) { this->tributeGenerated += .1; }, 0, []() {}, "");
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
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
}

bool Archery::isAlive() {
    return true;
}

void Archery::update(float dt) {
    timer.update(dt);
    awakenColliders(dt);
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
}
