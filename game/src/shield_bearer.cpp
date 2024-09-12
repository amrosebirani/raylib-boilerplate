#include "shield_bearer.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "globals.h"
#include "raylib.h"
#include "warrior_types.h"
#include "utils.h"
#include "warrior_state_params.hpp"
#include "enemy.hpp"

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
    if (!canAttack) {
        attackCooldownTracker += dt;
        if (attackCooldownTracker >= attackCooldown) {
            canAttack = true;
        }
    }
    stateUpdate(WarriorType::WARRIOR_TYPE_SHIELD_BEARER);
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
    : Warrior(get_warrior_size(WarriorType::WARRIOR_TYPE_SHIELD_BEARER), rx,
              ry) {};

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
    damage = 60;
}

ShieldBearer::~ShieldBearer() {
    delete collider_data;
    collider = nullptr;
    // delete fixture;
}

void ShieldBearer::takeAttack(float damage) {
    hp -= damage;
    throwBlood();
}

void ShieldBearer::tryAttack(std::shared_ptr<GameObject> target) {
    if (canAttack) {
        isAttacking = true;
        std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(target);
        timer.after(
            0.3f,
            [this, enemy](float dt) {
                if (!enemy->isAlive()) {
                    return;
                }
                enemy->takeAttack(this->damage);
                this->canAttack = false;
                this->attackCooldownTracker = 0;
            },
            "");
    }
}
