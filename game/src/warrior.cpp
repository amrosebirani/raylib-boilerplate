#include "warrior.hpp"
#include "dead_body.h"
#include "enemy_types.h"
#include "globals.h"
#include "states/warrior/idle.hpp"
#include "states/warrior/run.hpp"
#include "states/warrior/attack.hpp"

void Warrior::initStates(WarriorType type) {
    stateMachine = new StateMachine({
        {"Idle", new IdleWarrior()},
        {"Running", new RunningWarrior()},
        {"Attacking", new AttackingWarrior()},
    });
}

void Warrior::stateUpdate(WarriorType type) {

    if (isAttacking) {
        if (stateMachine->getCurrentStateName() != "Attacking") {
            stateMachine->changeState("Attacking",
                                      new WarriorStateParams(this, type));
        }

    } else {
        if (isIdle) {
            if (stateMachine->getCurrentStateName() != "Idle") {
                stateMachine->changeState("Idle",
                                          new WarriorStateParams(this, type));
            }
        } else {
            if (stateMachine->getCurrentStateName() != "Running" ||
                previousDirectionFacing != directionFacing) {
                stateMachine->changeState("Running",
                                          new WarriorStateParams(this, type));
            }
        }
    }
    previousDirectionFacing = directionFacing;
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
