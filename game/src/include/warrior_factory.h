#pragma once

#include "warrior.hpp"
#include "warrior_types.h"

class WarriorFactory {
    public:
        static std::shared_ptr<Warrior> createWarrior(WarriorType type,
                                                      float rel_x, float rel_y);
};
