#include "property.hpp"
#include "constants.h"
#include "raylib.h"
#include "region.hpp"
#include "chance_list.hpp"

#include <unordered_map>
#include <vector>
#include <string>

std::unordered_map<std::string, PropertyType> propertyTypeMap = {
    {"GOLD_MINE", PropertyType::GOLD_MINE},
    {"FOREST", PropertyType::FOREST},
    {"STONE_QUARRY", PropertyType::STONE_QUARRY},
    {"FARM", PropertyType::FARM},
    {"PASTURE", PropertyType::PASTURE},
    {"RESIDENTIAL", PropertyType::RESIDENTIAL},
    {"MILITARY", PropertyType::MILITARY},
    {"BARREN", PropertyType::BARREN}};

ChanceList *propertyChances = new ChanceList({{"GOLD_MINE", 1},
                                              {"FOREST", 2},
                                              {"STONE_QUARRY", 1},
                                              {"FARM", 3},
                                              {"RESIDENTIAL", 1},
                                              {"BARREN", 6}});

std::vector<Property *> getPropertiesForRing(int ring_no, float x, float y,
                                             PropertyRing *ring) {
    float diagonal_offset = CASTLE_WIDTH / 2.0f + (ring_no + 1) * ROAD_WIDTH +
                            (ring_no + 1) * PROPERTY_HEIGHT -
                            PROPERTY_HEIGHT / 2.0f;
    std::vector<Property *> properties;
    float y_offset = y - diagonal_offset;
    float x_offset = x - diagonal_offset;
    for (int i = 0; i < (ring_no + 1) * 2 + 2; i++) {
        PropertyType type = propertyTypeMap[propertyChances->next()];
        properties.push_back(
            new Property(x_offset + i * (PROPERTY_WIDTH + ROAD_WIDTH), y_offset,
                         PROPERTY_WIDTH, PROPERTY_HEIGHT, type, ring));
    }
    x_offset = x + diagonal_offset;
    y_offset = y - diagonal_offset + PROPERTY_HEIGHT + ROAD_WIDTH;
    for (int i = 0; i < (ring_no + 1) * 2; i++) {
        PropertyType type = propertyTypeMap[propertyChances->next()];
        properties.push_back(new Property(
            x_offset, y_offset + i * (PROPERTY_HEIGHT + ROAD_WIDTH),
            PROPERTY_WIDTH, PROPERTY_HEIGHT, type, ring));
    }
    x_offset = x + diagonal_offset;
    y_offset = y + diagonal_offset;
    for (int i = 0; i < (ring_no + 1) * 2 + 2; i++) {
        PropertyType type = propertyTypeMap[propertyChances->next()];
        properties.push_back(
            new Property(x_offset - i * (PROPERTY_WIDTH + ROAD_WIDTH), y_offset,
                         PROPERTY_WIDTH, PROPERTY_HEIGHT, type, ring));
    }
    x_offset = x - diagonal_offset;
    y_offset = y + diagonal_offset - PROPERTY_HEIGHT - ROAD_WIDTH;
    for (int i = 0; i < (ring_no + 1) * 2; i++) {
        PropertyType type = propertyTypeMap[propertyChances->next()];
        properties.push_back(new Property(
            x_offset, y_offset - i * (PROPERTY_HEIGHT + ROAD_WIDTH),
            PROPERTY_WIDTH, PROPERTY_HEIGHT, type, ring));
    }
    return properties;
}

void Property::update(float dt) {
    // will add functionality later for updating according to type
    timer.update(dt);
}

Property::Property(float x, float y, float width, float height,
                   PropertyType type, PropertyRing *ring)
    : x(x), y(y), width(width), height(height), type(type), ring(ring) {
    switch (type) {
    case PropertyType::GOLD_MINE:
        timer.every(
            BASE_GOLD_PRODUCTION_CYCLE,
            [this](float dt) {
                // add gold to region
                Region *region = dynamic_cast<Region *>(this->ring->region);
                region->addGold(BASE_GOLD_PRODUCTION);
            },
            0, []() {}, "");
        break;
    case PropertyType::FOREST:
        timer.every(
            BASE_WOOD_PRODUCTION_CYCLE,
            [this](float dt) {
                // add wood to region
                Region *region = dynamic_cast<Region *>(this->ring->region);
                region->addWood(BASE_WOOD_PRODUCTION);
            },
            0, []() {}, "");
    case PropertyType::STONE_QUARRY:
        timer.every(
            BASE_STONE_PRODUCTION_CYCLE,
            [this](float dt) {
                // add stone to region
                Region *region = dynamic_cast<Region *>(this->ring->region);
                region->addStone(BASE_STONE_PRODUCTION);
            },
            0, []() {}, "");
    case PropertyType::FARM:
        timer.every(
            BASE_FOOD_PRODUCTION_CYCLE,
            [this](float dt) {
                // add food to region
                Region *region = dynamic_cast<Region *>(this->ring->region);
                region->addFood(BASE_FOOD_PRODUCTION);
            },
            0, []() {}, "");
    default:
        break;
    }
}

void Property::draw() {
    // this is purely for debug purposes as of now
    Color color;
    switch (type) {
    case PropertyType::GOLD_MINE:
        color = GOLD;
        break;
    case PropertyType::FOREST:
        color = GREEN;
        break;
    case PropertyType::STONE_QUARRY:
        color = GRAY;
        break;
    case PropertyType::FARM:
        color = YELLOW;
        break;
    case PropertyType::PASTURE:
        color = BROWN;
        break;
    case PropertyType::RESIDENTIAL:
        color = BLUE;
        break;
    case PropertyType::MILITARY:
        color = RED;
        break;
    case PropertyType::BARREN:
        color = DARKGRAY;
        break;
    }
    DrawRectangle(x - width / 2, y - height / 2, width, height, color);
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
