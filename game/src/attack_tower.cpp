
#include "attack_tower.hpp"
#include "archer.hpp"
#include "collider_factory.hpp"
#include "constants.h"
#include "game_object.h"
#include "globals.h"
#include "property_type.hpp"
#include "utils.h"
#include <iostream>

AttackTower::AttackTower(float x, float y, int level)
    : Building(x, y, PropertyType::DEFENSE_TOWER, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::DEFENSE_TOWER);
    health = maxHealth;
}

void AttackTower::init() {
    initiate();
    timer.every(
        1, [this](float dt) { this->tributeGenerated += .1; }, 0, []() {}, "");
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
    sdata = new ColliderUserData();
    sdata->obj = get_shared_ptr();
    sdata->type = ColliderUserData::Type::AttackTowerSensor;
    enemySensor = ColliderFactory::newCircleSensor(
        sdata, x, y, DEFENSE_TOWER_SENSOR_RADIUS, b2_staticBody,
        CATEGORY_BUILDING_SENSOR, CATEGORY_ENEMY, getContainer()->getWorld());
    setArchers(1);
}

void AttackTower::repair(float repairAmount) {
    health += repairAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void AttackTower::draw() {
    // DrawConvexPolygon(auraPoints, DrawMode::FILL, {RED.r, RED.g, RED.b, 60});
    getSpriteHolder()->drawSprite(buildingData, x, y, perc_cover,
                                  buildingAlpha);
    drawUpgradeView(RED);
}

void AttackTower::setArchers(int level, bool operational) {
    for (auto archer : archers) {
        archer->die();
    }
    archers.clear();
    std::vector<Vector2> archerPositions = getAttackTowerArcherPos(level);
    for (auto pos : archerPositions) {
        std::shared_ptr<Archer> archer = std::make_shared<Archer>(
            x + pos.x, y + pos.y, get_shared_ptr(), operational);
        archers.push_back(archer);
        getContainer()->addGameObject(archer);
    }
}

bool AttackTower::isAlive() {
    return true;
}

void AttackTower::update(float dt) {
    timer.update(dt);
    awakenColliders(dt);
    enemySensor->SetAwake(true);
    // if mouse is clicked print the location relative to this object
    // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    //     Vector2 mousePos = getViewCamera()->getMousePosition();
    //     if (CheckCollisionPointRec(mousePos,
    //                                {transRect.x, transRect.y - 20,
    //                                 transRect.width, transRect.height + 20}))
    //                                 {
    //         Vector2 pos = {x, y};
    // Vector2 diff = Vector2Subtract(mousePos, pos);
    // std::cout << "diff: " << diff.x << " " << diff.y << std::endl;
    // }
    // }
    // remove all the non alive enemies from enemies vector
    std::vector<size_t> enemyIndicesToRemove;

    for (size_t i = 0; i < enemies.size(); i++) {
        std::shared_ptr<GameObject> go = enemies[i];
        if (!go->isAlive()) {
            enemyIndicesToRemove.push_back(i);
        }
    }
    for (size_t i = enemyIndicesToRemove.size(); i > 0; --i) {
        size_t index = enemyIndicesToRemove[i - 1];
        enemies.erase(enemies.begin() + index);
    }

    if (previous_level != 0 && level == 0) {
        for (auto archer : archers) {
            std::shared_ptr<Archer> archerObj =
                std::dynamic_pointer_cast<Archer>(archer);
            archerObj->isOperational = false;
        }
    }
    if (previous_level == 0 && level != 0) {
        for (auto archer : archers) {
            std::shared_ptr<Archer> archerObj =
                std::dynamic_pointer_cast<Archer>(archer);
            archerObj->isOperational = true;
            std::cout << "Archer operational" << std::endl;
        }
    }
    if (previous_level == 1 && level == 2) {
        setArchers(2, true);

        enemySensor = nullptr;
        enemySensor = ColliderFactory::newCircleSensor(
            sdata, x, y, 1.5 * DEFENSE_TOWER_SENSOR_RADIUS, b2_staticBody,
            CATEGORY_BUILDING_SENSOR, CATEGORY_ENEMY,
            getContainer()->getWorld());
    }
    if (previous_level == 2 && level == 0) {
        setArchers(1);
        enemySensor = nullptr;
        enemySensor = ColliderFactory::newCircleSensor(
            sdata, x, y, DEFENSE_TOWER_SENSOR_RADIUS, b2_staticBody,
            CATEGORY_BUILDING_SENSOR, CATEGORY_ENEMY,
            getContainer()->getWorld());
    }
    if (previousAlpha != buildingAlpha) {
        for (auto archer : archers) {
            std::shared_ptr<Archer> archerObj =
                std::dynamic_pointer_cast<Archer>(archer);
            archerObj->alpha = buildingAlpha;
        }
    }
    previous_level = level;
    previousAlpha = buildingAlpha;
}

void AttackTower::cleanupData() {
    colliderCleanup();
    sdata->obj = nullptr;
}

void AttackTower::die() {
    alive = false;
    tributeGenerated = 0;
}

AttackTower::~AttackTower() {
    delete sdata;
    enemySensor = nullptr;
}

void AttackTower::addEnemy(std::shared_ptr<GameObject> enemy) {
    enemies.push_back(enemy);
}

void AttackTower::onUpgrade(int level) {
}
