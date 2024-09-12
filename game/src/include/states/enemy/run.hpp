#pragma once

#include "state.hpp"
#include "state_params.hpp"
#include "animation.hpp"
#include "enemy_state_params.hpp"

class RunningEnemy : public State {
    public:
        RunningEnemy();
        ~RunningEnemy();
        void draw() override;
        void update(float dt) override;
        void Enter(StateParams *params) override;
        void Exit() override;

    private:
        Animation *animation;
        EnemyStateParams *enemyParams;
};
