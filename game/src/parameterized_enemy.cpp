#include "parameterized_enemy.hpp"
#include "archer.hpp"
#include "building.hpp"
#include "enemy_types.h"
#include "constants.h"
#include "globals.h"
#include "horde_cine_exp.hpp"
#include "raylib.h"
#include "utils.h"
#include "raymath.h"
#include "enemy_state_params.hpp"
#include <memory>

void ParameterizedEnemy::setMarching(Vector2 breakoffPoint) {
    isMarching = true;
    this->breakoffPoint = breakoffPoint;
}

void ParameterizedEnemy::draw() {
    if (!alive) {
        return;
    }
    stateMachine->draw();
}

void ParameterizedEnemy::die() {
    alive = false;
    afterDie(type);
}

void ParameterizedEnemy::attack() {
}

ParameterizedEnemy::ParameterizedEnemy(float x, float y, EnemyType type)
    : Enemy(get_enemy_size(type), x, y), type(type) {
    graphics_types.push_back("enemy");
}

void ParameterizedEnemy::setCameraTarget(Vector2 tp) {
    isLead = true;
    showWave = true;
    cameraTriggerPoint = tp;
}

void ParameterizedEnemy::setTargetObj(std::shared_ptr<GameObject> targetOb) {
    this->targetObj = targetOb;
}

void ParameterizedEnemy::setTargetForAttack() {

    if (targetObj != nullptr && !targetObj->isAlive()) {
        targetObj = nullptr;
    }
    if (targetObj != nullptr) {
        std::shared_ptr<Building> bb =
            std::dynamic_pointer_cast<Building>(targetObj);
        if (bb && bb->level == 0) {
            targetObj = nullptr;
        }
    }
    if (targetObj == nullptr) {
        targetObj = getContainer()->getClosestAttackUnit({x, y});
    }
    // if (targetObj == nullptr) {
    // die();
    // return;
    // }
}

void ParameterizedEnemy::setDirectionOfMovement() {
    if (isMarching) {
        dirToMove = dirToMarch;
    } else {
        setTargetForAttack();
        if (targetObj == nullptr) {
            dirToMove = dirToMarch;
        } else {
            dirToMove = {targetObj->x - x, targetObj->y - y};
            // normalize direction to dirToMove
            dirToMove = Vector2Normalize(dirToMove);
        }
    }
    directionFacing = get_direction(dirToMove);
}

void ParameterizedEnemy::init() {
    center = getContainer()->region->getCenterCoordinates();
    dirToMarch = {center.x - breakoffPoint.x, center.y - breakoffPoint.y};
    dirToMarch = Vector2Normalize(dirToMarch);
    breakOffDist = Vector2DistanceSqr(center, breakoffPoint);
    if (isLead) {
        cameraTriggerDist = Vector2DistanceSqr(center, cameraTriggerPoint);
    }
    collider_data = new ColliderUserData();
    collider_data->obj = get_shared_ptr();
    collider = getEnemyCollider(type, x, y, collider_data);
    initStates(type);
    // normalize direction to dirToMove
    setDirectionOfMovement();
    stateMachine->changeState("Running", new EnemyStateParams(this, type));
    b2Vec2 linearVelocity = {dirToMove.x * mvspd / PIXEL_TO_METER_SCALE,
                             dirToMove.y * mvspd / PIXEL_TO_METER_SCALE};
    collider->SetLinearVelocity(linearVelocity);
    damage = get_enemy_damage(type);
    hp = get_enemy_hp(type);
    attackCooldown = get_enemy_attack_time(type);
    if (type == EnemyType::ZOMBIE_GIANT) {
        damage = 200;
        hp = 3000;
    }
}

ParameterizedEnemy::~ParameterizedEnemy() {
}

void ParameterizedEnemy::hasCrossedMarchingPoint() {
    if (!isMarching) {
        return;
    }

    double curDist = Vector2DistanceSqr(center, {x, y});
    if (curDist < breakOffDist) {
        isMarching = false;
        showWave = false;
    }
}

void ParameterizedEnemy::hasCrossedCameraTriggerPoint() {
    if (!isLead) {
        return;
    }

    double curDist = Vector2DistanceSqr(center, {x, y});
    if (curDist < cameraTriggerDist) {
        isLead = false;
        // getContainer()->cinematographer->addExperience(
        // std::make_shared<HordeCineExperience>(get_shared_ptr()));
    }
}

void ParameterizedEnemy::update(float dt) {
    if (!alive) {
        // collider_data->obj = nullptr;
        return;
    }
    updateCounter++;
    updateCounter = updateCounter % 4;
    if (updateCounter == 3) {
        hasCrossedMarchingPoint();
    }
    if (updateCounter == 1) {
        hasCrossedCameraTriggerPoint();
    }
    timer->update(dt);
    stateMachine->update(dt);
    x = collider->GetPosition().x * PIXEL_TO_METER_SCALE;
    y = collider->GetPosition().y * PIXEL_TO_METER_SCALE;
    // if (!targetObj->isAlive()) {
    //     targetObj = nullptr;
    //     setTargetForAttack();
    //     if (targetObj == nullptr) {
    //         die();
    //         return;
    //     }
    // }
    setDirectionOfMovement();
    b2Vec2 linearVelocity = {dirToMove.x * mvspd / PIXEL_TO_METER_SCALE,
                             dirToMove.y * mvspd / PIXEL_TO_METER_SCALE};
    collider->SetLinearVelocity(linearVelocity);
    if (canAttack == 0) {
        attackCooldownTracker += dt;
        if (attackCooldownTracker >= attackCooldown) {
            attackCooldownTracker -= attackCooldown;
            canAttack = 1;
        }
    }

    if (hp <= 0) {
        die();
        return;
    }
    stateUpdate(type);
    if (canAttack == 1 && contactAttackUnits.size() > 0) {
        std::shared_ptr<GameObject> cont = contactAttackUnits[0];
        std::shared_ptr<Building> bb =
            std::dynamic_pointer_cast<Building>(cont);
        if (bb) {
            buildingAttack(bb);
        } else {
            std::shared_ptr<Archer> aa =
                std::dynamic_pointer_cast<Archer>(cont);
            if (aa) {
                archerAttack(aa);
            } else {
                std::shared_ptr<Warrior> ww =
                    std::dynamic_pointer_cast<Warrior>(cont);
                if (ww) {
                    tryAttack(ww);
                }
            }
        }
    }
}
//
void ParameterizedEnemy::cleanupData() {
    collider_data->obj = nullptr;
    delete collider_data;
    collider = nullptr;
    delete stateMachine;
}

bool ParameterizedEnemy::isAlive() {
    return alive;
}

void ParameterizedEnemy::tryAttack(std::shared_ptr<GameObject> target) {
    if (canAttack == 1) {
        isAttacking = true;
        canAttack = -1;
        std::shared_ptr<Warrior> warrior =
            std::dynamic_pointer_cast<Warrior>(target);
        getAudioManager()->playRandomCombatSound();
        timer->after(
            0.2f,
            [this, warrior](float dt) {
                warrior->takeAttack(this->damage);
                this->canAttack = 0;
                this->attackCooldownTracker = 0.0f;
            },
            "");
    }
}

void ParameterizedEnemy::buildingAttack(std::shared_ptr<Building> building) {
    if (canAttack == 1) {
        isAttacking = true;
        canAttack = -1;
        timer->after(
            0.2f,
            [this, building](float dt) {
                building->takeDamage(this->damage);
                this->canAttack = 0;
                this->attackCooldownTracker = 0.0f;
            },
            "");
    }
}

void ParameterizedEnemy::archerAttack(std::shared_ptr<Archer> archer) {
    if (canAttack == 1) {
        isAttacking = true;
        canAttack = -1;
        timer->after(
            0.2f,
            [this, archer](float dt) {
                archer->takeAttack(this->damage);
                this->canAttack = 0;
                this->attackCooldownTracker = 0.0f;
            },
            "");
    }
}

void ParameterizedEnemy::towerAttack(std::shared_ptr<DefenseTower> tower) {
    if (canAttack) {
        isAttacking = true;
        canAttack = -1;
        timer->after(
            0.2f,
            [this, tower](float dt) {
                tower->hit(this->damage);
                this->canAttack = 0;
                this->attackCooldownTracker = 0.0f;
            },
            "");
    }
}

void ParameterizedEnemy::takeAttack(float damage) {
    hp -= damage;
    throwBlood();
}
