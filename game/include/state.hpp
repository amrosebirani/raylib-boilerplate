#pragma once

#include "state_params.hpp"
class State {
    public:
        virtual void draw() = 0;
        virtual void update(float dt) = 0;
        virtual void Enter(StateParams *params) = 0;
        virtual void Exit() = 0;
        virtual ~State() = 0;
};
inline State::~State() {
}
