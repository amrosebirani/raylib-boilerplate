#include "region.hpp"
#include "constants.h"
#include "game_object_types.h"
#include "property_type.hpp"
#include "raylib.h"

#include "utils.h"
#include <fstream>

GameObjectType Region::getObjectType() {
    return GameObjectType::NON_TYPE;
}

void Region::Save(std::ofstream &out) const {
    regionObjectSave(out);
}

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
    return true;
}

void Region::die() {
}

void Region::init() {
    float health = getMaxHealthByLevel(level, PropertyType::CASTLE);

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

float Region::getWidth() {
    return current_width;
}

float Region::getHeight() {
    return current_height;
}

Region::Region(float x, float y, float width, float height)
    : GameObject(x, y), current_width(width), current_height(height) {
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

void Region::regionObjectSave(std::ofstream &out) const {
    gameObjectSave(out);
    out.write(reinterpret_cast<const char *>(&current_height),
              sizeof(current_height));
    out.write(reinterpret_cast<const char *>(&current_width),
              sizeof(current_width));
    out.write(reinterpret_cast<const char *>(&level), sizeof(level));
    out.write(reinterpret_cast<const char *>(&towerRaised),
              sizeof(towerRaised));
    out.write(reinterpret_cast<const char *>(&barracksRaised),
              sizeof(barracksRaised));
    out.write(reinterpret_cast<const char *>(&archeryRaised),
              sizeof(archeryRaised));
    out.write(reinterpret_cast<const char *>(&houseRaised),
              sizeof(houseRaised));
    out.write(reinterpret_cast<const char *>(&castleUpgraded),
              sizeof(castleUpgraded));
    castle->buildingObjectSave(out);
    int prop_ring_size = propertyRings.size();
    out.write(reinterpret_cast<const char *>(&prop_ring_size),
              sizeof(prop_ring_size));
    for (auto &propertyRing : propertyRings) {
        propertyRing->propertyRingObjectSave(out);
    }
}

Region::Region(std::ifstream &in) : GameObject(in) {
    in.read(reinterpret_cast<char *>(&current_height), sizeof(current_height));
    in.read(reinterpret_cast<char *>(&current_width), sizeof(current_width));
    in.read(reinterpret_cast<char *>(&level), sizeof(level));
    in.read(reinterpret_cast<char *>(&towerRaised), sizeof(towerRaised));
    in.read(reinterpret_cast<char *>(&barracksRaised), sizeof(barracksRaised));
    in.read(reinterpret_cast<char *>(&archeryRaised), sizeof(archeryRaised));
    in.read(reinterpret_cast<char *>(&houseRaised), sizeof(houseRaised));
    in.read(reinterpret_cast<char *>(&castleUpgraded), sizeof(castleUpgraded));
}

void Region::initLoad(std::ifstream &in) {

    castle = std::make_shared<Castle>(in);
    castle->init();
    int prop_ring_size;
    in.read(reinterpret_cast<char *>(&prop_ring_size), sizeof(prop_ring_size));
    for (int i = 0; i < prop_ring_size; i++) {
        PropertyRing *propertyRing = new PropertyRing(in, this);
        propertyRing->initRing(in);
        propertyRings.push_back(propertyRing);
    }
}
