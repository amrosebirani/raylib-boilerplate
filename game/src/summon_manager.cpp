#include "summon_manager.hpp"
#include "globals.h"
#include "infantry_summon.hpp"
#include "raylib.h"

SummonManager::SummonManager() {
}

SummonManager::~SummonManager() {
}

void SummonManager::update(float dt) {
    if (!summonActivated) {
        summonTimer += dt;
        if (summonTimer >= summonRT) {
            summonActivated = true;
            summonTimer = 0;
        }
    }

    if (summonActivated && IsKeyPressed(KEY_J) && !summonCards.empty()) {
        summonActivated = false;
        auto card = summonCards.front();
        summonCards.pop();
        Vector2 pos = getContainer()->getFormPos();
        InfantrySummon *inf = new InfantrySummon(pos, card);
        delete inf;
    }
}

void SummonManager::draw() {
}

void SummonManager::addSummonCard(std::shared_ptr<SummonCard> card) {
    summonCards.push(card);
}
