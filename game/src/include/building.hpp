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
        Building(std::ifstream &in);
        virtual ~Building();
        void takeDamage(float damage);
        virtual void repair(float repairAmount) = 0;
        virtual void onUpgrade(int level) = 0;
        virtual void pushSummonDialog();
        virtual void buildingObjectSave(std::ofstream &out) const = 0;
        void baseBuildingObjectSave(std::ofstream &out) const;
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
        bool inUpgrade = false;
        Vector2 upgradePoint;
        void drawUpgradeView(Color color);
        void drawSummonView();
        void setColliders();
        void awakenColliders(float dt);
        bool inContact = false;
        int inContactCount = 0;
        bool snContact = false;
        float inContactTimer;
        float snContactTimer;
        // float coinExchangeTimer;
        void doUpgrade();
        void setBasics();
        ColliderUserData *data = nullptr;
        ColliderUserData *cdata = nullptr;
        ColliderUserData *sndata = nullptr;
        void initiate();
        void downgrade();
        void drawAura();
        void startContact();
        void endContact();
        void startSummonContact();
        void endSummonContact();
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
        void highlight();
        void summonHighlight();
        void drawSummonPolygon();

    private:
        PropertyType type;
        std::shared_ptr<b2Body> upgrade_sensor;
        std::shared_ptr<b2Body> summon_sensor;
        std::shared_ptr<b2Body> collider;
        std::shared_ptr<ProgressBar> healthBar;
        std::vector<Vector2> upgradePolygon;
        std::vector<Vector2> summonPolygon;
        std::vector<Vector2> relativeUpgradePolygon;
        std::vector<Vector2> upgradePolyDrawPoints;
        std::vector<Vector2> summonPolyDrawPoints;
        Vector2 upgradeCenter;
        Vector2 summonCenter;
        bool underAttack = false;
        float attackCounter = 0;
        float attackExpiry = 2.0f;
        bool blinkToggle = false;
        bool upgradeDialogShown = false;
        bool summonDialogShown = false;
        float toggleChangeTime = 0.5f;
        float toggleCounter = 0;
        bool isHighlighted = false;
        bool isSummonHighlighted = false;
        void startGrow(float start);
        void startShrink(float start);
        void startSummonGrow(float start);
        void startSummonShrink(float start);
        void drawUpgradePolygon();
        float fire_size = 0;
        bool summon_building = false;
};
