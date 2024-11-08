#include "archer.hpp"
#include "archer_state_params.hpp"
#include "collider_user_data.h"
#include "raylib.h"
#include "states/archer/idle.hpp"
#include "states/archer/play_animation.hpp"
#include "warrior_types.h"
#include "collider_factory.hpp"
#include "constants.h"
#include "globals.h"
#include "utils.h"

Archer::Archer(float rx, float ry, std::shared_ptr<GameObject> tower,
               bool operational)
    : GameObject(rx, ry), tower(tower), isOperational(operational) {
    graphics_types.push_back("high_ground");
    if (tower != nullptr) {
        hasTower = true;
        raised = true;
    }
    hp = get_warrior_hp(WarriorType::WARRIOR_TYPE_ARCHER);
    stateMachine = new StateMachine(
        {{"Idle", new IdleArcher()}, {"PlayAnimation", new PlayAnimation()}});
    stateMachine->changeState("Idle", new ArcherStateParams(this));
    if (!hasTower) {
        sdata = new ColliderUserData();
        sdata->obj = get_shared_ptr();
        sdata->type = ColliderUserData::Type::ArcherSensor;
        enemySensor = ColliderFactory::newCircleSensor(
            sdata, x, y, 1.5 * DEFENSE_TOWER_SENSOR_RADIUS, b2_staticBody,
            CATEGORY_BUILDING_SENSOR, CATEGORY_ENEMY,
            getContainer()->getWorld());
        collider_data = new ColliderUserData();
        collider_data->obj = get_shared_ptr();
        collider = getWarriorCollider(WarriorType::WARRIOR_TYPE_ARCHER, x, y,
                                      collider_data, false);
        collider_data->type = ColliderUserData::Archer;
    }
}

void Archer::init() {
}

Archer::~Archer() {
    delete stateMachine;
    if (hasTower) {
        delete sdata;
        enemySensor = nullptr;
        delete collider_data;
        collider = nullptr;
    }
}

void Archer::draw() {
    stateMachine->draw();
}

void Archer::addEnemy(std::shared_ptr<GameObject> enemy) {
    enemies.push_back(enemy);
}

void Archer::update(float dt) {
    if (!alive) {
        collider_data->obj = nullptr;
        return;
    }
    if (hasTower) {
        if (!tower->isAlive()) {
            die();
            tower = nullptr;
            return;
        }
    }
    if (!hasTower) {
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
        x = collider->GetPosition().x * PIXEL_TO_METER_SCALE;
        y = collider->GetPosition().y * PIXEL_TO_METER_SCALE;
    }
    if (hp <= 0) {
        die();
    }
    // here we will check the queue from tower and attack a particuler enemy
    stateMachine->update(dt);
}

bool Archer::isAlive() {
    return alive;
}

void Archer::die() {
    alive = false;
}

void Archer::cleanupData() {
    if (hasTower) {
        sdata->obj = nullptr;
        collider_data->obj = nullptr;
    }
}

void Archer::attack() {
    DrawText("Archer attacks!", 10, 10, 20, DARKGRAY);
}

void Archer::tryAttack(std::shared_ptr<GameObject> enemy) {
}

void Archer::takeAttack(float damage) {
    hp -= damage;
    std::shared_ptr<ParticleSystem> bs = getBloodSplatter();
    bs->setPosition(x, y);
    bs->setPosition(x, y);
    bs->justEmit(10);
}

void Archer::changeState(std::string stateName, StateParams *params) {
    stateMachine->changeState(stateName, params);
}

float Archer::getAttackRange() {
    return attackRange;
}
