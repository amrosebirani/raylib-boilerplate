#include "enemy.hpp"
#include "states/enemy/run.hpp"
#include "states/enemy/attack.hpp"
#include "enemy_state_params.hpp"
#include "dead_body.h"
#include "globals.h"
#include <memory>

void Enemy::initStates(EnemyType type) {
    stateMachine = new StateMachine({
        {"Running", new RunningEnemy()},
        {"Attacking", new AttackingEnemy()},
    });
}

void Enemy::stateUpdate(EnemyType type) {
    manageContactAttackUnits();

    if (isAttacking) {
        if (stateMachine->getCurrentStateName() != "Attacking") {
            stateMachine->changeState("Attacking",
                                      new EnemyStateParams(this, type));
        }

    } else {
        if (stateMachine->getCurrentStateName() != "Running" ||
            previousDirectionFacing != directionFacing) {
            stateMachine->changeState("Running",
                                      new EnemyStateParams(this, type));
        }
    }
    previousDirectionFacing = directionFacing;
}

void Enemy::afterDie(EnemyType type) {
    std::shared_ptr<DeadBody> db = std::make_shared<DeadBody>(
        x, y, false, type, WarriorType::WARRIOR_TYPE_SPEARMAN);
    getContainer()->addScumObject(db);
    getContainer()->hmm->decreaseCount();
}

void Enemy::addContactAttack(std::shared_ptr<GameObject> cont) {
    contactAttackUnits.push_back(cont);
}

void Enemy::removeContactAttack(std::shared_ptr<GameObject> cont) {
    auto it =
        std::find(contactAttackUnits.begin(), contactAttackUnits.end(), cont);
    if (it != contactAttackUnits.end()) {
        contactAttackUnits.erase(it);
    }
}

void Enemy::manageContactAttackUnits() {
    std::vector<size_t> indicesR;
    for (size_t i = 0; i < contactAttackUnits.size(); i++) {
        std::shared_ptr<GameObject> go = contactAttackUnits[i];
        if (!go->isAlive()) {
            indicesR.push_back(i);
        } else {

            std::shared_ptr<Building> bb =
                std::dynamic_pointer_cast<Building>(go);
            if (bb && bb->level == 0) {
                indicesR.push_back(i);
            }
        }
    }
    for (size_t i = indicesR.size(); i > 0; --i) {
        size_t index = indicesR[i - 1];
        contactAttackUnits.erase(contactAttackUnits.begin() + index);
    }
}

void Enemy::throwBlood() {

    std::shared_ptr<ParticleSystem> bs = getEnemyBloodSplatter();
    bs->setPosition(x, y);
    bs->setPosition(x, y);
    bs->justEmit(10);
}
