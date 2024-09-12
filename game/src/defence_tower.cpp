#include "archer.hpp"
#include "box2d/b2_fixture.h"
#include "constants.h"
#include "defense_tower.hpp"
#include "globals.h"
#include "collider_factory.hpp"
#include "box2d/b2_circle_shape.h"

DefenseTower::DefenseTower(float x, float y, int archers)
    : GameObject(x, y), archers(archers) {
    circle_radius = DEFENSE_TOWER_RADIUS;
}

void DefenseTower::init() {

    data = new ColliderUserData();
    data->type = ColliderUserData::Type::DefenseTower;
    data->obj = get_shared_ptr();

    collider = ColliderFactory::newCircleCollider(
        data, x, y, circle_radius, b2_staticBody, CATEGORY_DEFENSE_TOWER,
        CATEGORY_ENEMY | CATEGORY_ENEMY_PROJECTILE, getContainer()->getWorld());

    // adding a sensor type fixture to detect enemies in tower range
    b2CircleShape sensor;
    sensor.m_radius = DEFENSE_TOWER_SENSOR_RADIUS / PIXEL_TO_METER_SCALE;

    b2FixtureDef fixtureDef;

    fixtureDef.shape = &sensor;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = CATEGORY_ENEMY;
    fixtureDef.filter.categoryBits = CATEGORY_DEFENSE_TOWER_SENSOR;

    collider->CreateFixture(&fixtureDef);
    healthBar =
        std::make_shared<ProgressBar>(x - 10, y - 70, 20, 3, maxHealth, health);
    Vector2 ss = getSpriteHolder()->getSpriteSize(TOWER_SPRITE_ID);
    float kr = 2.0f * DEFENSE_TOWER_RADIUS / ss.x;
    getContainer()->addGameObject(
        std::make_shared<Archer>(x, y - 260 * kr, get_shared_ptr()));
}

DefenseTower::~DefenseTower() {
    collider = nullptr;
    delete data;
}

void DefenseTower::hit(float damage) {
    isAttacked = true;
    health -= damage;
}

void DefenseTower::addEnemy(std::shared_ptr<GameObject> enemy) {
    enemies.push(enemy);
}

void DefenseTower::cleanupData() {
    data->obj = nullptr;
}

void DefenseTower::update(float dt) {
    if (!alive) {
        data->obj = nullptr;
        return;
    }
    collider->SetAwake(true);
    if (health <= 0) {
        die();
    }
    healthBar->setCurrent(health);
}

void DefenseTower::draw() {
    std::shared_ptr<SpriteHolder> spriteHolder = getSpriteHolder();
    Vector2 ss = spriteHolder->getSpriteSize(TOWER_SPRITE_ID);
    float kr = 2.0f * circle_radius / ss.x;
    getSpriteHolder()->drawSprite(
        TOWER_SPRITE_ID, {x - circle_radius, y - 300 * kr - 132 * kr * 0.5f,
                          2 * circle_radius, ss.y * kr});
    healthBar->draw();
}

bool DefenseTower::isAlive() {
    return alive;
}

void DefenseTower::die() {
    alive = false;
}
