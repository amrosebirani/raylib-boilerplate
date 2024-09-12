#pragma once
#include <memory>
#include "game_object.h"
#include "raylib.h"
#include <vector>

struct TowerSpawnLocation {
        float x;
        float y;
        bool occupied = false;
        std::shared_ptr<GameObject> tower = nullptr;
        std::shared_ptr<GameObject> addTower = nullptr;

        TowerSpawnLocation(float x, float y) : x(x), y(y) {
        }
};

class TowerSpawnRing {
    public:
        TowerSpawnRing(std::vector<Vector2> locations);
        bool hasEmpty();
        void update(float dt);
        void assignTower(std::shared_ptr<GameObject> tower);
        void spawnAddTower();

    private:
        std::vector<TowerSpawnLocation> locations;
};

class TowerSpawn {
    public:
        TowerSpawn();
        void update(float dt);

    private:
        std::vector<std::shared_ptr<TowerSpawnRing>> rings;
        Timer timer;
};
