#include "spearman.h"
#include "collider_user_data.h"
#include "constants.h"
#include "warrior_types.h"
#include "utils.h"
#include "globals.h"
#include "warrior_state_params.hpp"

void Spearman::draw() {
    stateMachine->draw();
}

void Spearman::cleanupData() {
    collider_data->obj = nullptr;
}

void Spearman::update(float dt) {
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
    stateUpdate(WarriorType::WARRIOR_TYPE_SPEARMAN, dt);
}

bool Spearman::isAlive() {
    return alive;
}

void Spearman::die() {
    getContainer()->removeFormFixture(fixture);
    alive = false;
    afterDie(WarriorType::WARRIOR_TYPE_SPEARMAN);
}

void Spearman::attack() {
}

Spearman::Spearman(float rx, float ry)
    : Warrior(get_warrior_size(WarriorType::WARRIOR_TYPE_SPEARMAN), rx, ry,
              true) {
}

void Spearman::init() {
    WarriorType type = WarriorType::WARRIOR_TYPE_SPEARMAN;
    collider_data = new ColliderUserData();
    collider_data->obj = get_shared_ptr();
    collider = getWarriorCollider(type, x, y, collider_data);
    fixture = getContainer()->addFormFixture(
        getFormationFixtureDef(get_warrior_size(type), rel_x, rel_y));
    initStates(type);
    stateMachine->changeState("Idle", new WarriorStateParams(this, type));
    damage = get_warrior_damage(type);
    hp = get_warrior_hp(type);
    attackCooldown = get_warrior_attack_time(type);
}

Spearman::~Spearman() {
    delete collider_data;
    collider = nullptr;
}

void Spearman::takeAttack(float damage) {
    hp -= damage;
    throwBlood();
}
