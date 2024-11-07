#pragma once

#include <queue>
#include <memory>
#include "summon_card.hpp"

class SummonManager {
    public:
        SummonManager();
        ~SummonManager();
        void update(float dt);
        void draw();
        void addSummonCard(std::shared_ptr<SummonCard> card);

    private:
        bool summonActivated = true;
        float summonRT = 0.2f;
        float summonTimer = 0;
        std::queue<std::shared_ptr<SummonCard>> summonCards;
};
