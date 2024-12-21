#pragma once

#include "warrior_types.h"
#include "magic_types.hpp"

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
        SummonCard(int level, MagicType mtype,
                   SummonCardType stype = SummonCardType::WIZARDRY)
            : level(level), magic_type(mtype), summon_type(stype) {};
        ~SummonCard() {};

        int level;
        WarriorType type = WarriorType::WARRIOR_TYPE_AXEMAN;
        MagicType magic_type = MagicType::ICE_SPELL;
        SummonCardType summon_type;
};
