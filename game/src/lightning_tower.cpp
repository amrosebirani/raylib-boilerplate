#include "lightning_tower.hpp"

#include "globals.h"
#include "lightning_line.hpp"
#include "property_type.hpp"
#include "collider_factory.hpp"
#include "utils.h"
#include <memory>
#include <fstream>

LightningTower::LightningTower(float x, float y, int level)
    : Building(x, y, PropertyType::LIGHTNING_TOWER, level) {
    maxHealth = getMaxHealthByLevel(level, PropertyType::LIGHTNING_TOWER);
    health = maxHealth;
}

LightningTower::LightningTower(std::ifstream &in) : Building(in) {
}

void LightningTower::init() {
    initiate();
    alive = false;
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
    sdata = new ColliderUserData();
    sdata->obj = get_shared_ptr();
    sdata->type = ColliderUserData::Type::LightningTowerSensor;
    enemySensor = ColliderFactory::newCircleSensor(
        sdata, x, y, DEFENSE_TOWER_SENSOR_RADIUS * 1.2, b2_staticBody,
        CATEGORY_BUILDING_SENSOR, CATEGORY_ENEMY, getContainer()->getWorld());
    Vector2 pp =
        translateBuildingPoint(buildingData, x, y, perc_cover, startPoint);
    xs = pp.x;
    ys = pp.y;
}

void LightningTower::repair(float repairAmount) {
    health += repairAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void LightningTower::draw() {
    // DrawConvexPolygon(auraPoints, DrawMode::FILL, {RED.r, RED.g, RED.b, 60});
    getSpriteHolder()->drawSprite(buildingData, x, y, perc_cover,
                                  buildingAlpha);
    drawUpgradeView(RED);
}

bool LightningTower::isAlive() {
    return true;
}

void LightningTower::update(float dt) {
    timer->update(dt);
    awakenColliders(dt);
    enemySensor->SetAwake(true);
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
    if (level > 0 && enemies.size() > 0) {
        cooldown_time_counter += dt;
        if (cooldown_time_counter > cooldown_time) {
            cooldown_time_counter -= cooldown_time;
            int enemyIndex = getRandomIntInRange(0, enemies.size() - 1);
            std::shared_ptr<Enemy> ee =
                std::dynamic_pointer_cast<Enemy>(enemies[enemyIndex]);
            hitLightningBolt(ee);
        }

    } else {
        cooldown_time_counter = 0;
    }
}

void LightningTower::cleanupData() {
    colliderCleanup();
    sdata->obj = nullptr;
}

void LightningTower::die() {
    alive = false;
    tributeGenerated = 0;
    enemies.clear();
}

LightningTower::~LightningTower() {
    delete sdata;
    enemySensor = nullptr;
}

void LightningTower::addEnemy(std::shared_ptr<GameObject> enemy) {
    if (!enemy->isAlive()) return;
    enemies.push_back(enemy);
}

void LightningTower::removeEnemy(std::shared_ptr<GameObject> enemy) {

    auto it = std::find(enemies.begin(), enemies.end(), enemy);
    if (it != enemies.end()) {
        enemies.erase(it);
    }
}

void LightningTower::hitLightningBolt(std::shared_ptr<Enemy> enemy) {

    float x2 = enemy->x;
    float y2 = enemy->y;
    enemy->takeAttack(damage);
    std::shared_ptr<LightningLine> ll =
        std::make_shared<LightningLine>(xs, ys, x2, y2);
    getContainer()->addGameObject(ll);
}

void LightningTower::onUpgrade(int level) {
}

void LightningTower::buildingObjectSave(std::ofstream &out) const {
    baseBuildingObjectSave(out);
}
