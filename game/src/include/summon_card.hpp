#pragma once

#include "warrior_types.h"
class SummonCard {
    public:
        SummonCard(int w, int d, std::vector<WarriorType> types)
            : w(w), d(d), types(types) {};
        ~SummonCard() {};

        int w;
        int d;
        std::vector<WarriorType> types;
};
