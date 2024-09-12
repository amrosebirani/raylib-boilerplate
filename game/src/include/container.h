#pragma once

#include "box2d/b2_world.h"
#include "collider_user_data.h"
#include "formation.h"
#include "castle.hpp"
#include "timer.h"
#include "enemy_wave.hpp"
#include "tower_spawn.hpp"
#include <memory>
#include <stack>

struct DefenseTowerRequests {
        float x;
        float y;
        int archers;
        TowerSpawnRing *ring = nullptr;

        DefenseTowerRequests(float x, float y, int archers,
                             TowerSpawnRing *ring = nullptr)
            : x(x), y(y), archers(archers), ring(ring) {};
};

class Container {
    public:
        Container();
        ~Container();
        std::shared_ptr<b2World> getWorld();
        void initFormation();
        void init();
        void initAppend(int count);
        void appendToFormation(int count);
        void initTower(float x, float y, int archers,
                       TowerSpawnRing *ring = nullptr);

        void draw();
        void update(float dt);
        void cleanup();
        void addGameObject(std::shared_ptr<GameObject> obj);
        void addScumObject(std::shared_ptr<GameObject> obj);
        void addAttackUnit(std::shared_ptr<GameObject> obj);
        std::shared_ptr<GameObject> getClosestAttackUnit(Vector2 pos);
        std::shared_ptr<GameObject> getFollowObject();
        Vector2 getFormPos();
        float getFormMvSpd();
        b2Fixture *addFormFixture(b2FixtureDef *fixtureDef);
        void removeFormFixture(b2Fixture *fixture);

    private:
        std::shared_ptr<b2World> world;
        std::shared_ptr<Formation> form;
        std::shared_ptr<Castle> castle;
        b2Body *groundBody;
        MyContactListener *contactListener;
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        // scum objects contain
        std::vector<std::shared_ptr<GameObject>> scumObjects;
        std::vector<std::shared_ptr<GameObject>> attackUnits;
        bool colliderDebugDraw = false;
        Timer timer;
        void initTimers();
        bool toAppend = false;
        int appendCount = 0;
        bool gameover = false;
        int level = 1;
        float health = 0;
        std::stack<DefenseTowerRequests *> towerRequests;
        std::shared_ptr<EnemyWave> wave = nullptr;
        std::shared_ptr<TowerSpawn> towerSpawn = nullptr;
        int wave_count = 1;
        float wave_delay = 0;
        float wave_delay_timer = 0;
};
