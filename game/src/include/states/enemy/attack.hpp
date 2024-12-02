#pragma once

#include "state.hpp"
#include "state_params.hpp"
#include "animation.hpp"
#include "enemy_state_params.hpp"

class AttackingEnemy : public State {
    public:
        AttackingEnemy();
        ~AttackingEnemy();
        void draw() override;
        void update(float dt) override;
        void Enter(StateParams *params) override;
        void Exit() override;

    private:
        Animation *animation = nullptr;
        EnemyStateParams *enemyParams = nullptr;
        bool canAttack = true;
        float attackCooldown = 1.0f;
        float attackCooldownTracker = 0.0f;
};
