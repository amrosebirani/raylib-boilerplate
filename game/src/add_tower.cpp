#include "add_tower.hpp"
#include "box2d/b2_body.h"
#include "collider_factory.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "globals.h"
#include "tower_spawn.hpp"
#include "utils.h"

AddTower::AddTower(float x, float y) : Collectible(x, y) {
    archers = 1;
    timer.after(
        randomFloatInRange(3.0f, 5.0f), [this](float dt) { this->die(); }, "");
}

AddTower::AddTower(float x, float y, TowerSpawnRing *ring)
    : Collectible(x, y), ring(ring) {
    archers = 1;
    timer.after(
        randomFloatInRange(3.0f, 5.0f), [this](float dt) { this->die(); }, "");
}

void AddTower::init() {
    data = new ColliderUserData();
    data->type = ColliderUserData::Collectable;
    data->obj = get_shared_ptr();
    circle_radius = 15;
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    collider = ColliderFactory::newCircleCollider(
        data, x, y, circle_radius, b2_dynamicBody, CATEGORY_COLLECTIBLE,
        CATEGORY_WARRIOR, world);
}

void AddTower::cleanupData() {
    data->obj = nullptr;
}

void AddTower::update(float dt) {
    if (!alive) {
        data->obj = nullptr;
        return;
    }
    timer.update(dt);
    collider->SetAwake(true);
}

bool AddTower::isAlive() {
    return alive;
}

void AddTower::die() {
    alive = false;
}

void AddTower::draw() {
    DrawCircleLines(x, y, circle_radius, WHITE);
    float offset = circle_radius * 1.0f / sqrt(2);
    getSpriteHolder()->drawSprite(
        BOW_SPRITE_ID, ADD_BOW_SPRITE_NO,
        {x - offset * 1.3f, y - offset * 1.6f, 2.5f * offset, 2.5f * offset});
}

void AddTower::collect() {
    die();
    // call the container to add a tower at this position
    getContainer()->initTower(x, y, archers, ring);
}

AddTower::~AddTower() {
    collider = nullptr;
    delete data;
}
