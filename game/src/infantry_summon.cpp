#include "infantry_summon.hpp"
#include "globals.h"
#include "warrior_types.h"
#include "raymath.h"
#include "warrior_factory.h"

InfantrySummon::InfantrySummon(Vector2 position,
                               std::shared_ptr<SummonCard> card) {
    Vector2 cc = getContainer()->region->getCenterCoordinates();
    Vector2 dirDepth = Vector2Subtract(position, cc);
    dirDepth = Vector2Normalize(dirDepth);
    Vector2 dp1 = {-dirDepth.y, dirDepth.x};
    Vector2 dp2 = {dirDepth.y, -dirDepth.x};
    Vector2 sp = position;
    int unit_size = get_warrior_size(card->types[0]);
    int us = card->w / 2;
    float uss = unit_size * 3;
    float ssx = sp.x + dp1.x * us * uss;
    float ssy = sp.y + dp1.y * us * uss;
    for (int i = 0; i < card->w; i++) {
        float spawn_x = ssx;
        float spawn_y = ssy;
        for (int j = 0; j < card->d; j++) {
            // spawning enemy unit here and adding to game objects
            std::shared_ptr<Warrior> pe = WarriorFactory::createWarrior(
                card->types[1], spawn_x, spawn_y, 0);
            getContainer()->addGameObject(pe);
            spawn_x += dirDepth.x * uss;
            spawn_y += dirDepth.y * uss;
        }

        ssx += dp2.x * uss;
        ssy += dp2.y * uss;
    }
}

InfantrySummon::~InfantrySummon() {
}
