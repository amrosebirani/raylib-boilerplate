#pragma once
#include "box2d/b2_body.h"
#include "building_data.hpp"
#include "collider_user_data.h"
#include "game_object.h"
#include "progress_bar.hpp"
#include "property_type.hpp"
#include "raylib.h"

class Building : public GameObject {
    public:
        Building(float x, float y, PropertyType type, int level,
                 float health = 0)
            : GameObject(x, y), type(type), level(level), health(health) {};
        virtual ~Building();
        void takeDamage(float damage);
        virtual void repair(float repairAmount) = 0;
        virtual void onUpgrade(int level) = 0;
        int maxHealth;
        int health;
        int level = 0;
        float tributeGenerated = 0;
        BuildingData *buildingData;
        bool alive = true;
        float perc_cover;
        float aura_r;
        std::vector<Vector2> auraPoints;
        void initAuraPoints();
        void setUpgradeInfo();
        int coinsToUpgrade;
        int coinsObtained;
        bool inUpgrade;
        Vector2 upgradePoint;
        void drawUpgradeView(Color color);
        void setColliders();
        void awakenColliders(float dt);
        bool inContact = false;
        float inContactTimer;
        float coinExchangeTimer;
        void doUpgrade();
        void setBasics();
        ColliderUserData *data = nullptr;
        ColliderUserData *cdata = nullptr;
        void initiate();
        void downgrade();
        void drawAura();
        void startContact();
        void endContact();
        void drawLevelText();
        void colliderCleanup();
        unsigned char buildingAlpha;
        Rectangle transRect;
        void drawDebugInfo();
        Color getColor() override {
            if (level == 0) {
                return {0, 0, 0, 0};
            }
            return {BEIGE.r, BEIGE.g, BEIGE.b, 159};
        };
        float ucr; // upgrade circle radius

        bool hasCards = false;

    private:
        PropertyType type;
        std::shared_ptr<b2Body> upgrade_sensor;
        std::shared_ptr<b2Body> collider;
        std::shared_ptr<ProgressBar> healthBar;
        bool underAttack = false;
        float attackCounter = 0;
        float attackExpiry = 2.0f;
        bool blinkToggle = false;
        float toggleChangeTime = 0.5f;
        float toggleCounter = 0;
        float baseTributeRate = 0;
        float tributeRate = 0;
};
