#include "house_block.hpp"
#include "constants.h"
#include "property_type.hpp"
#include "utils.h"
#include "property_type.hpp"
#include "globals.h"

HouseBlock::HouseBlock(float x, float y, int level)
    : Building(x, y, PropertyType::HOUSE, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::HOUSE);
    health = maxHealth;
}

void HouseBlock::init() {
    initiate();
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
}

void HouseBlock::repair(float repairAmount) {
    health += repairAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void HouseBlock::draw() {
    // DrawConvexPolygon(auraPoints, DrawMode::FILL,
    // {SKYBLUE.r, SKYBLUE.g, SKYBLUE.b, 60});
    // need to draw 4 houses around the grid
    float offset = CASTLE_WIDTH * perc_cover / 2.0f;
    float pc = perc_cover / 2.3;
    getSpriteHolder()->drawSprite(buildingData, x, y - offset / 2.0f, pc,
                                  buildingAlpha);
    getSpriteHolder()->drawSprite(buildingData, x + offset, y, pc,
                                  buildingAlpha);
    getSpriteHolder()->drawSprite(buildingData, x - offset, y, pc,
                                  buildingAlpha);
    getSpriteHolder()->drawSprite(buildingData, x, y + offset / 2.0f, pc,
                                  buildingAlpha);
    drawUpgradeView(BLUE);
}

bool HouseBlock::isAlive() {
    return true;
}

void HouseBlock::update(float dt) {
    timer->update(dt);
    awakenColliders(dt);
}

void HouseBlock::cleanupData() {
    colliderCleanup();
}

void HouseBlock::die() {
    alive = false;
    tributeGenerated = 0;
}

HouseBlock::~HouseBlock() {
}

void HouseBlock::onUpgrade(int level) {
}
