#include "castle.hpp"
#include "arrow.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "fire_ball.hpp"
#include "globals.h"
#include "utils.h"
#include "raylib.h"
#include "collider_factory.hpp"
#include "warrior_types.h"
#include <iostream>

Castle::Castle(float x, float y, float health, int level)
    : Building(x, y, PropertyType::CASTLE, level, health) {
    graphics_types.push_back("castle");
    maxHealth = getMaxHealthByLevel(level, PropertyType::CASTLE);
    changeAttackTimeout = getCastleAttackTimer(level);
}

void Castle::init() {

    initiate();
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
    esdata = new ColliderUserData();
    esdata->obj = get_shared_ptr();
    esdata->type = ColliderUserData::Type::CastleSensor;
    enemySensor = ColliderFactory::newCircleSensor(
        esdata, x, y, CASTLE_SENSOR_RADIUS, b2_staticBody,
        CATEGORY_BUILDING_SENSOR, CATEGORY_ENEMY, getContainer()->getWorld());
}

Castle::~Castle() {
    delete esdata;
    enemySensor = nullptr;
}

void Castle::cleanupData() {
    colliderCleanup();
    esdata->obj = nullptr;
}

void Castle::update(float dt) {
    enemySensor->SetAwake(true);
    if (!alive) {
        cdata->obj = nullptr;
        return;
    }
    if (health > .5f * maxHealth) {
        state = CastleState::OPERATIONAL;
    } else if (health <= .5f * maxHealth && health > 0) {
        state = CastleState::UNOPERATIONAL;
    } else {
        state = CastleState::CAPTURED;
    }
    if (isAttacked) {
        if (state == CastleState::OPERATIONAL) {
            state = CastleState::UNOPERATIONAL_UNDER_ATTACK;
        }
        if (state == CastleState::UNOPERATIONAL) {
            state = CastleState::UNOPERATIONAL_UNDER_ATTACK;
        }
    }
    if (health <= 0) {
        die();
    }
    changeAttackCounter += dt;
    if (changeAttackCounter >= changeAttackTimeout) {
        changeAttackCounter = 0;
        isAttacked = false;
    }
    timer->update(dt);
    awakenColliders(dt);
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
    if (level > 0) {
        bowAttackCounter += dt;
        if (bowAttackCounter >= bowAttackTime) {
            bowAttack();
            bowAttackCounter = 0;
        }
    }
    if (level > 1) {
        fireBallAttackCounter += dt;
        if (fireBallAttackCounter >= fireBallAttackTime) {
            fireBallAttack();
            fireBallAttackCounter = 0;
        }
    }
}

void Castle::bowAttack() {
    if (enemies.empty()) {
        return;
    }
    auto enemy = enemies[getRandomIntInRange(0, enemies.size() - 1)];
    std::shared_ptr<Arrow> arrow =
        std::make_shared<Arrow>(x, y, Vector2{enemy->x - x, enemy->y - y},
                                WarriorType::WARRIOR_TYPE_ARCHER);
    arrow->init();
    getContainer()->addGameObject(arrow);
}

void Castle::fireBallAttack() {
    if (enemies.empty()) {
        return;
    }
    auto enemy = enemies[getRandomIntInRange(0, enemies.size() - 1)];
    std::shared_ptr<FireBall> fireBall =
        std::make_shared<FireBall>(x, y, Vector2{enemy->x - x, enemy->y - y});
    fireBall->init();
    getContainer()->addGameObject(fireBall);
    std::cout << "fireball launched" << std::endl;
}

void Castle::hit(float damage) {
    isAttacked = true;
    takeDamage(damage);
    if (health * 1.0f / maxHealth < 0.5) {
        getAudioManager()->switchBGM("under_attack_fucked");
    }
}

void Castle::repair(float repairAmount) {
    float prevHealth = health;
    health += repairAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
    if (prevHealth * 1.0f / maxHealth < 0.5 &&
        health * 1.0f / maxHealth >= 0.5) {
        getAudioManager()->switchBGM("under_attack_in_control");
    }
}

void Castle::draw() {
    // DrawRectangle(x - width / 2, y - height / 2, width, height, RAYWHITE);
    // float mt = MeasureText(TextFormat("Castle"), 20);
    // DrawText("Castle", x - mt / 2, y - 10, 20, BLACK);
    // getSpriteHolder()->drawSprite(
    // CASTLE_SPRITE_ID, {x - width / 2, y - height / 2, width, height});
    // DrawConvexPolygon(auraPoints, DrawMode::FILL,
    // {YELLOW.r, YELLOW.g, YELLOW.b, 60});
    getSpriteHolder()->drawSprite(buildingData, x, y, perc_cover,
                                  buildingAlpha);

    // float rx = width / 2178;
    // float ry = height / 1516;
    // DrawConvexPolygon({Vector2{x - 1073 * rx, y + 234 * ry},
    // Vector2{x - 97 * rx, y + 730 * ry},
    // Vector2{x + 903 * rx, y + 218 * ry},
    // Vector2{x - 73 * rx, y - 94 * ry}},
    // DrawMode::FILL, WHITE);
    // DrawRectangle(x - width / 2, y - height / 2, width, height,
    //               {255, 255, 255, 100});
    // DrawCircle(x, y, 5, RED);
    drawUpgradeView(YELLOW);
}

bool Castle::isAlive() {
    return alive;
}

void Castle::die() {
    alive = false;
    enemies.clear();
}

Vector2 Castle::getDimensions() {
    return Vector2{width, height};
}

Castle::CastleState Castle::getState() {
    return state;
}

void Castle::addEnemy(std::shared_ptr<GameObject> enemy) {
    if (level == 0) {
        return;
    }
    if (!enemy->isAlive()) {
        return;
    }
    enemies.push_back(enemy);
}

void Castle::onUpgrade(int level) {
    if (level > 0) {
        if (getContainer()->hmm->isWaveActive()) {
            getAudioManager()->switchBGM("under_attack_in_control");
        } else {
            getAudioManager()->switchBGM("normal");
        }
    }
}
