#pragma once

#include "state_params.hpp"
#include "warrior.hpp"
#include "warrior_types.h"

struct WarriorStateParams : public StateParams {
        Warrior *warrior;
        float enemy_x;
        float enemy_y;
        WarriorType type;

        WarriorStateParams(Warrior *warrior, WarriorType type) {
            this->warrior = warrior;
            this->type = type;
        }
};
