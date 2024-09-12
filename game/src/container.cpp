#include "container.h"
#include "add_tower.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "defense_tower.hpp"
#include "game_object.h"
#include "box2d/b2_draw.h"
#include "debug_draw.hpp"
#include "raylib.h"
#include "tower_spawn.hpp"
#include "utils.h"
#include "add_warriors.hpp"
#include "raymath.h"
#include "enemy_wave_config.hpp"
#include "enemy_wave.hpp"

Container::Container() {
    b2Vec2 gravity(0.0f, 0.0f);
    world = std::make_shared<b2World>(gravity);
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);
    groundBody = world->CreateBody(&groundBodyDef);
    contactListener = new MyContactListener();
    world->SetContactListener(contactListener);
    RaylibDebugDraw *debugDraw = new RaylibDebugDraw();
    debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
    world->SetDebugDraw(debugDraw);
    health = getMaxCastleHealthByLevel(level);
    initTimers();
}

float Container::getFormMvSpd() {
    return form->getMvSpd();
}

b2Fixture *Container::addFormFixture(b2FixtureDef *fixtureDef) {
    return form->addFormationFixture(fixtureDef);
}

void Container::removeFormFixture(b2Fixture *fixture) {
    form->removeFormationFixture(fixture);
}

void Container::init() {
    castle = std::make_shared<Castle>(VIRTUAL_WIDTH / 2.0f,
                                      VIRTUAL_HEIGHT / 2.0f, health, level);
    castle->init();
    gameObjects.push_back(castle);
    attackUnits.push_back(castle);
    initFormation();
    std::shared_ptr<EnemyWaveConfig> waveConfig =
        getEnemyWaveConfig(wave_count);
    wave = std::make_shared<EnemyWave>(waveConfig->time, waveConfig->count,
                                       castle);
    towerSpawn = std::make_shared<TowerSpawn>();
}

void Container::initFormation() {
    form = std::make_shared<Formation>(
        1, VIRTUAL_WIDTH / 2.0f + castle->getDimensions().x / 2 + 40,
        VIRTUAL_HEIGHT / 2.0f, 10, 2);
    form->initOrbits();
    attackUnits.push_back(form->getKeyWarrior());
}

void Container::appendToFormation(int count) {
    form->appendWarriors(count);
}

std::shared_ptr<GameObject> Container::getClosestAttackUnit(Vector2 pos) {
    std::shared_ptr<GameObject> closest = nullptr;
    float minDistance = 1000000.0f;
    float min = 1000;
    for (auto &go : attackUnits) {
        float distance = Vector2Distance(pos, {go->x, go->y});
        if (distance < min) {
            closest = go;
        }
    }
    return closest;
}

void Container::initAppend(int count) {
    toAppend = true;
    appendCount = count;
}

void Container::initTower(float x, float y, int archers, TowerSpawnRing *ring) {
    towerRequests.push(new DefenseTowerRequests(x, y, archers, ring));
    // addGameObject(std::make_shared<DefenseTower>(x, y, archers));
}

Container::~Container() {
    delete contactListener;
}

std::shared_ptr<b2World> Container::getWorld() {
    return world;
}

void Container::initTimers() {
    timer.every(
        randomFloatInRange(5.0f, 8.0f),
        [this](float dt) {
            Vector2 dd = this->castle->getDimensions();
            int xr = getRandomValue(0, 1);
            int yr = getRandomValue(0, 1);
            float xd;
            if (xr == 0)
                xd = randomFloatInRange(-VIRTUAL_WIDTH / 3.0f, -dd.x / 2.0f);
            else
                xd = randomFloatInRange(dd.x / 2.0f, VIRTUAL_WIDTH / 3.0f);
            float yd;
            if (yr == 0)
                yd = randomFloatInRange(-VIRTUAL_HEIGHT / 3.0f, -dd.y / 2.0f);
            else
                yd = randomFloatInRange(dd.y / 2.0f, VIRTUAL_HEIGHT / 3.0f);
            std::shared_ptr<AddWarriors> aw = std::make_shared<AddWarriors>(
                this->castle->x + xd, this->castle->y + yd);
            aw->init();
            this->addGameObject(aw);
        },
        0, []() {}, "addWarriors");
    // timer.every(
    //     randomFloatInRange(2.0f, 3.0f),
    //     [this](float dt) {
    //         Vector2 dd = this->castle->getDimensions();
    //         int xr = getRandomValue(0, 1);
    //         int yr = getRandomValue(0, 1);
    //         float xd;
    //         if (xr == 0)
    //             xd = randomFloatInRange(-VIRTUAL_WIDTH / 3.0f, -dd.x / 2.0f);
    //         else
    //             xd = randomFloatInRange(dd.x / 2.0f, VIRTUAL_WIDTH / 3.0f);
    //         float yd;
    //         if (yr == 0)
    //             yd = randomFloatInRange(-VIRTUAL_HEIGHT / 3.0f, -dd.y
    //             / 2.0f);
    //         else
    //             yd = randomFloatInRange(dd.y / 2.0f, VIRTUAL_HEIGHT / 3.0f);
    //         std::shared_ptr<AddTower> at = std::make_shared<AddTower>(
    //             this->castle->x + xd, this->castle->y + yd);
    //         at->init();
    //         this->addGameObject(at);
    //     },
    //     0, []() {}, "addTowers");
}

void Container::draw() {
    if (gameover) {
        const char *t = "Game Over";
        float sz = MeasureText(t, 20);
        DrawText(t, form->x - sz / 2, form->y, 20, WHITE);
        return;
    }
    for (auto &go : scumObjects) {
        go->draw();
    }
    std::vector<std::shared_ptr<GameObject>> formObjects;
    form->getGameObjects(formObjects);
    // castle->draw();
    // merge gameObjects and formObjects and sort them based y values ascending

    std::vector<std::shared_ptr<GameObject>> gameObjectsMerged;
    gameObjectsMerged.reserve(gameObjects.size() + formObjects.size());
    gameObjectsMerged.insert(gameObjectsMerged.end(), gameObjects.begin(),
                             gameObjects.end());
    gameObjectsMerged.insert(gameObjectsMerged.end(), formObjects.begin(),
                             formObjects.end());
    std::sort(gameObjectsMerged.begin(), gameObjectsMerged.end(),
              [](const std::shared_ptr<GameObject> &a,
                 const std::shared_ptr<GameObject> &b) { return a->y < b->y; });
    std::vector<std::shared_ptr<GameObject>> gameObjectsRaised;
    for (auto &go : gameObjectsMerged) {
        if (go->raised) {
            gameObjectsRaised.push_back(go);
        } else {
            go->draw();
        }
    }
    for (auto &go : gameObjectsRaised) {
        go->draw();
    }
    // form->draw();
    if (colliderDebugDraw) world->DebugDraw();
}

void Container::update(float dt) {
    if (gameover) {
        castle = nullptr;
        if (gameObjects.size() > 0) {
            gameObjects.clear();
        }
        return;
    }
    timer.update(dt);
    world->Step(dt, 8, 3);
    castle->update(dt);
    std::vector<size_t> unitIndicesToRemove;
    for (size_t i = 0; i < attackUnits.size(); i++) {
        std::shared_ptr<GameObject> go = attackUnits[i];
        if (!go->isAlive()) {
            unitIndicesToRemove.push_back(i);
        }
    }
    for (size_t i = unitIndicesToRemove.size(); i > 0; --i) {
        size_t index = unitIndicesToRemove[i - 1];
        attackUnits.erase(attackUnits.begin() + index);
    }
    std::vector<size_t> scumIndicesToRemove;
    for (size_t i = 0; i < scumObjects.size(); i++) {
        std::shared_ptr<GameObject> go = scumObjects[i];
        go->update(dt);
        if (!go->isAlive()) {
            scumIndicesToRemove.push_back(i);
            go->cleanupData();
        }
    }
    for (size_t i = scumIndicesToRemove.size(); i > 0; --i) {
        size_t index = scumIndicesToRemove[i - 1];
        scumObjects.erase(scumObjects.begin() + index);
    }
    std::vector<size_t> indices_to_remove;
    for (size_t i = 0; i < gameObjects.size(); i++) {
        std::shared_ptr<GameObject> go = gameObjects[i];
        go->update(dt);
        if (!go->isAlive()) {
            indices_to_remove.push_back(i);
            // do something here to cleanup the pointer
            go->cleanupData();
        }
    }

    for (size_t i = indices_to_remove.size(); i > 0; --i) {
        size_t index = indices_to_remove[i - 1];
        gameObjects.erase(gameObjects.begin() + index);
    }
    if (toAppend) {
        appendToFormation(appendCount);
        toAppend = false;
        appendCount = 0;
    }
    if (!towerRequests.empty()) {
        while (!towerRequests.empty()) {
            DefenseTowerRequests *req = towerRequests.top();
            std::shared_ptr<DefenseTower> tower =
                std::make_shared<DefenseTower>(req->x, req->y, req->archers);
            tower->init();
            addGameObject(tower);
            addAttackUnit(tower);
            if (req->ring != nullptr) {
                req->ring->assignTower(tower);
            }
            // Vector2 ss = getSpriteHolder()->getSpriteSize(TOWER_SPRITE_ID);
            // float kr = 2.0f * DEFENSE_TOWER_RADIUS / ss.x;
            // addGameObject(
            // std::make_shared<Archer>(req->x, req->y - 260 * kr, tower));
            towerRequests.pop();
        }
    }
    form->update(dt);
    towerSpawn->update(dt);
    if (wave != nullptr) {
        wave->update(dt);
        if (wave->isFinishedWave()) {
            std::shared_ptr<EnemyWaveConfig> waveConfig =
                getEnemyWaveConfig(wave_count);
            wave_delay = waveConfig->delayAfterWave;
            wave_count++;
            wave = nullptr;
        }
    }
    if (wave_delay > 0) {
        wave_delay_timer += dt;
        if (wave_delay_timer >= wave_delay) {
            wave_delay_timer = 0;
            wave_delay = 0;
            std::shared_ptr<EnemyWaveConfig> waveConfig =
                getEnemyWaveConfig(wave_count);
            wave = std::make_shared<EnemyWave>(waveConfig->time,
                                               waveConfig->count, castle);
        }
    }
    if (IsKeyPressed(KEY_F1)) {
        colliderDebugDraw = !colliderDebugDraw;
    }
    if (!form->isKeyWarriorAlive()) gameover = true;
    if (!castle->isAlive()) gameover = true;
}

void Container::addGameObject(std::shared_ptr<GameObject> obj) {
    gameObjects.push_back(obj);
}

void Container::addScumObject(std::shared_ptr<GameObject> obj) {
    scumObjects.push_back(obj);
}

void Container::addAttackUnit(std::shared_ptr<GameObject> obj) {
    attackUnits.push_back(obj);
}

std::shared_ptr<GameObject> Container::getFollowObject() {
    // int x = getRandomValue(0, 1);
    // if (x == 0)
    return form->getKeyWarrior();
    // else
    // return castle;
}

Vector2 Container::getFormPos() {
    return Vector2{form->x, form->y};
}
