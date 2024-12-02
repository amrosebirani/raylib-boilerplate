#include "state_machine.hpp"

StateMachine::StateMachine(std::unordered_map<std::string, State *> states) {
    this->states = states;
    this->currentState = nullptr;
}

StateMachine::~StateMachine() {
    for (auto &state : this->states) {
        delete state.second;
    }
    states.clear();
}

void StateMachine::changeState(std::string stateName, StateParams *params) {
    if (this->currentState != nullptr) {
        this->currentState->Exit();
    }
    this->currentState = this->states[stateName];
    this->currentState->Enter(params);
    this->currentStateName = stateName;
}

void StateMachine::update(float dt) {
    if (this->currentState != nullptr) {
        this->currentState->update(dt);
    }
}

void StateMachine::draw() {
    if (this->currentState != nullptr) {
        this->currentState->draw();
    }
}

std::string StateMachine::getCurrentStateName() {
    return this->currentStateName;
}
