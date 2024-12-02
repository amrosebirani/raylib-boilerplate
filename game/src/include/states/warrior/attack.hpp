#pragma once

#include "state.hpp"
#include "state_params.hpp"
#include "animation.hpp"
#include "warrior_state_params.hpp"

class AttackingWarrior : public State {
    public:
        AttackingWarrior();
        ~AttackingWarrior();
        void draw() override;
        void update(float dt) override;
        void Enter(StateParams *params) override;
        void Exit() override;

    private:
        Animation *animation = nullptr;
        WarriorStateParams *warriorParams = nullptr;
};
