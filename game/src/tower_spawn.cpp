#include "tower_spawn.hpp"
#include "constants.h"
#include "globals.h"
#include "utils.h"
#include "add_tower.hpp"
#include <iostream>

bool TowerSpawnRing::hasEmpty() {
    for (auto &location : locations) {
        if (!location.occupied) {
            return true;
        }
    }
    return false;
}

void TowerSpawnRing::update(float dt) {
    for (auto &location : locations) {
        if (location.addTower != nullptr) {
            if (!location.addTower->isAlive()) {
                location.addTower = nullptr;
            }
        }
        if (location.tower != nullptr) {
            if (!location.tower->isAlive()) {
                location.tower = nullptr;
                location.occupied = false;
            }
        }
    }
}

void TowerSpawnRing::assignTower(std::shared_ptr<GameObject> tower) {
    for (auto &location : locations) {
        if ((VIRTUAL_WIDTH / 2.0f + location.x) == tower->x &&
            (VIRTUAL_HEIGHT / 2.0f + location.y) == tower->y) {
            std::cout << "tower assighed\n";
            location.tower = tower;
            location.occupied = true;
        }
    }
}

void TowerSpawnRing::spawnAddTower() {
    bool spawned = false;
    int index = 0;
    while (!spawned) {
        TowerSpawnLocation &location = locations[index];
        if (!location.occupied && location.addTower == nullptr) {
            int xx = getRandomIntInRange(1, 20);
            if (xx < 6) {
                std::shared_ptr<AddTower> at = std::make_shared<AddTower>(
                    VIRTUAL_WIDTH / 2.0f + location.x,
                    VIRTUAL_HEIGHT / 2.0f + location.y, this);
                at->init();
                location.addTower = at;
                getContainer()->addGameObject(at);
                spawned = true;
            }
        }
        index = (index + 1) % locations.size();
    }
}

TowerSpawnRing::TowerSpawnRing(std::vector<Vector2> locations) {
    for (auto &location : locations) {
        TowerSpawnLocation tsl(location.x, location.y);
        this->locations.push_back(tsl);
    }
}

TowerSpawn::TowerSpawn() {
    // initialize the rings here
    float tb = TOWER_BASE_DISTANCE;
    float tu = TOWER_UNIT_DISTANCE;
    float ss = tb + tu;
    rings.push_back(
        std::make_shared<TowerSpawnRing>(std::vector<Vector2>{{ss, ss},
                                                              {0, ss},
                                                              {-ss, ss},
                                                              {-ss, 0},
                                                              {-ss, -ss},
                                                              {0, -ss},
                                                              {ss, -ss},
                                                              {ss, 0}}));
    ss += tu;
    rings.push_back(
        std::make_shared<TowerSpawnRing>(std::vector<Vector2>{{ss, ss},
                                                              {ss - tu, ss},
                                                              {0, ss},
                                                              {tu - ss, ss},
                                                              {-ss, ss},
                                                              {-ss, ss - tu},
                                                              {-ss, 0},
                                                              {-ss, tu - ss},
                                                              {-ss, -ss},
                                                              {-ss + tu, -ss},
                                                              {0, -ss},
                                                              {ss - tu, -ss},
                                                              {ss, -ss},
                                                              {ss, -ss + tu},
                                                              {ss, 0},
                                                              {ss, ss - tu}}));
    timer = Timer();
    timer.every(
        randomFloatInRange(4.0f, 6.0f),
        [this](float dt) {
            for (auto &ring : rings) {
                if (ring->hasEmpty()) {
                    ring->spawnAddTower();
                    break;
                }
            }
        },
        0, []() {}, "");
}

void TowerSpawn::update(float dt) {
    timer.update(dt);
    for (auto &ring : rings) {
        ring->update(dt);
    }
}
