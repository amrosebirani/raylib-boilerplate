#pragma once

#include "warrior_types.h"
#include "magic_types.hpp"
#include <fstream>

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
        SummonCard(std::ifstream &in) {

            in.read(reinterpret_cast<char *>(&level), sizeof(level));
            in.read(reinterpret_cast<char *>(&type), sizeof(type));
            in.read(reinterpret_cast<char *>(&magic_type), sizeof(magic_type));
            in.read(reinterpret_cast<char *>(&summon_type),
                    sizeof(summon_type));
        };
        ~SummonCard() {};

        int level;
        WarriorType type = WarriorType::WARRIOR_TYPE_AXEMAN;
        MagicType magic_type = MagicType::ICE_SPELL;
        SummonCardType summon_type;
        void save(std::ofstream &out) const {
            out.write(reinterpret_cast<const char *>(&level), sizeof(level));
            out.write(reinterpret_cast<const char *>(&type), sizeof(type));
            out.write(reinterpret_cast<const char *>(&magic_type),
                      sizeof(magic_type));
            out.write(reinterpret_cast<const char *>(&summon_type),
                      sizeof(summon_type));
        };
};
