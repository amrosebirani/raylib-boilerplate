#include "warrior_para.hpp"
#include "constants.h"
#include "warrior_types.h"
#include "utils.h"
#include "globals.h"
#include "warrior_state_params.hpp"

void WarriorPara::draw() {
    if (!alive) {
        return;
    }
    stateMachine->draw();
}

void WarriorPara::cleanupData() {
    collider_data->obj = nullptr;
    if (!inFormation) {
        sensorData->obj = nullptr;
    }
}

void WarriorPara::update(float dt) {
    if (!alive) {
        collider_data->obj = nullptr;
        if (!inFormation) {
            sensorData->obj = nullptr;
        }
        return;
    }
    timer->update(dt);
    stateMachine->update(dt);
    if (inFormation) {

        collider->SetTransform(
            b2Vec2(x / PIXEL_TO_METER_SCALE, y / PIXEL_TO_METER_SCALE), 0);
    } else {
        x = collider->GetPosition().x * PIXEL_TO_METER_SCALE;
        y = collider->GetPosition().y * PIXEL_TO_METER_SCALE;

        b2Vec2 linearVelocity = {dirToMove.x * mvspd / PIXEL_TO_METER_SCALE,
                                 dirToMove.y * mvspd / PIXEL_TO_METER_SCALE};

        collider->SetLinearVelocity(linearVelocity);
        sensor->SetTransform(
            b2Vec2(x / PIXEL_TO_METER_SCALE, y / PIXEL_TO_METER_SCALE), 0);
    }
    collider->SetAwake(true);
    if (hp <= 0) {
        die();
        return;
    }
    stateUpdate(type, dt);
}

bool WarriorPara::isAlive() {
    return alive;
}

void WarriorPara::die() {
    if (inFormation) {
        std::shared_ptr<Container> cc = getContainer();
        if (cc != nullptr) getContainer()->removeFormFixture(fixture);
    }
    alive = false;
    afterDie(type);
}

void WarriorPara::attack() {
    DrawText("WarriorPara attacks!", 10, 10, 20, DARKGRAY);
}

WarriorPara::WarriorPara(float rx, float ry, WarriorType type, bool inFormation)
    : Warrior(get_warrior_size(type), rx, ry, inFormation), type(type) {
}

WarriorPara::WarriorPara(float x, float y, WarriorType type)
    : Warrior(get_warrior_size(type), x, y, 1), type(type) {
}

void WarriorPara::init() {
    collider_data = new ColliderUserData();
    collider_data->obj = get_shared_ptr();
    if (!inFormation) {

        sensorData = new ColliderUserData();
        sensorData->obj = get_shared_ptr();
        sensor = getWarriorSensor(type, x, y, sensorData);
    }
    collider = getWarriorCollider(type, x, y, collider_data, inFormation);
    if (inFormation) {
        fixture = getContainer()->addFormFixture(
            getFormationFixtureDef(get_warrior_size(type), rel_x, rel_y));
    }
    initStates(type);
    stateMachine->changeState("Idle", new WarriorStateParams(this, type));
    damage = get_warrior_damage(type);
    hp = get_warrior_hp(type);
    attackCooldown = get_warrior_attack_time(type);
    in_damage_mult = get_in_damage_multiplier(type);
}

WarriorPara::~WarriorPara() {
    delete collider_data;
    collider = nullptr;
    if (!inFormation) {
        delete sensorData;
        sensor = nullptr;
    }
}

void WarriorPara::takeAttack(float damage) {
    if (type == WarriorType::WARRIOR_TYPE_KNIGHT) {
        if (getContainer()->getUpgradeContent()->get_chance(
                HERO_DODGE_CHANCE_M)) {
            return;
        }
    }
    if (inFormation) {
        if (getContainer()->getUpgradeContent()->get_chance(
                HERO_UNIT_DODGE_CHANCE_M)) {
            return;
        }
    }
    if (!inFormation) {
        if (getContainer()->getUpgradeContent()->get_chance(
                INFANTRY_DODGE_CHANCE_M)) {
            return;
        }
    }
    hp -= damage * in_damage_mult;
    throwBlood();
}
