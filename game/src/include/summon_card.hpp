#pragma once

#include "warrior_types.h"

enum class SummonCardType {
    INFANTRY,
    ARCHERY,
    WIZARDRY,
};

class SummonCard {
    public:
        SummonCard(int level, WarriorType wtype,
                   SummonCardType stype = SummonCardType::INFANTRY)
            : level(level), type(wtype), summon_type(stype) {};
        ~SummonCard() {};

        int level;
        WarriorType type;
        SummonCardType summon_type;
};
