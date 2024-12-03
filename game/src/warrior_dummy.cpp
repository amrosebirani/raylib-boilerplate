#include "warrior_dummy.hpp"
#include "globals.h"
#include "raymath.h"
#include "state_machine.hpp"
#include "states/warrior/idle.hpp"
#include "states/warrior/run.hpp"
#include "utils.h"

WarriorDummy::~WarriorDummy() {
    delete stateMachine;
}

void WarriorDummy::init() {
    Vector2 cc = getContainer()->region->getCenterCoordinates();
    Vector2 dirDepth = Vector2Subtract({x, y}, cc);
    dirDepth = Vector2Normalize(dirDepth);
    directionFacing = get_direction(dirDepth);
    stateMachine = new StateMachine({
        {"Idle", new IdleWarrior()},
        {"Running", new RunningWarrior()},
    });
    stateMachine->changeState("Idle", new WarriorStateParams(this, type));
}

void WarriorDummy::stateUpdate(float dt) {

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

void WarriorDummy::draw() {
    if (!alive) {
        return;
    }
    stateMachine->draw();
}

void WarriorDummy::cleanupData() {
}

void WarriorDummy::update(float dt) {
    if (!alive) {
        return;
    }
    stateMachine->update(dt);
    stateUpdate(dt);
}

bool WarriorDummy::isAlive() {
    return alive;
}

void WarriorDummy::die() {
    alive = false;
}
