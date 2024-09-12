#include "swordsman.h"
#include "constants.h"
#include "warrior_types.h"
#include "utils.h"
#include "enemy.hpp"
#include "globals.h"
#include "warrior_state_params.hpp"

void Swordsman::draw() {
    stateMachine->draw();
}

void Swordsman::cleanupData() {
    collider_data->obj = nullptr;
}

void Swordsman::update(float dt) {
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
    stateUpdate(WarriorType::WARRIOR_TYPE_SWORDSMAN);
}

bool Swordsman::isAlive() {
    return alive;
}

void Swordsman::die() {
    getContainer()->removeFormFixture(fixture);
    alive = false;
    afterDie(WarriorType::WARRIOR_TYPE_SWORDSMAN);
}

void Swordsman::attack() {
}

Swordsman::Swordsman(float rx, float ry)
    : Warrior(get_warrior_size(WarriorType::WARRIOR_TYPE_SWORDSMAN), rx, ry) {
}

void Swordsman::init() {
    WarriorType type = WarriorType::WARRIOR_TYPE_SWORDSMAN;
    collider_data = new ColliderUserData();
    collider_data->obj = get_shared_ptr();
    collider = getWarriorCollider(type, x, y, collider_data);
    fixture = getContainer()->addFormFixture(
        getFormationFixtureDef(get_warrior_size(type), rel_x, rel_y));
    initStates(type);
    stateMachine->changeState("Idle", new WarriorStateParams(this, type));
    damage = 100;
}

Swordsman::~Swordsman() {
    delete collider_data;
    collider = nullptr;
}

void Swordsman::takeAttack(float damage) {
    hp -= damage;
    throwBlood();
}

void Swordsman::tryAttack(std::shared_ptr<GameObject> target) {
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
