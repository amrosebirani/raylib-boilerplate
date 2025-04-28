#include "property.hpp"
#include "archery.hpp"
#include "attack_tower.hpp"
#include "barrack.hpp"
#include "constants.h"
#include "house_block.hpp"
#include "lightning_tower.hpp"
#include "raylib.h"
#include "temp_chance_list.hpp"
#include "utils.h"
#include <vector>
#include "globals.h"
#include "wizardry.hpp"

std::vector<TempChanceList<PropertyType> *> propertyRingChanceList = {
    new TempChanceList<PropertyType>({{PropertyType::HOUSE, 2},
                                      {PropertyType::BARRACKS, 1},
                                      {PropertyType::ARCHERY, 1}}),
    new TempChanceList<PropertyType>({{PropertyType::HOUSE, 3},
                                      {PropertyType::BARRACKS, 2},
                                      {PropertyType::WIZARDRY, 1},
                                      {PropertyType::ARCHERY, 2}}),
    new TempChanceList<PropertyType>({{PropertyType::LIGHTNING_TOWER, 6}}),
};

std::vector<PropertySlot *> getPropertiesForRing(int ring_no, float x, float y,
                                                 PropertyRing *ring) {
    float diagonal_offset = CASTLE_WIDTH / 2.0f + (ring_no + 1) * CASTLE_WIDTH -
                            CASTLE_WIDTH / 2.0f;
    std::vector<PropertySlot *> properties;
    float y_offset = y - diagonal_offset;
    float x_offset = x - diagonal_offset;
    for (int i = 0; i < (ring_no + 1) * 2 + 1; i++) {
        PropertyType type;
        if (i % 2 == 1) {
            type = PropertyType::DEFENSE_TOWER;
        } else {
            type = propertyRingChanceList[ring_no]->next();
        }
        properties.push_back(new PropertySlot(x_offset + i * CASTLE_WIDTH,
                                              y_offset, CASTLE_WIDTH,
                                              CASTLE_WIDTH, type, ring));
    }
    x_offset = x + diagonal_offset;
    y_offset = y - diagonal_offset;
    for (int i = 1; i < (ring_no + 1) * 2; i++) {
        PropertyType type;
        if (i % 2 == 1) {
            type = PropertyType::DEFENSE_TOWER;
        } else {
            type = propertyRingChanceList[ring_no]->next();
        }
        properties.push_back(
            new PropertySlot(x_offset, y_offset + i * CASTLE_WIDTH,
                             CASTLE_WIDTH, CASTLE_WIDTH, type, ring));
    }
    x_offset = x + diagonal_offset;
    y_offset = y + diagonal_offset;
    for (int i = 0; i < (ring_no + 1) * 2 + 1; i++) {
        PropertyType type;
        if (i % 2 == 1) {
            type = PropertyType::DEFENSE_TOWER;
        } else {
            type = propertyRingChanceList[ring_no]->next();
        }
        properties.push_back(new PropertySlot(x_offset - i * CASTLE_WIDTH,
                                              y_offset, CASTLE_WIDTH,
                                              CASTLE_WIDTH, type, ring));
    }
    x_offset = x - diagonal_offset;
    y_offset = y + diagonal_offset;
    for (int i = 1; i < (ring_no + 1) * 2; i++) {
        PropertyType type;
        if (i % 2 == 1) {
            type = PropertyType::DEFENSE_TOWER;
        } else {
            type = propertyRingChanceList[ring_no]->next();
        }
        properties.push_back(
            new PropertySlot(x_offset, y_offset - i * CASTLE_WIDTH,
                             CASTLE_WIDTH, CASTLE_WIDTH, type, ring));
    }
    return properties;
}

void PropertySlot::update(float dt) {
    // will add functionality later for updating according to type
    timer.update(dt);
    // if (building != nullptr) {
    // building->update(dt);
    // }
}

std::shared_ptr<Building> PropertySlot::getBuilding() {
    return building;
}

PropertySlot::PropertySlot(float x, float y, float width, float height,
                           PropertyType type, PropertyRing *ring)
    : x(x), y(y), width(width), height(height), type(type), ring(ring) {
    Vector2 ss = getWorldIsometricCoordinated(Vector2{x, y});
    if (type == PropertyType::HOUSE) {
        building = std::make_shared<HouseBlock>(ss.x, ss.y, 0);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::BARRACKS) {
        building = std::make_shared<Barrack>(ss.x, ss.y, 0);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::ARCHERY) {
        building = std::make_shared<Archery>(ss.x, ss.y, 0);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::DEFENSE_TOWER) {
        building = std::make_shared<AttackTower>(ss.x, ss.y, 0);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::WIZARDRY) {
        building = std::make_shared<Wizardry>(ss.x, ss.y, 0);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::LIGHTNING_TOWER) {
        building = std::make_shared<LightningTower>(ss.x, ss.y, 0);
        building->init();
        getContainer()->addGameObject(building);
    }
}

void PropertySlot::propertySlotObjectSave(std::ofstream &out) const {
    out.write(reinterpret_cast<const char *>(&x), sizeof(x));
    out.write(reinterpret_cast<const char *>(&y), sizeof(y));
    out.write(reinterpret_cast<const char *>(&width), sizeof(width));
    out.write(reinterpret_cast<const char *>(&height), sizeof(height));
    out.write(reinterpret_cast<const char *>(&type), sizeof(type));
    building->buildingObjectSave(out);
}

PropertySlot::PropertySlot(std::ifstream &in, PropertyRing *ring) : ring(ring) {
    in.read(reinterpret_cast<char *>(&x), sizeof(x));
    in.read(reinterpret_cast<char *>(&y), sizeof(y));
    in.read(reinterpret_cast<char *>(&width), sizeof(width));
    in.read(reinterpret_cast<char *>(&height), sizeof(height));
    in.read(reinterpret_cast<char *>(&type), sizeof(type));
    if (type == PropertyType::HOUSE) {
        building = std::make_shared<HouseBlock>(in);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::BARRACKS) {
        building = std::make_shared<Barrack>(in);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::ARCHERY) {
        building = std::make_shared<Archery>(in);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::DEFENSE_TOWER) {
        building = std::make_shared<AttackTower>(in);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::WIZARDRY) {
        building = std::make_shared<Wizardry>(in);
        building->init();
        getContainer()->addGameObject(building);
    } else if (type == PropertyType::LIGHTNING_TOWER) {
        building = std::make_shared<LightningTower>(in);
        building->init();
        getContainer()->addGameObject(building);
    }
}

void PropertySlot::draw() {
    // this is purely for debug purposes as of now
    // need to draw the isometric polygon over here
    // std::vector<Vector2> points;
    // points.push_back(
    //     getWorldIsometricCoordinated(Vector2{x + width / 2, y + height /
    //     2}));
    // points.push_back(
    //     getWorldIsometricCoordinated(Vector2{x + width / 2, y - height /
    //     2}));
    // points.push_back(
    //     getWorldIsometricCoordinated(Vector2{x - width / 2, y - height /
    //     2}));
    // points.push_back(
    //     getWorldIsometricCoordinated(Vector2{x - width / 2, y + height /
    //     2}));
    // DrawConvexPolygon(points, DrawMode::LINE, WHITE);
    // if (building != nullptr) {
    // building->draw();
    // }
}

void PropertyRing::update(float dt) {
    for (auto &property : properties) {
        property->update(dt);
    }
}

void PropertyRing::draw() {
    for (auto &property : properties) {
        property->draw();
    }
}

int PropertyRing::getRingNo() {
    return ring_no;
}

PropertyRing::~PropertyRing() {
    for (auto &property : properties) {
        delete property;
    }
}

PropertyRing::PropertyRing(int ring_no, Vector2 center, GameObject *region)
    : ring_no(ring_no), region(region) {
    properties = getPropertiesForRing(ring_no, center.x, center.y, this);
}

void PropertyRing::propertyRingObjectSave(std::ofstream &out) const {
    out.write(reinterpret_cast<const char *>(&ring_no), sizeof(ring_no));
    out.write(reinterpret_cast<const char *>(&operational),
              sizeof(operational));
    int property_size = properties.size();
    out.write(reinterpret_cast<const char *>(&property_size),
              sizeof(property_size));
    for (auto &property : properties) {
        property->propertySlotObjectSave(out);
    }
}

PropertyRing::PropertyRing(std::ifstream &in, GameObject *region)
    : region(region) {
    in.read(reinterpret_cast<char *>(&ring_no), sizeof(ring_no));
    in.read(reinterpret_cast<char *>(&operational), sizeof(operational));
}

void PropertyRing::initRing(std::ifstream &in) {
    int property_size;
    in.read(reinterpret_cast<char *>(&property_size), sizeof(property_size));
    for (int i = 0; i < property_size; i++) {
        properties.push_back(new PropertySlot(in, this));
    }
}
