#pragma once
#include <vector>
#include "raylib.h"
#include "game_object.h"
#include "timer.h"

enum PropertyType {
    GOLD_MINE,
    FOREST,
    STONE_QUARRY,
    FARM,
    PASTURE,
    RESIDENTIAL,
    MILITARY,
    BARREN
};

class PropertyRing;

class Property {
    public:
        Property(float x, float y, float width, float height, PropertyType type,
                 PropertyRing *ring);
        PropertyType type;
        void update(float dt);
        void draw();

    private:
        PropertyRing *ring;
        float x;
        float y;
        float width;
        float height;
        Timer timer;
};

class PropertyRing {
    public:
        PropertyRing(int ring_no, Vector2 center, GameObject *region);
        ~PropertyRing();
        int getRingNo();
        void update(float dt);
        void draw();
        GameObject *region;

    private:
        int ring_no;
        std::vector<Property *> properties;
        bool operational = true;
};

std::vector<Property *> getPropertiesForRing(int ring_no, float x, float y,
                                             PropertyRing *ring);
