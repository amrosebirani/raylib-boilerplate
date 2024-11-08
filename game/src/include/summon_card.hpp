#pragma once

#include "warrior_types.h"

enum class SummonCardType {
    INFANTRY,
    ARCHERY,
    WIZARDRY,
};

class SummonCard {
    public:
        SummonCard(int w, int d, std::vector<WarriorType> types,
                   SummonCardType type = SummonCardType::INFANTRY)
            : w(w), d(d), types(types), summon_type(type) {};
        ~SummonCard() {};

        int w;
        int d;
        std::vector<WarriorType> types;
        SummonCardType summon_type;
};
