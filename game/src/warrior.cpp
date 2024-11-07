#include "warrior.hpp"
#include "dead_body.h"
#include "enemy.hpp"
#include "enemy_types.h"
#include "globals.h"
#include "states/warrior/idle.hpp"
#include "states/warrior/run.hpp"
#include "states/warrior/attack.hpp"
#include "states/warrior/seeking.hpp"
#include "utils.h"
#include "raymath.h"

Warrior::Warrior(float radius, float x, float y, int i)
    : radius(radius), GameObject(x, y), inFormation(false) {
    // set direction facing here
    Vector2 cc = getContainer()->region->getCenterCoordinates();
    Vector2 dirDepth = Vector2Subtract({x, y}, cc);
    dirDepth = Vector2Normalize(dirDepth);
    directionFacing = get_direction(dirDepth);
}

void Warrior::initStates(WarriorType type) {
    stateMachine = new StateMachine({
        {"Idle", new IdleWarrior()},
        {"Running", new RunningWarrior()},
        {"Attacking", new AttackingWarrior()},
        {"Seeking", new SeekingWarrior()},
    });
}

void Warrior::doAttacking(float dt) {
    if (canAttack == 0) {
        attackCooldownTracker += dt;
        if (attackCooldownTracker >= attackCooldown) {
            attackCooldownTracker -= attackCooldown;
            canAttack = 1;
        }
    }
    if (canAttack == 1 && contactAttackUnits.size() > 0) {
        std::shared_ptr<GameObject> cont = contactAttackUnits[0];
        tryAttack(cont);
    }
}

void Warrior::stateUpdate(WarriorType type, float dt) {

    manageContactAttackUnits();
    if (!inFormation) {
        manageInRangeEnemyUnits();
    }
    doAttacking(dt);

    if (inFormation) {
        if (isAttacking) {
            if (stateMachine->getCurrentStateName() != "Attacking") {
                stateMachine->changeState("Attacking",
                                          new WarriorStateParams(this, type));
            }

        } else {
            if (isIdle) {
                if (stateMachine->getCurrentStateName() != "Idle") {
                    stateMachine->changeState(
                        "Idle", new WarriorStateParams(this, type));
                }
            } else {
                if (stateMachine->getCurrentStateName() != "Running" ||
                    previousDirectionFacing != directionFacing) {
                    stateMachine->changeState(
                        "Running", new WarriorStateParams(this, type));
                }
            }
        }
    } else {
        if (isAttacking) {
            if (stateMachine->getCurrentStateName() != "Attacking") {
                stateMachine->changeState("Attacking",
                                          new WarriorStateParams(this, type));
            }

        } else {
            if (inRangeEnemyUnits.size() > 0) {
                if (stateMachine->getCurrentStateName() != "Seeking") {
                    stateMachine->changeState(
                        "Seeking", new WarriorStateParams(this, type));
                }
            } else {
                if (stateMachine->getCurrentStateName() != "Idle") {
                    stateMachine->changeState(
                        "Idle", new WarriorStateParams(this, type));
                }
            }
        }
    }
    previousDirectionFacing = directionFacing;
}

void Warrior::tryAttack(std::shared_ptr<GameObject> target) {
    if (canAttack == 1) {
        isAttacking = true;
        canAttack = -1;
        std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(target);
        timer.after(
            0.3f,
            [this, enemy](float dt) {
                if (!enemy->isAlive()) {
                    return;
                }
                enemy->takeAttack(this->damage);
                this->canAttack = 0;
                this->attackCooldownTracker = 0;
            },
            "");
    }
}

void Warrior::afterDie(WarriorType type) {
    std::shared_ptr<DeadBody> db = std::make_shared<DeadBody>(
        x, y, true, EnemyType::ENEMY_TYPE_SHIELD_BEARER, type);
    getContainer()->addScumObject(db);
}

void Warrior::throwBlood() {

    std::shared_ptr<ParticleSystem> bs = getBloodSplatter();
    bs->setPosition(x, y);
    bs->setPosition(x, y);
    bs->justEmit(10);
}

void Warrior::addContactAttack(std::shared_ptr<GameObject> cont) {
    contactAttackUnits.push_back(cont);
}

void Warrior::removeContactAttack(std::shared_ptr<GameObject> cont) {
    auto it =
        std::find(contactAttackUnits.begin(), contactAttackUnits.end(), cont);
    if (it != contactAttackUnits.end()) {
        contactAttackUnits.erase(it);
    }
}

void Warrior::addInRangeEnemyUnit(std::shared_ptr<GameObject> enemyUnit) {
    inRangeEnemyUnits.push_back(enemyUnit);
}

void Warrior::removeInRangeEnemyUnit(std::shared_ptr<GameObject> enemyUnit) {
    auto it = std::find(inRangeEnemyUnits.begin(), inRangeEnemyUnits.end(),
                        enemyUnit);
    if (it != inRangeEnemyUnits.end()) {
        inRangeEnemyUnits.erase(it);
    }
}

void Warrior::manageContactAttackUnits() {
    std::vector<size_t> indicesR;
    for (size_t i = 0; i < contactAttackUnits.size(); i++) {
        std::shared_ptr<GameObject> go = contactAttackUnits[i];
        if (!go->isAlive()) {
            indicesR.push_back(i);
        }
    }
    for (size_t i = indicesR.size(); i > 0; --i) {
        size_t index = indicesR[i - 1];
        contactAttackUnits.erase(contactAttackUnits.begin() + index);
    }
}

void Warrior::manageInRangeEnemyUnits() {

    std::vector<size_t> toRemove;
    for (size_t i = 0; i < inRangeEnemyUnits.size(); i++) {
        if (!inRangeEnemyUnits[i]->isAlive()) {
            toRemove.push_back(i);
        }
    }
    for (size_t i = toRemove.size(); i > 0; i--) {
        inRangeEnemyUnits.erase(inRangeEnemyUnits.begin() + toRemove[i - 1]);
    }
}
