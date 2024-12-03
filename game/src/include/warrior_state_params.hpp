#pragma once

#include "game_object.h"
#include "state_params.hpp"
#include "warrior_types.h"

struct WarriorStateParams : public StateParams {
        GameObject *warrior;
        float enemy_x;
        float enemy_y;
        WarriorType type;

        WarriorStateParams(GameObject *warrior, WarriorType type) {
            this->warrior = warrior;
            this->type = type;
        }
};
