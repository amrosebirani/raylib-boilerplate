#include "summon_gain.hpp"
#include "globals.h"
#include "raylib.h"

SummonGain::SummonGain(Vector2 worldPos, Vector2 dim,
                       std::shared_ptr<SummonCard> card)
    : card(card), dim(dim) {
    timer = Timer();
    Vector2 screenPos = getViewCamera()->toCameraCoords(worldPos.x, worldPos.y);
    params = std::make_shared<std::unordered_map<std::string, float>>();
    (*params)["x"] = screenPos.x;
    (*params)["y"] = screenPos.y;
    timer.tween(2, this->params, {{"x", 0}, {"y", GetScreenHeight() / 1.5f}},
                "in-out-cubic",
                [this]() {
                    this->finished = true;
                    getWorldState()->summon_manager->addSummonCard(this->card);
                },
                "", {});
    switch (card->summon_type) {
    case SummonCardType::INFANTRY:
        sprite_id = INFANTRY_ICON;
        break;
    case SummonCardType::ARCHERY:
        sprite_id = ARCHERY_ICON;
        break;
    case SummonCardType::WIZARDRY:
        sprite_id = WIZARDRY_ICON;
        break;
    }
}

bool SummonGain::update(float dt) {
    timer.update(dt);
    return true;
}

void SummonGain::draw() {
    float scale = getViewCamera()->getScale();
    Rectangle rect = {(*params)["x"], (*params)["y"], scale * dim.x,
                      scale * dim.y};
    DrawRectangleRounded(rect, .5, 50, PURPLE);
    DrawRectangleRoundedLinesEx(rect, .5, 50, 2, BLACK);
    getSpriteHolder()->drawSprite(sprite_id, rect, {0, 0}, 0);
}

bool SummonGain::isFinished() {
    return finished;
}
