#include "enemy_horde.hpp"
#include "constants.h"
#include "enemy_types.h"
#include "globals.h"
#include "horde_config.hpp"
#include "parameterized_enemy.hpp"
#include "raymath.h"
#include "utils.h"

EnemyHorde::EnemyHorde(HordeConfig *conf) {
    std::vector<Vector2> regionPoints =
        getContainer()->region->getRegionPoints(conf->horde_angle);
    Vector2 dirDepth = Vector2Subtract(regionPoints[1], regionPoints[0]);
    dirDepth = Vector2Normalize(dirDepth);
    Vector2 dp1 = {-dirDepth.y, dirDepth.x};
    Vector2 dp2 = {dirDepth.y, -dirDepth.x};
    Vector2 sp = regionPoints[3];
    float drr = randomFloatInRange(0, CASTLE_WIDTH);
    sp.x += drr * dirDepth.x;
    sp.y += drr * dirDepth.y;
    int ls = conf->leaderCount / 2;
    int leader_size = get_enemy_size(conf->leaderType);
    int ss = leader_size * 2;
    float lxe = sp.x + ls * dp1.x * ss;
    float lye = sp.y + ls * dp1.y * ss;
    for (int i = 0; i < conf->leaderCount; i++) {
        // spawning leaders and adding to game objects

        std::shared_ptr<ParameterizedEnemy> pe =
            std::make_shared<ParameterizedEnemy>(lxe, lye, conf->leaderType);
        pe->setMarching(regionPoints[1]);
        // for leader we also set target and point to trigger storyboard to
        pe->setTargetObj(getContainer()->region->castle);
        if (i == 0) {
            pe->setCameraTarget(regionPoints[2]);
            // getViewCamera()->follow(pe->get_shared_ptr());
        }
        // start cinematographic camera
        pe->init();
        getContainer()->addGameObject(pe);
        // setting location
        lxe += dp2.x * ss;
        lye += dp2.y * ss;
    }

    float uspx = sp.x + dirDepth.x * ss;
    float uspy = sp.y + dirDepth.y * ss;
    int unit_size = get_enemy_size(conf->unitType);
    int us = conf->width / 2;
    float uss = unit_size * 3;
    float ssx = uspx + dp1.x * us * uss;
    float ssy = uspy + dp1.y * us * uss;
    for (int i = 0; i < conf->width; i++) {
        float spawn_x = ssx;
        float spawn_y = ssy;
        for (int j = 0; j < conf->depth; j++) {
            // spawning enemy unit here and adding to game objects

            std::shared_ptr<ParameterizedEnemy> pe =
                std::make_shared<ParameterizedEnemy>(spawn_x, spawn_y,
                                                     conf->unitType);
            pe->setMarching(regionPoints[1]);
            pe->init();
            getContainer()->addGameObject(pe);

            // set next depth location
            spawn_x += dirDepth.x * uss;
            spawn_y += dirDepth.y * uss;
        }

        // setting next start location
        ssx += dp2.x * uss;
        ssy += dp2.y * uss;
    }
    delete conf;
}

EnemyHorde::~EnemyHorde() {
}
