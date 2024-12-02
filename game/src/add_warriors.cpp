#include "add_warriors.hpp"
#include "box2d/b2_body.h"
#include "collider_factory.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "globals.h"
#include "raylib.h"
#include "utils.h"

AddWarriors::AddWarriors(float x, float y) : Collectible(x, y) {
    warriorCount = getRandomIntInRange(1, 4);
    timer->after(
        randomFloatInRange(1.5f, 2.8f), [this](float dt) { this->die(); }, "");
}

void AddWarriors::init() {
    data = new ColliderUserData();
    data->type = ColliderUserData::Collectable;
    data->obj = get_shared_ptr();
    const char *tt = TextFormat("+%d", warriorCount);
    int tm = MeasureText(tt, 14);
    circle_radius = tm / 2.0f + 5;
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    collider = ColliderFactory::newCircleCollider(
        data, x, y, circle_radius, b2_dynamicBody, CATEGORY_COLLECTIBLE,
        CATEGORY_WARRIOR, world);
}

void AddWarriors::update(float dt) {
    if (!alive) {
        data->obj = nullptr;
        return;
    }
    timer->update(dt);
    collider->SetAwake(true);
}

void AddWarriors::cleanupData() {
    data->obj = nullptr;
}

bool AddWarriors::isAlive() {
    return alive;
}

void AddWarriors::die() {
    alive = false;
}

void AddWarriors::draw() {
    DrawCircleLines(x, y, circle_radius, WHITE);
    const char *tt = TextFormat("+%d", warriorCount);
    DrawText(tt, x - MeasureText(tt, 14) / 2.0f, y - 7, 14, WHITE);
}

void AddWarriors::collect() {
    getContainer()->initAppend(warriorCount);
    die();
}

AddWarriors::~AddWarriors() {
    collider = nullptr;
    delete data;
}
