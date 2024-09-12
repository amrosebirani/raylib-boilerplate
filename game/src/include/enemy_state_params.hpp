#pragma once

#include "state_params.hpp"
#include "enemy.hpp"
#include "enemy_types.h"

struct EnemyStateParams : public StateParams {
        Enemy *enemy;
        float enemy_x;
        float enemy_y;
        EnemyType type;

        EnemyStateParams(Enemy *enemy, EnemyType type) {
            this->enemy = enemy;
            this->type = type;
        }
};
