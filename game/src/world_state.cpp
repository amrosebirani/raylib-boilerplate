#include "world_state.hpp"
#include "constants.h"
#include "gem_type.hpp"
#include "raylib.h"
#include "globals.h"

using std::string;

WorldState::WorldState() {
    gem_chance_list = new TempChanceList<GemType>(
        {{GemType::Coin, 7}, {GemType::Diamond, 3}});

    gems_for_next_upgrade.push_back(5);
    for (int i = 1; i < 1024; i = i + 4) {
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i - 1] + 3);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i]);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i + 1] * 1.3);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i + 2]);
    }
    gem_progress_bar =
        new ProgressBar(0, 40, GetScreenWidth(), 40, 1, 0, {54, 137, 179, 255});
    summon_manager = std::make_shared<SummonManager>();
}

void WorldState::draw() {
    DrawRectangle(0, 0, GetScreenWidth(), 80, {0, 0, 0, 125});
    const char *tg = TextFormat("%d", gems_for_next_upgrade[gem_round] - gems);
    int tgm = MeasureText(tg, 20);
    DrawText(tg, 95 - tgm, 10, 20, WHITE);
    getSpriteHolder()->drawSprite(COINS_AND_GEMS, 5, {100, 10, 20, 20});

    DrawText("to next upgrade", 125, 10, 20, WHITE);

    getSpriteHolder()->drawSprite(COINS_AND_GEMS, 1,
                                  {GetScreenWidth() - 40.0f, 10, 20, 20});
    const char *tt = TextFormat("%d", coins);
    int ttm = MeasureText(tt, 20);
    DrawText(tt, GetScreenWidth() - 40.0f - ttm - 5, 10, 20, WHITE);
    gem_progress_bar->draw();
    summon_manager->draw();
}

WorldState::~WorldState() {
    delete gem_chance_list;
    delete gem_progress_bar;
}

void WorldState::addCoins(int coins) {
    this->coins += coins;
}

void WorldState::addGems(int g) {
    this->gems += g;
    if (this->gems >= gems_for_next_upgrade[gem_round]) {
        this->gems -= gems_for_next_upgrade[gem_round];
        gem_round += 1;
    }
    gem_progress_bar->setCurrent(getCurrentGemPercent());
}

void WorldState::removeCoins(int coins) {
    this->coins -= coins;
}

void WorldState::removeGems(int gems) {
    this->gems -= gems;
}

int WorldState::getCoins() {
    return coins;
}

int WorldState::getGems() {
    return gems;
}

GemType WorldState::getNextGemType() {
    return gem_chance_list->next();
}

float WorldState::getCurrentGemPercent() {
    return (float)gems / gems_for_next_upgrade[gem_round];
}

bool WorldState::update(float dt) {
    summon_manager->update(dt);
    return true;
}

bool WorldState::isFinished() {
    return false;
}
