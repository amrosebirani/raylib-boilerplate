// what's the point of this?
// region contains the information about the economy around the
// castle/stronghold
//

#pragma once

#include "castle.hpp"
#include "game_object.h"
#include "property.hpp"

class Region : public GameObject {
    public:
        Region(float x, float y, float width, float height, float gold,
               float wood, float stone, float food);
        ~Region();
        void draw() override;
        void update(float dt) override;
        bool isAlive() override;
        void die() override;
        Vector2 getCenterCoordinates();

        void init() override;
        void cleanupData() override;
        float getGold();
        float getWood();
        float getStone();
        float getFood();

        void setGold(float gold);
        void setWood(float wood);
        void setStone(float stone);
        void setFood(float food);

        void addGold(float gold);
        void addWood(float wood);
        void addStone(float stone);
        void addFood(float food);

        float getWidth();
        float getHeight();
        void setCurrentHeightAndWidth(float percentHealth);
        void addPropertyRing(int ring_no);
        std::shared_ptr<Castle> castle;
        std::vector<Vector2> getRegionPoints();
        std::vector<Vector2> getRegionPoints(float random_angle);

    private:
        float width;
        float height;
        float gold;
        float wood;
        float stone;
        float food;
        bool alive = true;
        float current_width;
        float current_height;
        std::vector<PropertyRing *> propertyRings;
        int level = 1;
        float health = 0;
};
