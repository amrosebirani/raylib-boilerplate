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

        void assignTower(std::shared_ptr<GameObject> tower) {
            this->tower = tower;
            occupied = true;
        }
};

class TowerSpawnRing {
    public:
        TowerSpawnRing(std::vector<Vector2> locations);
        ~TowerSpawnRing();
        bool hasEmpty();
        void update(float dt);
        void assignTower(std::shared_ptr<GameObject> tower);
        void spawnAddTower();
        void draw();

    private:
        std::vector<TowerSpawnLocation *> locations;
};

class TowerSpawn {
    public:
        TowerSpawn();
        ~TowerSpawn();
        void update(float dt);
        void draw();

    private:
        std::vector<std::shared_ptr<TowerSpawnRing>> rings;
        Timer timer;
};
