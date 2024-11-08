#pragma once

#include "state.hpp"
#include "state_params.hpp"
#include "animation.hpp"
#include "enemy_state_params.hpp"

class CooldownEnemy : public State {
    public:
        CooldownEnemy();
        ~CooldownEnemy();
        void draw() override;
        void update(float dt) override;
        void Enter(StateParams *params) override;
        void Exit() override;

    private:
        Animation *animation = nullptr;
        EnemyStateParams *enemyParams = nullptr;
};
