#pragma once

#include "state.hpp"
#include <unordered_map>
#include <string>

class StateMachine {
    public:
        StateMachine(std::unordered_map<std::string, State *> states);
        ~StateMachine();
        void changeState(std::string stateName, StateParams *params);
        void update(float dt);
        void draw();
        std::string getCurrentStateName();

    private:
        std::unordered_map<std::string, State *> states;
        State *currentState;
        std::string currentStateName;
};
