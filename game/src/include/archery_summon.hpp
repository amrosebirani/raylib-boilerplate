
#pragma once

#include "summon_card.hpp"
#include <memory>
#include "raylib.h"

class ArcherySummon {
    public:
        ArcherySummon(Vector2 postion, std::shared_ptr<SummonCard> card);
        ~ArcherySummon();
};
