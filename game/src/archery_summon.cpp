#include "archery_summon.hpp"
#include "archer.hpp"
#include "globals.h"
#include "raymath.h"
#include "utils.h"
#include <memory>

ArcherySummon::ArcherySummon(Vector2 position,
                             std::shared_ptr<SummonCard> card) {

    getAudioManager()->playSound("summon_infantry");
    Vector2 cc = getContainer()->region->getCenterCoordinates();
    Vector2 dirDepth = Vector2Subtract(position, cc);
    dirDepth = Vector2Normalize(dirDepth);
    Vector2 dp1 = {-dirDepth.y, dirDepth.x};
    Vector2 dp2 = {dirDepth.y, -dirDepth.x};
    Vector2 sp = position;
    WarriorType wtype =
        card->types[getRandomIntInRange(0, card->types.size() - 1)];
    int unit_size = get_warrior_size(wtype);
    int us = card->w / 2;
    float uss = unit_size * 3;
    float ssx = sp.x + dp1.x * us * uss;
    float ssy = sp.y + dp1.y * us * uss;
    Direction dd = get_direction(dirDepth);
    for (int i = 0; i < card->w; i++) {
        float spawn_x = ssx;
        float spawn_y = ssy;
        for (int j = 0; j < card->d; j++) {
            // spawning enemy unit here and adding to game objects
            std::shared_ptr<Archer> aa =
                std::make_shared<Archer>(spawn_x, spawn_y, nullptr, true);
            aa->init();
            aa->directionFacing = dd;
            aa->directionAttacking = dd;
            getContainer()->addGameObject(aa);
            spawn_x += dirDepth.x * uss;
            spawn_y += dirDepth.y * uss;
        }

        ssx += dp2.x * uss;
        ssy += dp2.y * uss;
    }
}

ArcherySummon::~ArcherySummon() {
}
