#include "building.hpp"
#include "box2d/b2_body.h"
#include "constants.h"
#include "globals.h"
#include "property_type.hpp"
#include "raylib.h"
#include "utils.h"
#include "draft.h"
#include "collider_factory.hpp"
#include <cmath>

void Building::initAuraPoints() {
    auraPoints.clear();
    std::vector<Vector2> app =
        getBuildingColliderPolygon(buildingData, x, y, perc_cover, true);
    for (auto &p : app) {
        auraPoints.push_back({
            p.x + x,
            p.y + y,
        });
    }
}

void Building::drawAura() {
    auraPoints.clear();
    std::vector<Vector2> app =
        getBuildingColliderPolygon(buildingData, x, y, perc_cover, true);
    for (auto &p : app) {
        auraPoints.push_back({
            p.x * (*params)["as"] + x,
            p.y * (*params)["as"] + y,
        });
    }
    DrawConvexPolygon(auraPoints, DrawMode::FILL,
                      {YELLOW.r, YELLOW.g, YELLOW.b, 60});
}

void Building::setUpgradeInfo() {
    int maxLevel = getMaxLevel(type);
    if (maxLevel == level) {
        inUpgrade = false;
        return;
    }
    coinsObtained = 0;
    upgradePoint = getBuildingUpgradePoint(buildingData, x, y, perc_cover);
    if (type == PropertyType::DEFENSE_TOWER) {
        upgradePoint.y -= ucr * 1.7;
    }
    timer->after(4, [this](float dt) { this->inUpgrade = true; }, "");
}

void Building::drawLevelText() {
    const char *tt = TextFormat("Lv %d/%d", level, getMaxLevel(type));
    float ttm = MeasureText(tt, 10);
    float ylt = upgradePoint.y - ucr - 20;
    DrawText(tt, upgradePoint.x - ttm / 2, ylt, 10, WHITE);
}

void Building::drawUpgradeView(Color color) {
    if (level > 0) {

        healthBar->draw();
        drawLevelText();
    }

    if (inUpgrade) {
        const char *tt = TextFormat("%d", coinsToUpgrade - coinsObtained);
        float ttm = MeasureText(tt, 10);
        float tw = ttm + 10 + 5;
        float angle = (coinsObtained * 1.0f / coinsToUpgrade) * 2 * M_PI;
        DraftArc(upgradePoint.x, upgradePoint.y, ucr * 1.2, angle, -M_PI / 2,
                 100, DrawMode::FILL, YELLOW, 1.0f);
        DrawCircle(upgradePoint.x, upgradePoint.y, ucr, WHITE);
        DrawText(tt, upgradePoint.x - tw / 2.0f, upgradePoint.y - 5, 10, BLACK);
        getSpriteHolder()->drawSprite(

            COINS_AND_GEMS, 1,
            {upgradePoint.x - tw / 2 + ttm + 5, upgradePoint.y - 5, 10, 10});
        if (inContact) {
            DrawCircle(upgradePoint.x, upgradePoint.y, ucr,
                       {YELLOW.r, YELLOW.g, YELLOW.b, 150});
        }
        drawAura();
    }
    // drawDebugInfo();
}

void Building::drawDebugInfo() {

    if (buildingData->h_y > 0) {
        DrawRectangleLinesEx(transRect, 3, SKYBLUE);
    }
}

void Building::initiate() {

    data = new ColliderUserData();
    data->type = ColliderUserData::Type::BuildingSensor;
    data->obj = get_shared_ptr();
    cdata = new ColliderUserData();
    cdata->type = ColliderUserData::Type::Building;
    cdata->obj = get_shared_ptr();
    params = std::make_shared<std::unordered_map<std::string, float>>();
    (*params)["as"] = 0;
    setBasics();
    float hby = upgradePoint.y - ucr - 35;
    healthBar = std::make_shared<ProgressBar>(upgradePoint.x - 15, hby, 30, 3,
                                              maxHealth, health);
    baseTributeRate = getTributeRate(type);
    tributeRate = level * baseTributeRate;
    timer->every(
        3,
        [this](float dt) {
            if (this->type == PropertyType::CASTLE) {
                return;
            }
            // std::cout << "Tribute generated: " << this->tributeGenerated
            // << std::endl;
            int tt = floor(this->tributeGenerated);
            this->tributeGenerated -= tt;
            // std::cout << "Tribute generated: " << tt << std::endl;
            getWorldState()->addCoins(tt);
        },
        0, []() {}, "");
}

void Building::setColliders() {
    std::vector<std::vector<Vector2>> points;
    std::vector<std::vector<Vector2>> cpoints;
    std::vector<Vector2> poly =
        getBuildingColliderPolygon(buildingData, x, y, perc_cover, true);
    std::vector<Vector2> cpoly =
        getBuildingColliderPolygon(buildingData, x, y, perc_cover, false);
    points.push_back(poly);
    cpoints.push_back(cpoly);
    upgrade_sensor = ColliderFactory::newPolygonSensor(
        data, x, y, points, b2_staticBody, CATEGORY_BUILDING_SENSOR,
        CATEGORY_FORMATION, getContainer()->getWorld());
    collider = ColliderFactory::newPolygonCollider(
        cdata, x, y, cpoints, b2_staticBody, CATEGORY_BUILDING,
        CATEGORY_ENEMY | CATEGORY_ENEMY_PROJECTILE | CATEGORY_FORMATION |
            CATEGORY_WARRIOR_OF | CATEGORY_COLLECTIBLE,
        getContainer()->getWorld());
    if (level == 0) {
        collider = nullptr;
    }
}

void Building::colliderCleanup() {

    data->obj = nullptr;
    cdata->obj = nullptr;
}

void Building::awakenColliders(float dt) {
    if (upgrade_sensor) {
        upgrade_sensor->SetAwake(true);
    }
    if (collider) {
        collider->SetAwake(true);
    }
    if (level == 0) {
        buildingAlpha = 100;
    } else {
        if (buildingData->h_y < 0) {
            buildingAlpha = 255;
        } else {
            if (CheckCollisionPointRec(getContainer()->getFormPos(),
                                       transRect)) {
                buildingAlpha = 200;
            } else {
                buildingAlpha = 255;
            }
        }
    }

    if (underAttack) {
        toggleCounter += dt;
        if (toggleCounter > toggleChangeTime) {
            toggleCounter -= toggleChangeTime;
            blinkToggle = !blinkToggle;
            if (blinkToggle) {
                buildingAlpha = 100;
            }
        }
    }

    if (level > 0) {

        healthBar->setCurrent(health);
    }

    if (underAttack) {
        attackCounter += dt;
        if (attackCounter > attackExpiry) {
            underAttack = false;
            buildingAlpha = 255;
            attackCounter = 0;
            toggleCounter = 0;
        }
    }

    tributeGenerated += tributeRate * dt;
    if (!inUpgrade) {
        return;
    }

    if (coinsObtained >= coinsToUpgrade) {
        doUpgrade();
    }

    if (inContact) {
        if (underAttack) {
            return;
        }
        if (hasCards) {
            return;
        }
        if (getWorldState()->getCoins() == 0) {
            inContactTimer = 0;
            coinExchangeTimer = 0;
            return;
        }
        inContactTimer += dt;
        if (inContactTimer > 1) {
            coinExchangeTimer += dt;
            if (coinExchangeTimer > .3) {
                coinsObtained += 1;
                getWorldState()->removeCoins(1);
                getAudioManager()->playSound("coin_dropped");
                coinExchangeTimer -= 0.3;
            }
        }
    } else {
        inContactTimer = 0;
        coinExchangeTimer = 0;
    }
}

void Building::doUpgrade() {
    inContactTimer = 0;
    level += 1;
    tributeRate = level * baseTributeRate;
    if (level == 1) {
        getContainer()->addAttackUnit(
            std::dynamic_pointer_cast<Building>(get_shared_ptr()));
        getAudioManager()->playSound("building_created");
    } else {
        getAudioManager()->playSound("building_upgraded");
    }
    if (type == PropertyType::CASTLE) {
        // add a ring
        getContainer()->region->addPropertyRing(level - 1);
    }
    maxHealth = getMaxHealthByLevel(level, type);
    health = maxHealth;
    healthBar->reset(maxHealth, health);
    setBasics();
    // initAuraPoints();
    setUpgradeInfo();
    setColliders();
    healthBar->setY(upgradePoint.y - ucr - 35);
    healthBar->setX(upgradePoint.x - 15);
    onUpgrade(level);
}

void Building::downgrade() {

    level = 0;
    tributeRate = level * baseTributeRate;
    onUpgrade(level);
    setBasics();
    // initAuraPoints();
    setUpgradeInfo();
    setColliders();
    getAudioManager()->playSound("building_destruction");
}

void Building::startContact() {
    inContact = true;
    timer->tween(1, this->params, {{"as", 1}}, "linear", []() {}, "aura_tween",
                 {});
}

void Building::endContact() {
    inContact = false;
    timer->cancel("aura_tween");
    (*params)["as"] = 0;
}

void Building::setBasics() {
    buildingData = getBuildingData(getBuildingId(type, level));
    perc_cover = getPercentCover(type, level);
    aura_r = CASTLE_WIDTH * perc_cover * 1.2 / 2;
    transRect = getBuildingTransparencyRect(buildingData, x, y, perc_cover);
    upgradePoint = getBuildingUpgradePoint(buildingData, x, y, perc_cover);
    if (type == PropertyType::DEFENSE_TOWER) {
        upgradePoint.y -= ucr * 1.7;
    }
    int nextLevel = level + 1;
    if (nextLevel > getMaxLevel(type)) {
        nextLevel = level;
    }
    coinsToUpgrade = getUpgradeCoins(type, nextLevel) / 10;
    const char *tt = TextFormat("%d", coinsToUpgrade);
    float ttm = MeasureText(tt, 10);
    float tw = ttm + 10 + 5;
    ucr = (tw + 5) / 2.0f;
}

Building::~Building() {
    delete data;
    // delete buildingData;
    upgrade_sensor = nullptr;
    collider = nullptr;
}

void Building::takeDamage(float damage) {
    underAttack = true;
    attackCounter = 0;
    health -= damage;
    getAudioManager()->playBuildingHitSound();
    if (health <= 0) {
        downgrade();
    }
}
