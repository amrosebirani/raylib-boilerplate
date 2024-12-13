#include "region.hpp"
#include "constants.h"
#include "property_type.hpp"
#include "raylib.h"

#include "utils.h"
void Region::draw() {
    // draw the Region
    for (auto &propertyRing : propertyRings) {
        propertyRing->draw();
    }

    // region highlight
    // above rectangle is just for debug purposes
}

void Region::update(float dt) {
    // update all the properties
    for (auto &propertyRing : propertyRings) {
        propertyRing->update(dt);
    }
}

Region::~Region() {
    for (auto &propertyRing : propertyRings) {
        delete propertyRing;
    }
    castle = nullptr;
}

bool Region::isAlive() {
    return alive;
}

void Region::die() {
    alive = false;
}

void Region::init() {
    health = getMaxHealthByLevel(level, PropertyType::CASTLE);

    Vector2 ss = getWorldIsometricCoordinated(Vector2{x, y});
    castle = std::make_shared<Castle>(ss.x, ss.y, health, level);
    castle->init();
}

std::shared_ptr<GameObject> Region::getTutorialTower(PropertyType type) {
    if (type == PropertyType::DEFENSE_TOWER && towerRaised) {
        return nullptr;
    }
    if (type == PropertyType::BARRACKS && barracksRaised) {
        return nullptr;
    }
    if (type == PropertyType::ARCHERY && archeryRaised) {
        return nullptr;
    }
    if (type == PropertyType::HOUSE && houseRaised) {
        return nullptr;
    }
    PropertyRing *first = propertyRings[0];
    std::shared_ptr<GameObject> tower;
    for (auto &p : first->properties) {
        if (p->type == type) {
            tower = p->getBuilding();
            break;
        }
    }
    return tower;
}

std::vector<Vector2> Region::getRegionPoints() {
    std::vector<Vector2> retv;
    Vector2 ss = getWorldIsometricCoordinated(Vector2{x, y});
    retv.push_back(ss);
    float random_angle = randomFloatInRange(0, 2 * M_PI);
    float dd1 = (propertyRings.size() + 1) * CASTLE_WIDTH;
    Vector2 pp1 = {x + dd1 * std::cos(random_angle),
                   y + dd1 * std::sin(random_angle)};
    retv.push_back(getWorldIsometricCoordinated(pp1));
    float dd2 = (propertyRings.size() + 2.5) * CASTLE_WIDTH;
    Vector2 pp2 = {x + dd2 * std::cos(random_angle),
                   y + dd2 * std::sin(random_angle)};
    retv.push_back(getWorldIsometricCoordinated(pp2));
    float dd3 = (propertyRings.size() + 5.0) * CASTLE_WIDTH;
    Vector2 pp3 = {x + dd3 * std::cos(random_angle),
                   y + dd3 * std::sin(random_angle)};
    retv.push_back(getWorldIsometricCoordinated(pp3));
    return retv;
}

std::vector<Vector2> Region::getRegionPoints(float random_angle) {
    std::vector<Vector2> retv;
    Vector2 ss = getWorldIsometricCoordinated(Vector2{x, y});
    retv.push_back(ss);
    float dd1 = (propertyRings.size() + 1) * CASTLE_WIDTH;
    Vector2 pp1 = {x + dd1 * std::cos(random_angle),
                   y + dd1 * std::sin(random_angle)};
    retv.push_back(getWorldIsometricCoordinated(pp1));
    float dd2 = (propertyRings.size() + 2.5) * CASTLE_WIDTH;
    Vector2 pp2 = {x + dd2 * std::cos(random_angle),
                   y + dd2 * std::sin(random_angle)};
    retv.push_back(getWorldIsometricCoordinated(pp2));
    float dd3 = (propertyRings.size() + 5.0) * CASTLE_WIDTH;
    Vector2 pp3 = {x + dd3 * std::cos(random_angle),
                   y + dd3 * std::sin(random_angle)};
    retv.push_back(getWorldIsometricCoordinated(pp3));
    return retv;
}

Vector2 Region::getCenterCoordinates() {
    return getWorldIsometricCoordinated(Vector2{x, y});
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
    propertyRings.push_back(new PropertyRing(0, {x, y}, this));
} // create the property rings
//
//
void Region::addPropertyRing(int ring_no) {
    propertyRings.push_back(new PropertyRing(ring_no, {x, y}, this));
    castleUpgraded = true;
}

void Region::setCurrentHeightAndWidth(float percentHealth) {
    current_width = width * percentHealth;
    current_height = height * percentHealth;
    // set operational information for the property rings here
}
