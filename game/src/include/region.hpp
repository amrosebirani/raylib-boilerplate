// what's the point of this?
// region contains the information about the economy around the
// castle/stronghold
//

#pragma once

#include "castle.hpp"
#include "game_object.h"
#include "game_object_types.h"
#include "property.hpp"
#include "property_type.hpp"
#include <fstream>

class Region : GameObject {
    public:
        Region(float x, float y, float width, float height);
        Region(std::ifstream &in);
        ~Region();
        void draw() override;
        void update(float dt) override;
        bool isAlive() override;
        void die() override;

        void regionObjectSave(std::ofstream &out) const;
        Vector2 getCenterCoordinates();

        void init() override;
        void initLoad(std::ifstream &in);
        void cleanupData() override;
        GameObjectType getObjectType() override;
        void Save(std::ofstream &out) const override;

        float getWidth();
        float getHeight();
        void addPropertyRing(int ring_no);
        std::shared_ptr<Castle> castle;
        std::vector<Vector2> getRegionPoints();
        std::vector<Vector2> getRegionPoints(float random_angle);
        std::shared_ptr<GameObject>
        getTutorialTower(PropertyType type = PropertyType::DEFENSE_TOWER);
        bool towerRaised = false;
        bool barracksRaised = false;
        bool archeryRaised = false;
        bool houseRaised = false;
        bool castleUpgraded = false;

    private:
        float current_width;
        float current_height;
        std::vector<PropertyRing *> propertyRings;
        int level = 1;
};
