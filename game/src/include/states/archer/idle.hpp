#pragma once
#include "state.hpp"
#include "state_params.hpp"
#include "animation.hpp"
#include "archer_state_params.hpp"

class IdleArcher : public State {
    public:
        IdleArcher();
        ~IdleArcher();
        void draw() override;
        void update(float dt) override;
        void Enter(StateParams *params) override;
        void Exit() override;

    private:
        Animation *animation = nullptr;
        ArcherStateParams *archerParams = nullptr;
        bool canAttack = true;
        float attackCooldown = 1.0f;
        float attackCooldownTracker = 0.0f;
};
