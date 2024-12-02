#pragma once

#include "room.hpp"
#include "box2d/b2_world.h"
#include "cinematographer.hpp"
#include "collider_user_data.h"
#include "formation.h"
#include "horde_manager.hpp"
#include "region.hpp"
#include "timer.h"
// #include "enemy_wave.hpp"
// #include "tower_spawn.hpp"
#include "tree_patch.hpp"
#include <memory>
// #include <stack>

// struct DefenseTowerRequests {
//         float x;
//         float y;
//         int archers;
//         TowerSpawnLocation *location = nullptr;
//
// DefenseTowerRequests(float x, float y, int archers,
//                      TowerSpawnLocation *location = nullptr)
//             : x(x), y(y), archers(archers), location(location) {};
// };

class Container : public Room {
    public:
        Container();
        ~Container();
        std::shared_ptr<b2World> getWorld();
        void initFormation();
        void init();
        void initAppend(int count);
        void appendToFormation(int count);
        // void initTower(float x, float y, int archers,
        // TowerSpawnLocation *location = nullptr);
        void drawGround();

        void draw() override;
        bool isFinished() override;
        bool update(float dt) override;
        void cleanup();
        void addGameObject(std::shared_ptr<GameObject> obj);
        void addScumObject(std::shared_ptr<GameObject> obj);
        void addAttackUnit(std::shared_ptr<Building> obj);
        std::shared_ptr<GameObject> getClosestAttackUnit(Vector2 pos);
        std::shared_ptr<GameObject> getFollowObject();
        Vector2 getFormPos();
        float getFormMvSpd();
        b2Fixture *addFormFixture(b2FixtureDef *fixtureDef);
        void removeFormFixture(b2Fixture *fixture);
        std::shared_ptr<Region> region;
        std::shared_ptr<Cinematographer> cinematographer;
        std::shared_ptr<HordeManager> hmm;
        float getCastleHealth();
        void endGame();
        void gameOverSet();
        void victorySet();

    private:
        std::shared_ptr<b2World> world;
        std::shared_ptr<Formation> form;
        // std::shared_ptr<Castle> castle;
        // b2Body *groundBody;
        MyContactListener *contactListener;
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        // scum objects contain
        std::vector<std::shared_ptr<GameObject>> scumObjects;
        std::vector<std::shared_ptr<Building>> attackUnits;
        bool colliderDebugDraw = false;
        Timer timer;
        void initTimers();
        bool toAppend = false;
        int appendCount = 0;
        bool gameover = false;
        bool victory = false;
        // std::stack<DefenseTowerRequests *> towerRequests;
        // std::shared_ptr<EnemyWave> wave = nullptr;
        // std::shared_ptr<TowerSpawn> towerSpawn = nullptr;
        // int wave_count = 1;
        // float wave_delay = 0;
        // float wave_delay_timer = 0;
        std::vector<std::shared_ptr<TreePatch>> tree_patches;
        void setMiniMapDetails();
        void drawMiniMap(std::shared_ptr<GameObject> go);
        float miniMapS;
        float miniMapW;
        float miniMapH;
        Rectangle miniMap;
        Vector2 miniMapO;
        Vector2 center;
        float physicsTimeStep = 1.0f / 60.0f;
        float physicsAccumulator = 0;
};
