#pragma once

#include "summon_card.hpp"
#include <memory>
#include "raylib.h"

class WizardrySummon {
    public:
        WizardrySummon(Vector2 postion, std::shared_ptr<SummonCard> card);
        ~WizardrySummon();
};
