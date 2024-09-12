#include "enemy.hpp"
#include "states/enemy/run.hpp"
#include "states/enemy/attack.hpp"
#include "enemy_state_params.hpp"
#include "dead_body.h"
#include "globals.h"

void Enemy::initStates(EnemyType type) {
    stateMachine = new StateMachine({
        {"Running", new RunningEnemy()},
        {"Attacking", new AttackingEnemy()},
    });
}

void Enemy::stateUpdate(EnemyType type) {

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
}

void Enemy::throwBlood() {

    std::shared_ptr<ParticleSystem> bs = getEnemyBloodSplatter();
    bs->setPosition(x, y);
    bs->setPosition(x, y);
    bs->justEmit(10);
}
