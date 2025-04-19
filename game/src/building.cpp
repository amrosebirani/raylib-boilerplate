#include "building.hpp"
#include "box2d/b2_body.h"
#include "building_data.hpp"
#include "constants.h"
#include "firebase.hpp"
#include "globals.h"
#include "property_type.hpp"
#include "raylib.h"
#include "upgrade_dialog.hpp"
#include "utils.h"
#include "draft.h"
#include "collider_factory.hpp"
#include <cmath>
#include <vector>

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

void Building::drawUpgradePolygon() {
    upgradePolyDrawPoints.clear();
    for (auto &p : relativeUpgradePolygon) {
        upgradePolyDrawPoints.push_back(
            {p.x * (*params)["highlight"] + upgradeCenter.x,
             p.y * (*params)["highlight"] + upgradeCenter.y});
    }
    DrawConvexPolygon(upgradePolyDrawPoints, DrawMode::FILL,
                      {BROWN.r, BROWN.g, BROWN.b, 200});

    if (inContact) {
        DrawConvexPolygon(upgradePolyDrawPoints, DrawMode::LINE,
                          {WHITE.r, WHITE.g, WHITE.b, 255});
    }
}

void Building::drawSummonPolygon() {
    summonPolyDrawPoints.clear();
    for (auto &p : relativeUpgradePolygon) {
        summonPolyDrawPoints.push_back(
            {p.x * (*params)["shighlight"] + summonCenter.x,
             p.y * (*params)["shighlight"] + summonCenter.y});
    }
    DrawConvexPolygon(summonPolyDrawPoints, DrawMode::FILL,
                      {PURPLE.r, PURPLE.g, PURPLE.b, 200});

    if (snContact) {
        DrawConvexPolygon(summonPolyDrawPoints, DrawMode::LINE,
                          {WHITE.r, WHITE.g, WHITE.b, 255});
    }
}

void Building::setUpgradeInfo() {
    int maxLevel = getMaxLevel(type);
    if (maxLevel == level) {
        inUpgrade = false;
        return;
    } else {
        inUpgrade = true;
    }
    coinsObtained = 0;
    upgradePoint = getBuildingUpgradePoint(buildingData, x, y, perc_cover);
    if (type == PropertyType::DEFENSE_TOWER) {
        upgradePoint.y -= ucr * 1.7;
    }
    // if (level == 0) {
    //
    // timer->after(4, [this](float dt) { this->inUpgrade = true; }, "");
    // }
}

void Building::drawLevelText() {
    const char *tt = TextFormat("Lv %d/%d", level, getMaxLevel(type));
    float ttm = MeasureText(tt, 10);
    float ylt = upgradePoint.y - 20;
    DrawText(tt, upgradePoint.x - ttm / 2, ylt, 10, WHITE);
}

void Building::drawUpgradeView(Color color) {
    if (level > 0) {

        healthBar->draw();
        drawLevelText();

        getSpriteHolder()->drawSprite(
            FIRE_BURNING_SPRITE_ID, getContainer()->getCurrentFireFrame(),
            {x - fire_size, y - fire_size, 2 * fire_size, 2 * fire_size});
    }

    if (inUpgrade) {
        if (getContainer()->hmm->isWaveActive()) {
            return;
        }
        const char *tt = TextFormat("%d", coinsToUpgrade - coinsObtained);
        float fs = 6;
        float ttm = MeasureText(tt, fs);
        // float tw = ttm + 10 + 5;
        // float angle = (coinsObtained * 1.0f / coinsToUpgrade) * 2 * M_PI;
        // DraftArc(upgradePoint.x, upgradePoint.y, ucr * 1.2, angle, -M_PI / 2,
        // 100, DrawMode::FILL, YELLOW, 1.0f);
        // DrawCircle(upgradePoint.x, upgradePoint.y, ucr, WHITE);
        // DrawText(tt, upgradePoint.x - tw / 2.0f, upgradePoint.y - 5, 10,
        // BLACK); getSpriteHolder()->drawSprite(
        //
        // COINS_AND_GEMS, 1,
        // {upgradePoint.x - tw / 2 + ttm + 5, upgradePoint.y - 5, 10, 10});
        // if (inContact) {
        // DrawCircle(upgradePoint.x, upgradePoint.y, ucr,
        // {YELLOW.r, YELLOW.g, YELLOW.b, 150});
        // }
        drawAura();
        drawUpgradePolygon();

        float cx = upgradePolygon[1].x / 2 + upgradePolygon[3].x / 2;
        float cy = upgradePolygon[0].y / 2 + upgradePolygon[2].y / 2;
        DrawText(tt, cx - ttm / 2, cy + 2, fs, WHITE);
        getSpriteHolder()->drawSprite(
            GOLD_ICON,
            {cx + ttm / 2 + fs / 2, cy + 2 - fs / 2, 2 * fs, 2 * fs});
        getSpriteHolder()->drawSprite(
            UPG_ICON,
            {cx - ttm / 2 - 5 - 2 * fs, cy + 2 - fs / 2, 2 * fs, 2 * fs});

        getSpriteHolder()->drawSprite(buildingData->building_icon,
                                      {cx - 15, cy - 30, 30, 30});
    }
    // drawDebugInfo();
}

void Building::drawSummonView() {
    if (summon_building && level > 0 && !getContainer()->hmm->isWaveActive()) {
        drawSummonPolygon();
        drawSummonTextRotated(summonCenter);
    }
}

void Building::drawDebugInfo() {

    if (buildingData->h_y > 0) {
        DrawRectangleLinesEx(transRect, 3, SKYBLUE);
    }
}

void Building::initiate() {

    if (type == PropertyType::BARRACKS || type == PropertyType::ARCHERY ||
        type == PropertyType::WIZARDRY) {
        summon_building = true;
    }
    data = new ColliderUserData();
    data->type = ColliderUserData::Type::BuildingSensor;
    data->obj = get_shared_ptr();
    cdata = new ColliderUserData();
    cdata->type = ColliderUserData::Type::Building;
    cdata->obj = get_shared_ptr();
    if (summon_building) {
        sndata = new ColliderUserData();
        sndata->type = ColliderUserData::Type::SummonSensor;
        sndata->obj = get_shared_ptr();
    }
    params = std::make_shared<std::unordered_map<std::string, float>>();
    (*params)["as"] = 0;
    setBasics();
    float hby = upgradePoint.y - 35;
    healthBar = std::make_shared<ProgressBar>(upgradePoint.x - 15, hby, 30, 3,
                                              maxHealth, health);

    relativeUpgradePolygon =
        getBuildingUpgradePolygon(buildingData, perc_cover);
    upgradeCenter = getBuildingUpgradeOrigin(buildingData, x, y, perc_cover);
    upgradePolygon.clear();
    for (auto &p : relativeUpgradePolygon) {
        upgradePolygon.push_back(
            {p.x + upgradeCenter.x, p.y + upgradeCenter.y});
    }
    (*params)["highlight"] = 1;
    if (isHighlighted) {
        highlight();
    }
    if (summon_building) {
        summonCenter = getBuildingSummonOrigin(buildingData, x, y, perc_cover);
        summonPolygon.clear();
        for (auto &p : relativeUpgradePolygon) {
            summonPolygon.push_back(
                {p.x + summonCenter.x, p.y + summonCenter.y});
        }
        (*params)["shighlight"] = 1;
        if (isSummonHighlighted) {
            summonHighlight();
        }
    }
}

void Building::setColliders() {
    std::vector<std::vector<Vector2>> points;
    std::vector<std::vector<Vector2>> cpoints;
    // std::vector<Vector2> poly =
    // getBuildingColliderPolygon(buildingData, x, y, perc_cover, true);
    std::vector<Vector2> cpoly =
        getBuildingColliderPolygon(buildingData, x, y, perc_cover, false);
    points.push_back(relativeUpgradePolygon);
    cpoints.push_back(cpoly);
    upgrade_sensor = ColliderFactory::newPolygonSensor(
        data, upgradeCenter.x, upgradeCenter.y, points, b2_staticBody,
        CATEGORY_BUILDING_SENSOR, CATEGORY_FORMATION,
        getContainer()->getWorld());
    inContact = false;
    inContactCount = 0;
    collider = ColliderFactory::newPolygonCollider(
        cdata, x, y, cpoints, b2_staticBody, CATEGORY_BUILDING,
        CATEGORY_ENEMY | CATEGORY_ENEMY_PROJECTILE | CATEGORY_FORMATION |
            CATEGORY_WARRIOR_OF | CATEGORY_COLLECTIBLE,
        getContainer()->getWorld());
    if (summon_building) {
        std::vector<std::vector<Vector2>> spoints;
        spoints.push_back(relativeUpgradePolygon);
        summon_sensor = ColliderFactory::newPolygonSensor(
            sndata, summonCenter.x, summonCenter.y, spoints, b2_staticBody,
            CATEGORY_BUILDING_SENSOR, CATEGORY_FORMATION,
            getContainer()->getWorld());
    }
    if (level == 0) {
        collider = nullptr;
    }
}

void Building::colliderCleanup() {

    data->obj = nullptr;
    cdata->obj = nullptr;
    if (summon_building) {
        sndata->obj = nullptr;
    }
}

void Building::highlight() {
    isHighlighted = true;
    startGrow(1);
}

void Building::summonHighlight() {
    isHighlighted = true;
    startSummonGrow(1);
}

void Building::startSummonGrow(float start) {
    if (!isSummonHighlighted) {
        return;
    }
    (*params)["shighlight"] = start;
    timer->tween(0.5, params, {{"shighlight", 1.1f}}, "linear",
                 [this]() { this->startSummonShrink(1.1); }, "shighlight_up",
                 {});
}

void Building::startSummonShrink(float start) {
    if (!isSummonHighlighted) {
        return;
    }
    (*params)["shighlight"] = start;
    timer->tween(0.5, params, {{"shighlight", .9f}}, "linear",
                 [this]() { this->startSummonGrow(0.9); }, "shighlight_down",
                 {});
}

void Building::startGrow(float start) {
    if (!isHighlighted) {
        return;
    }
    (*params)["highlight"] = start;
    timer->tween(0.5, params, {{"highlight", 1.1f}}, "linear",
                 [this]() { this->startShrink(1.1); }, "highlight_up", {});
}

void Building::startShrink(float start) {
    if (!isHighlighted) {
        return;
    }
    (*params)["highlight"] = start;
    timer->tween(0.5, params, {{"highlight", .9f}}, "linear",
                 [this]() { this->startGrow(0.9); }, "highlight_down", {});
}

void Building::awakenColliders(float dt) {
    if (upgrade_sensor) {
        upgrade_sensor->SetAwake(true);
    }
    if (collider) {
        collider->SetAwake(true);
    }
    if (summon_building && summon_sensor && level > 0) {
        summon_sensor->SetAwake(true);
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
        float perc_health = health * 1.0f / maxHealth;
        if (perc_health < .75) {
            fire_size = 20 + 30 * (.75 - perc_health);
        }
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

    if (summon_building) {
        if (snContact) {
            if (getContainer()->hmm->isWaveActive()) {
                return;
            }
            snContactTimer += dt;
            if (snContactTimer > 1) {
                if (isSummonHighlighted) {
                    isSummonHighlighted = false;
                    timer->cancel("shighlight_up");
                    timer->cancel("shighlight_down");
                }
                if (!summonDialogShown) {
                    summonDialogShown = true;
                    pushSummonDialog();
                    // getStateStack()->push(std::make_shared<UpgradeDialog>(
                    // type, level + 1, coinsToUpgrade,
                    // [this]() { this->doUpgrade(); },
                    // getBuildingData(getBuildingId(type, level + 1))));
                }
            }
            return;
        } else {
            snContactTimer = 0;
        }
    }

    if (!inUpgrade) {
        return;
    }

    // if (coinsObtained >= coinsToUpgrade) {
    // doUpgrade();
    // }

    if (inContact) {
        if (getContainer()->hmm->isWaveActive()) {
            return;
        }
        inContactTimer += dt;
        if (inContactTimer > 1) {
            if (isHighlighted) {
                isHighlighted = false;
                timer->cancel("highlight_up");
                timer->cancel("highlight_down");
                getViewCamera()->follow(getContainer()->getFollowObject());
            }
            if (!upgradeDialogShown) {
                upgradeDialogShown = true;
                getStateStack()->push(std::make_shared<UpgradeDialog>(
                    type, level + 1, coinsToUpgrade,
                    [this]() { this->doUpgrade(); },
                    getBuildingData(getBuildingId(type, level + 1))));
            }
        }
    } else {
        inContactTimer = 0;
    }
}

void Building::doUpgrade() {
    sendFirebaseEvent("BuildingUpgrade",
                      {{"building_type", buildingData->building_text},
                       {"level", TextFormat("%d", level + 1)}});
    getWorldState()->removeCoins(coinsToUpgrade);
    inContactTimer = 0;
    fire_size = 0;
    level += 1;
    if (level == 1) {
        getContainer()->addAttackUnit(
            std::dynamic_pointer_cast<Building>(get_shared_ptr()));
        getAudioManager()->playSound("building_created");
        if (type == PropertyType::DEFENSE_TOWER) {
            getContainer()->region->towerRaised = true;
        }
        if (type == PropertyType::HOUSE) {
            getContainer()->region->houseRaised = true;
        }
        if (type == PropertyType::BARRACKS) {
            getContainer()->region->barracksRaised = true;
        }
        if (type == PropertyType::ARCHERY) {
            getContainer()->region->archeryRaised = true;
        }
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
    healthBar->setY(upgradePoint.y - 35);
    healthBar->setX(upgradePoint.x - 15);
    onUpgrade(level);
}

void Building::downgrade() {

    level = 0;
    sendFirebaseEvent("BuildingDestroyed",
                      {{"building_type", buildingData->building_text}});
    onUpgrade(level);
    setBasics();
    // initAuraPoints();
    setUpgradeInfo();
    setColliders();
    getAudioManager()->playSound("building_destruction");
}

void Building::startContact() {
    inContactCount++;
    if (!inContact) {

        inContact = true;
        timer->tween(1, this->params, {{"as", 1}}, "linear", []() {},
                     "aura_tween", {});
    }
}

void Building::endContact() {
    inContactCount -= 1;
    if (inContactCount <= 0) {
        inContactCount = 0;
        inContact = false;
        timer->cancel("aura_tween");
        (*params)["as"] = 0;
        upgradeDialogShown = false;
    }
}

void Building::startSummonContact() {
    snContact = true;
}

void Building::endSummonContact() {
    snContact = false;
    summonDialogShown = false;
}

void Building::setBasics() {
    buildingData = getBuildingData(getBuildingId(type, level));
    perc_cover = getPercentCover(type, level);
    aura_r = CASTLE_WIDTH * perc_cover * 1.2 / 2;
    transRect = getBuildingTransparencyRect(buildingData, x, y, perc_cover);
    upgradePoint = getBuildingUpgradePoint(buildingData, x, y, perc_cover);
    relativeUpgradePolygon =
        getBuildingUpgradePolygon(buildingData, perc_cover);
    upgradeCenter = getBuildingUpgradeOrigin(buildingData, x, y, perc_cover);
    upgradePolygon.clear();
    for (auto &p : relativeUpgradePolygon) {
        upgradePolygon.push_back(
            {p.x + upgradeCenter.x, p.y + upgradeCenter.y});
    }
    if (summon_building) {
        summonCenter = getBuildingSummonOrigin(buildingData, x, y, perc_cover);
        summonPolygon.clear();
        for (auto &p : relativeUpgradePolygon) {
            summonPolygon.push_back(
                {p.x + summonCenter.x, p.y + summonCenter.y});
        }
    }
    if (type == PropertyType::DEFENSE_TOWER) {
        upgradePoint.y -= ucr * 1.7;
    }
    int nextLevel = level + 1;
    if (nextLevel > getMaxLevel(type)) {
        nextLevel = level;
    }
    coinsToUpgrade =
        getUpgradeCoins(type, nextLevel) *
        getContainer()->getUpgradeContent()->get_stat(BUILDING_UPGRADE_COST_M) /
        2;
    const char *tt = TextFormat("%d", coinsToUpgrade);
    float ttm = MeasureText(tt, 10);
    float tw = ttm + 10 + 5;
    ucr = (tw + 5) / 2.0f;
}

Building::~Building() {
    delete data;
    delete sndata;
    // delete buildingData;
    upgrade_sensor = nullptr;
    collider = nullptr;
    if (summon_building) {
        summon_sensor = nullptr;
    }
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

void Building::pushSummonDialog() {
}
