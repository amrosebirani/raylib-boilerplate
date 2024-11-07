#include "shield_bearer.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "globals.h"
#include "raylib.h"
#include "warrior_types.h"
#include "utils.h"
#include "warrior_state_params.hpp"

void ShieldBearer::draw() {
    // DrawCircle(x, y,
    // get_warrior_size(WarriorType::WARRIOR_TYPE_SHIELD_BEARER), WHITE);
    stateMachine->draw();
}

void ShieldBearer::cleanupData() {
    collider_data->obj = nullptr;
}

void ShieldBearer::update(float dt) {
    if (!alive) {
        collider_data->obj = nullptr;
        return;
    }
    timer.update(dt);
    stateMachine->update(dt);
    collider->SetTransform(
        b2Vec2(x / PIXEL_TO_METER_SCALE, y / PIXEL_TO_METER_SCALE), 0);
    collider->SetAwake(true);
    if (hp <= 0) {
        die();
    }
    stateUpdate(WarriorType::WARRIOR_TYPE_SHIELD_BEARER, dt);
}

bool ShieldBearer::isAlive() {
    return alive;
}

void ShieldBearer::die() {
    getContainer()->removeFormFixture(fixture);
    alive = false;
    afterDie(WarriorType::WARRIOR_TYPE_SHIELD_BEARER);
}

void ShieldBearer::attack() {
    DrawText("ShieldBearer attacks!", 10, 10, 20, DARKGRAY);
}

ShieldBearer::ShieldBearer(float rx, float ry)
    : Warrior(get_warrior_size(WarriorType::WARRIOR_TYPE_SHIELD_BEARER), rx, ry,
              true) {};

void ShieldBearer::init() {

    WarriorType type = WarriorType::WARRIOR_TYPE_SHIELD_BEARER;
    graphics_types.push_back("warrior");
    collider_data = new ColliderUserData();
    collider_data->obj = get_shared_ptr();
    collider = getWarriorCollider(type, x, y, collider_data);
    fixture = getContainer()->addFormFixture(
        getFormationFixtureDef(get_warrior_size(type), rel_x, rel_y));
    initStates(type);
    stateMachine->changeState(
        "Idle",
        new WarriorStateParams(this, WarriorType::WARRIOR_TYPE_SHIELD_BEARER));
    damage = get_warrior_damage(type);
    hp = get_warrior_hp(type);
    attackCooldown = get_warrior_attack_time(type);
}

ShieldBearer::~ShieldBearer() {
    delete collider_data;
    collider = nullptr;
    // delete fixture;
}

void ShieldBearer::takeAttack(float damage) {
    hp -= damage / 2;
    throwBlood();
}
