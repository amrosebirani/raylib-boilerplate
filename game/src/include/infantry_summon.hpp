#pragma once

#include "summon_card.hpp"
#include <memory>
#include "raylib.h"

class InfantrySummon {
    public:
        InfantrySummon(Vector2 postion, std::shared_ptr<SummonCard> card);
        ~InfantrySummon();
};
