#include "archer.hpp"
#include "archer_state_params.hpp"
#include "raylib.h"
#include "states/archer/idle.hpp"
#include "states/archer/play_animation.hpp"

Archer::Archer(float rx, float ry, std::shared_ptr<GameObject> tower)
    : GameObject(rx, ry), tower(tower) {
    graphics_types.push_back("high_ground");
    raised = true;
    stateMachine = new StateMachine(
        {{"Idle", new IdleArcher()}, {"PlayAnimation", new PlayAnimation()}});
    stateMachine->changeState("Idle", new ArcherStateParams(this));
}

void Archer::init() {
}

Archer::~Archer() {
    delete stateMachine;
}

void Archer::draw() {
    stateMachine->draw();
}

void Archer::update(float dt) {
    if (!alive) {
        return;
    }
    if (!tower->isAlive()) {
        die();
        tower = nullptr;
        return;
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
}

void Archer::attack() {
    DrawText("Archer attacks!", 10, 10, 20, DARKGRAY);
}

void Archer::tryAttack(std::shared_ptr<GameObject> enemy) {
}

void Archer::takeAttack(float damage) {
}

void Archer::changeState(std::string stateName, StateParams *params) {
    stateMachine->changeState(stateName, params);
}

float Archer::getAttackRange() {
    return attackRange;
}
