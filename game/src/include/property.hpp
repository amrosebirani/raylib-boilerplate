#pragma once
#include <vector>
#include "property_type.hpp"
#include "building.hpp"
#include "raylib.h"
#include "game_object.h"
#include "timer.h"

class PropertyRing;

class PropertySlot {
    public:
        PropertySlot(float x, float y, float width, float height,
                     PropertyType type, PropertyRing *ring);
        PropertyType type;
        void update(float dt);
        void draw();
        std::shared_ptr<Building> getBuilding();

    private:
        PropertyRing *ring;
        float x;
        float y;
        float width;
        float height;
        Timer timer;
        std::shared_ptr<Building> building;
};

class PropertyRing {
    public:
        PropertyRing(int ring_no, Vector2 center, GameObject *region);
        ~PropertyRing();
        int getRingNo();
        void update(float dt);
        void draw();
        GameObject *region;
        std::vector<PropertySlot *> properties;

    private:
        int ring_no;
        bool operational = true;
};

std::vector<PropertySlot *> getPropertiesForRing(int ring_no, float x, float y,
                                                 PropertyRing *ring);
