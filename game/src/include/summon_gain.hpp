#pragma once

#include "room.hpp"
#include "raylib.h"
#include "timer.h"
#include "summon_card.hpp"

class SummonGain : public Room {
    public:
        SummonGain(Vector2 worldPos, Vector2 dim,
                   std::shared_ptr<SummonCard> card);
        bool update(float dt) override;
        void draw() override;
        bool isFinished() override;

    private:
        Timer timer;
        float x;
        float y;
        std::shared_ptr<std::unordered_map<std::string, float>> params;
        bool finished = false;
        std::shared_ptr<SummonCard> card;
        Vector2 dim;
        std::string sprite_id;
};
