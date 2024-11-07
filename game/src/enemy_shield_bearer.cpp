#include "enemy_shield_bearer.hpp"
#include "building.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "defense_tower.hpp"
#include "enemy_types.h"
#include "globals.h"
#include "raylib.h"
#include "warrior_types.h"
#include "utils.h"
#include "raymath.h"
#include "enemy_state_params.hpp"

void EnemyShieldBearer::draw() {
    // DrawCircle(x, y, get_enemy_size(EnemyType::ENEMY_TYPE_SHIELD_BEARER),
    // WHITE);
    stateMachine->draw();
}

void EnemyShieldBearer::die() {
    alive = false;
    afterDie(EnemyType::ENEMY_TYPE_SHIELD_BEARER);
}

void EnemyShieldBearer::attack() {
    DrawText("ShieldBearer attacks!", 10, 10, 20, DARKGRAY);
}

EnemyShieldBearer::EnemyShieldBearer(float x, float y)
    : Enemy(get_warrior_size(WarriorType::WARRIOR_TYPE_SHIELD_BEARER), x, y) {
    graphics_types.push_back("enemy");
}

void EnemyShieldBearer::init() {
    EnemyType type = EnemyType::ENEMY_TYPE_SHIELD_BEARER;
    collider_data = new ColliderUserData();
    collider_data->obj = get_shared_ptr();
    collider = getEnemyCollider(type, x, y, collider_data);
    initStates(type);
    targetObj = getContainer()->getClosestAttackUnit({x, y});
    if (targetObj == nullptr) {
        die();
        return;
    }
    Vector2 dirToMove = {targetObj->x - x, targetObj->y - y};
    // normalize direction to dirToMove
    dirToMove = Vector2Normalize(dirToMove);
    directionFacing = get_direction(dirToMove);
    stateMachine->changeState("Running", new EnemyStateParams(this, type));
    b2Vec2 linearVelocity = {dirToMove.x * mvspd / PIXEL_TO_METER_SCALE,
                             dirToMove.y * mvspd / PIXEL_TO_METER_SCALE};
    collider->SetLinearVelocity(linearVelocity);
    damage = 50;
}

EnemyShieldBearer::~EnemyShieldBearer() {
    delete collider_data;
    collider = nullptr;
}

void EnemyShieldBearer::update(float dt) {
    if (!alive) {

        collider_data->obj = nullptr;
        return;
    }
    timer.update(dt);
    stateMachine->update(dt);
    x = collider->GetPosition().x * PIXEL_TO_METER_SCALE;
    y = collider->GetPosition().y * PIXEL_TO_METER_SCALE;
    if (!targetObj->isAlive()) {
        targetObj = getContainer()->getClosestAttackUnit({x, y});
        if (targetObj == nullptr) {
            die();
            return;
        }
    }
    Vector2 dirToMove = {targetObj->x - x, targetObj->y - y};
    // normalize direction to dirToMove
    dirToMove = Vector2Normalize(dirToMove);
    directionFacing = get_direction(dirToMove);
    b2Vec2 linearVelocity = {dirToMove.x * mvspd / PIXEL_TO_METER_SCALE,
                             dirToMove.y * mvspd / PIXEL_TO_METER_SCALE};
    collider->SetLinearVelocity(linearVelocity);
    if (!canAttack) {
        attackCooldownTracker += dt;
        if (attackCooldownTracker >= attackCooldown) {
            canAttack = true;
        }
    }
    if (hp <= 0) {
        die();
    }
    stateUpdate(EnemyType::ENEMY_TYPE_SHIELD_BEARER);
}

bool EnemyShieldBearer::isAlive() {
    return alive;
}

void EnemyShieldBearer::tryAttack(std::shared_ptr<GameObject> target) {
    if (canAttack) {
        isAttacking = true;
        std::shared_ptr<Warrior> warrior =
            std::dynamic_pointer_cast<Warrior>(target);
        timer.after(
            0.3f,
            [this, warrior](float dt) {
                warrior->takeAttack(this->damage);
                this->canAttack = false;
                this->attackCooldownTracker = 0.0f;
            },
            "");
    }
}

void EnemyShieldBearer::buildingAttack(std::shared_ptr<Building> building) {
    if (canAttack) {
        isAttacking = true;
        timer.after(
            0.3f,
            [this, building](float dt) {
                building->takeDamage(this->damage);
                this->canAttack = false;
                this->attackCooldownTracker = 0.0f;
            },
            "");
    }
}

void EnemyShieldBearer::cleanupData() {
    collider_data->obj = nullptr;
}

void EnemyShieldBearer::towerAttack(std::shared_ptr<DefenseTower> tower) {
    if (canAttack) {
        isAttacking = true;
        timer.after(
            0.3f,
            [this, tower](float dt) {
                tower->hit(this->damage);
                this->canAttack = false;
                this->attackCooldownTracker = 0.0f;
            },
            "");
    }
}

void EnemyShieldBearer::takeAttack(float damage) {
    hp -= damage;
    throwBlood();
}
