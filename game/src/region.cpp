#include "region.hpp"
#include "constants.h"
#include "raylib.h"

void Region::draw() {
    // draw the Region
    for (auto &propertyRing : propertyRings) {
        propertyRing->draw();
    }

    // region highlight
    DrawRectangle(x - current_width / 2, y - current_height / 2, current_width,
                  current_height, {BLUE.r, BLUE.g, BLUE.b, 40});
    // above rectangle is just for debug purposes
}

void Region::update(float dt) {
    // update all the properties
    for (auto &propertyRing : propertyRings) {
        propertyRing->update(dt);
    }
}

bool Region::isAlive() {
    return alive;
}

void Region::die() {
    alive = false;
}

void Region::init() {
}

void Region::cleanupData() {
}

float Region::getGold() {
    return gold;
}

float Region::getWood() {
    return wood;
}

float Region::getStone() {
    return stone;
}

float Region::getFood() {
    return food;
}

void Region::setGold(float gold) {
    this->gold = gold;
}

void Region::setWood(float wood) {
    this->wood = wood;
}

void Region::setStone(float stone) {
    this->stone = stone;
}

void Region::setFood(float food) {
    this->food = food;
}

void Region::addGold(float gold) {
    this->gold += gold;
}

void Region::addWood(float wood) {
    this->wood += wood;
}

void Region::addStone(float stone) {
    this->stone += stone;
}

void Region::addFood(float food) {
    this->food += food;
}

float Region::getWidth() {
    return current_width;
}

float Region::getHeight() {
    return current_height;
}

Region::Region(float x, float y, float width, float height, float gold,
               float wood, float stone, float food)
    : GameObject(x, y), width(width), height(height), gold(gold), wood(wood),
      stone(stone), food(food) {
    current_width = width;
    current_height = height;
    // to start with create just 2 property rings, that's the base castle
    // post that on upgrade more rings can be added
    for (int i = 0; i < DEFAULT_PROPERTY_RINGS; i++) {
        propertyRings.push_back(new PropertyRing(i, {x, y}, this));
    }
} // create the property rings
//

void Region::setCurrentHeightAndWidth(float percentHealth) {
    current_width = width * percentHealth;
    current_height = height * percentHealth;
    // set operational information for the property rings here
}
