#include "container.h"
#include "collider_user_data.h"
#include "constants.h"
#include "firebase.hpp"
#include "game_object.h"
#include "box2d/b2_draw.h"
#include "debug_draw.hpp"
#include "game_object_types.h"
#include "gems.hpp"
#include "globals.h"
#include "horde_manager.hpp"
#include "parameterized_enemy.hpp"
#include "raylib.h"
#include "scoreboard.hpp"
#include "raymath.h"
#include "warrior_para.hpp"
#include <algorithm>
#include <memory>
#include <set>

Container::Container() {
    setBasics();
}

Container::Container(std::ifstream &in) {
    setBasics();
    in.read(reinterpret_cast<char *>(&gameover), sizeof(gameover));
    in.read(reinterpret_cast<char *>(&victory), sizeof(victory));
}

void Container::setBasics() {
    b2Vec2 gravity(0.0f, 0.0f);
    world = std::make_shared<b2World>(gravity);
    contactListener = new MyContactListener();
    world->SetContactListener(contactListener);
    RaylibDebugDraw *debugDraw = new RaylibDebugDraw();
    debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
    world->SetDebugDraw(debugDraw);
    fireAnimation =
        new Animation({2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, true, 0.2);
}

void Container::Save(std::ofstream &out) const {
    out.write(reinterpret_cast<const char *>(&gameover), sizeof(gameover));
    out.write(reinterpret_cast<const char *>(&victory), sizeof(victory));
    region->Save(out);
    form->save(out);
    hmm->Save(out);
    saveGameObjects(out);
}

float Container::getCastleHealth() {
    return region->castle->health * 1.0f / region->castle->maxHealth;
}

float Container::getFormMvSpd() {
    return form->getMvSpd();
}

bool Container::isFinished() {
    return gameover || victory;
}

b2Fixture *Container::addFormFixture(b2FixtureDef *fixtureDef) {
    return form->addFormationFixture(fixtureDef);
}

void Container::removeFormFixture(b2Fixture *fixture) {
    form->removeFormationFixture(fixture);
}

std::shared_ptr<UpgradeContent> Container::getUpgradeContent() {
    return upgradeContent;
}

std::shared_ptr<UpgradeManager> Container::getUpgradeManager() {
    return upgradeManager;
}

void Container::baseInit() {

    upgradeContent = std::make_shared<UpgradeContent>();
    upgradeManager = std::make_shared<UpgradeManager>();
}

void Container::loadInit(std::ifstream &in) {
    baseInit();
    region = std::make_shared<Region>(in);
    region->init();
    setMiniMapDetails();
    gameObjects.push_back(region->castle);
    attackUnits.push_back(region->castle);
    form = std::make_shared<Formation>(in);
    form->initOrbits();
    hmm = std::make_shared<HordeManager>(in);
    getViewCamera()->follow(getFollowObject());
}

void Container::init() {
    baseInit();
    float region_dimension = CASTLE_WIDTH + CASTLE_WIDTH * 2;
    region =
        std::make_shared<Region>(VIRTUAL_WIDTH / 2.0f, VIRTUAL_HEIGHT / 2.0f,
                                 region_dimension, region_dimension);
    region->init();
    setMiniMapDetails();
    gameObjects.push_back(region->castle);
    attackUnits.push_back(region->castle);
    initFormation();
    hmm = std::make_shared<HordeManager>(-1);
    getViewCamera()->follow(getFollowObject());
}

void Container::initFormation() {
    form = std::make_shared<Formation>(
        1, VIRTUAL_WIDTH / 2.0f + region->castle->getDimensions().x / 2 + 40,
        VIRTUAL_HEIGHT / 2.0f, 10, 2);
    form->initOrbits();
    // attackUnits.push_back(form->getKeyWarrior());
}

void Container::saveGameObjects(std::ofstream &out) const {
    std::set<GameObjectType> saved_types = {
        GameObjectType::ARCHER,
        GameObjectType::ENEMY,
        GameObjectType::GEM,
        GameObjectType::WARRIOR,
    };
    for (auto &go : gameObjects) {
        // check set membership
        if (saved_types.find(go->getObjectType()) != saved_types.end()) {
            if (go->getObjectType() == GameObjectType::ARCHER) {
                if (go->raised) {
                    continue;
                }
            }
            out.write(reinterpret_cast<const char *>(go->getObjectType()),
                      sizeof(go->getObjectType()));
            go->Save(out);
        }
    }
}

void Container::readGameObjects(std::ifstream &in) {
    // read till the end of the file
    while (in.peek() != EOF) {
        GameObjectType type;
        in.read(reinterpret_cast<char *>(&type), sizeof(type));
        std::shared_ptr<GameObject> go = nullptr;
        if (type == GameObjectType::ARCHER) {
            go = std::make_shared<Archer>(in);
            go->init();
        } else if (type == GameObjectType::ENEMY) {
            std::shared_ptr<ParameterizedEnemy> pe =
                std::make_shared<ParameterizedEnemy>(in);
            pe->baseInit();
            go = pe;
        } else if (type == GameObjectType::GEM) {
            go = std::make_shared<Gem>(in);
            go->init();
        } else if (type == GameObjectType::WARRIOR) {
            std::shared_ptr<WarriorPara> wp = std::make_shared<WarriorPara>(in);
            wp->baseInit();
            go = wp;
        }

        if (go != nullptr) {
            gameObjects.push_back(go);
        }
    }
}

void Container::appendToFormation(int count) {
    form->appendWarriors(count);
}

std::shared_ptr<GameObject> Container::getClosestAttackUnit(Vector2 pos) {
    std::shared_ptr<GameObject> closest = nullptr;
    float minDistance = 100000.0f;
    for (auto &go : attackUnits) {
        float distance = Vector2DistanceSqr(pos, {go->x, go->y});
        if (distance < minDistance) {
            closest = go;
        }
    }
    if (closest == nullptr) {
        return region->castle;
    }
    return closest;
}

Container::~Container() {
    world = nullptr;
    delete contactListener;
    delete fireAnimation;
}

std::shared_ptr<b2World> Container::getWorld() {
    return world;
}

void Container::draw() {
    if (gameover || victory) {
        return;
    }
    getViewCamera()->attach(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    drawGround();
    // if (gameover) {
    //     const char *t = "Game Over";
    //     float sz = MeasureText(t, 20);
    //     DrawText(t, form->x - sz / 2, form->y, 20, WHITE);
    //     return;
    // }
    if (region == nullptr) return;
    region->draw();
    // towerSpawn->draw();
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

    for (auto &go : flashObjects) {
        go->draw();
    }
    // for (std::shared_ptr<TreePatch> tp : tree_patches) {
    //     tp->draw();
    // }
    // form->draw();
    if (colliderDebugDraw) world->DebugDraw();
    // for (auto &pp : rp1) {
    //     DrawCircle(pp.x, pp.y, 10, BLACK);
    // }
    // for (auto &pp : rp2) {
    //     DrawCircle(pp.x, pp.y, 10, ORANGE);
    // }
    getViewCamera()->detach();

    DrawRectangleRounded(miniMap, .5, 50, {BLACK.r, BLACK.g, BLACK.b, 180});
    DrawRectangleRoundedLinesEx(miniMap, .5, 50, 1,
                                {WHITE.r, WHITE.g, WHITE.b, 150});
    for (auto &go : gameObjectsMerged) {
        if (!go->raised) drawMiniMap(go);
    }

    getViewCamera()->attach(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    getBloodSplatter()->draw();
    getEnemyBloodSplatter()->draw();
    getViewCamera()->detach();
}

void Container::drawMiniMap(std::shared_ptr<GameObject> go) {
    float xx = (go->x - center.x) * miniMapS + miniMapO.x;
    float yy = (go->y - center.y) * miniMapS + miniMapO.y;
    Color col = go->getColor();
    float size = 10;
    if (col.a > 155) {
        size = 24;
    }
    if (CheckCollisionPointRec({xx, yy}, miniMap)) {
        DrawRectangle(xx, yy, size, size, col);
    }
}

void Container::setMiniMapDetails() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float distToCover = 4.5 * CASTLE_WIDTH;
    if (sw > sh) {
        miniMapH = sh / 4;
        miniMapW = sw / 5;
        miniMapS = (miniMapH / 2) / distToCover;
        miniMap = {5.0f / 6 * sw - miniMapW / 2, getWorldState()->getMiniMapH(),
                   miniMapW, miniMapH};
        miniMapO = {miniMap.x + miniMapW / 2, miniMap.y + miniMapH / 2};
    } else {
        miniMapH = sh / 6;
        miniMapW = sw / 3;
        miniMapS = (miniMapH / 2) / distToCover;
        miniMap = {1.0f / 2 * sw - miniMapW / 2, getWorldState()->getMiniMapH(),
                   miniMapW, miniMapH};
        miniMapO = {miniMap.x + miniMapW / 2, miniMap.y + miniMapH / 2};
    }
    center = region->getCenterCoordinates();
}

int Container::getCurrentFireFrame() {
    return fireAnimation->getCurrentFrame();
}

bool Container::update(float dt) {
    if (gameover || victory) {
        // region = nullptr;
        if (gameObjects.size() > 0) {
            gameObjects.clear();
        }
        return true;
    }
    timer.update(dt);
    fireAnimation->update(dt);
    physicsAccumulator += dt;
    while (physicsAccumulator >= physicsTimeStep) {
        world->Step(physicsTimeStep, 8, 3);
        physicsAccumulator -= physicsTimeStep;
    }
    // world->Step(dt, 8, 3);
    region->update(dt);
    // cinematographer->update(dt);
    hmm->update(dt);
    std::vector<size_t> unitIndicesToRemove;
    for (size_t i = 0; i < attackUnits.size(); i++) {
        std::shared_ptr<Building> go = attackUnits[i];
        if (go->level == 0) {
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
    std::vector<size_t> flashIndicesToRemove;
    for (size_t i = 0; i < flashObjects.size(); i++) {
        std::shared_ptr<GameObject> go = flashObjects[i];
        go->update(dt);
        if (!go->isAlive()) {
            flashIndicesToRemove.push_back(i);
        }
    }
    for (size_t i = flashIndicesToRemove.size(); i > 0; --i) {
        size_t index = flashIndicesToRemove[i - 1];
        flashObjects.erase(flashObjects.begin() + index);
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
    form->update(dt);
    getBloodSplatter()->update(dt);
    getEnemyBloodSplatter()->update(dt);
    if (IsKeyPressed(KEY_F1)) {
        colliderDebugDraw = !colliderDebugDraw;
    }
    // if (isPlatformAndroid()) {
    //     // check for long press
    //     if (isShakeDetected()) {
    //         resetShakeDetected();
    //         colliderDebugDraw = !colliderDebugDraw;
    //     }
    // }
    // if (!form->isKeyWarriorAlive()) gameover = true;
    if (!region->castle->isAlive()) {
        endGame();
        gameOverSet();
    }
    if (hmm->isVictory()) {
        endGame();
        victorySet();
    }
    return true;
}

void Container::gameOverSet() {
    getAudioManager()->switchBGM("sad");
    gameover = true;
    setScoreBoard(getScoreBoard());
    getWorldState()->finalize();
    getGameOver()->reset();
    getStateStack()->push(getGameOver());
    sendFirebaseEvent("GameOver", {});
}

std::shared_ptr<ScoreBoard> Container::getScoreBoard() {
    return std::make_shared<ScoreBoard>(
        getWorldState()->getScore(), getWorldState()->getTotalKills(),
        getWorldState()->getTimeSurvived(), hmm->getCurrentWave());
}

void Container::victorySet() {
    getAudioManager()->switchBGM("victory");
    victory = true;
    // initialize scoreboard here
    setScoreBoard(getScoreBoard());
    getWorldState()->finalize();
    getVictory()->reset();
    getStateStack()->push(getVictory());
    sendFirebaseEvent("Victory", {});
}

void Container::endGame() {

    getViewCamera()->follow(nullptr);
    cleanup();
}

void Container::cleanup() {
    attackUnits.clear();
    tree_patches.clear();
    for (auto &go : gameObjects) {
        go->cleanupData();
        go->die();
    }
    gameObjects.clear();
    for (auto &go : scumObjects) {
        go->cleanupData();
        go->die();
    }
    scumObjects.clear();
    flashObjects.clear();
    region = nullptr;
    form = nullptr;
}

void Container::addGameObject(std::shared_ptr<GameObject> obj) {
    gameObjects.push_back(obj);
}

void Container::addFlashObject(std::shared_ptr<GameObject> obj) {
    flashObjects.push_back(obj);
}

void Container::addScumObject(std::shared_ptr<GameObject> obj) {
    scumObjects.push_back(obj);
}

void Container::addAttackUnit(std::shared_ptr<Building> obj) {
    attackUnits.push_back(obj);
}

void Container::drawGround() {
    Vector2 dim = getSpriteHolder()->getSpriteSize(TERRAIN_OP_SPRITE_ID);
    float th = VIRTUAL_HEIGHT;
    float tw = VIRTUAL_HEIGHT * dim.x / dim.y;
    float sx = VIRTUAL_WIDTH / 2.0f - tw / 2;
    // middle row
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx, 0, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx + tw, 0, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx - tw, 0, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 2 * tw, 0, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 2 * tw, 0, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 3 * tw, 0, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 3 * tw, 0, tw, th});
    // row + 1
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + tw / 2, th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - tw / 2, th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 3 * tw / 2, th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 3 * tw / 2, th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 5 * tw / 2, th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 5 * tw / 2, th / 2, tw, th});
    // row - 1
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + tw / 2, -th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - tw / 2, -th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 3 * tw / 2, -th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 3 * tw / 2, -th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 5 * tw / 2, -th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 5 * tw / 2, -th / 2, tw, th});
    // row + 2

    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx, th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx + tw, th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx - tw, th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 2 * tw, th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 2 * tw, th, tw, th});

    // row - 2
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx, -th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx + tw, -th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx - tw, -th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 2 * tw, -th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 2 * tw, -th, tw, th});

    // row + 3
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + tw / 2, 3 * th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - tw / 2, 3 * th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 3 * tw / 2, 3 * th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 3 * tw / 2, 3 * th / 2, tw, th});

    // row - 3
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + tw / 2, -3 * th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - tw / 2, -3 * th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + 3 * tw / 2, -3 * th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - 3 * tw / 2, -3 * th / 2, tw, th});

    // row + 4
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx, 2 * th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + tw, 2 * th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - tw, 2 * th, tw, th});

    // row - 4
    //
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx, -2 * th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + tw, -2 * th, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - tw, -2 * th, tw, th});

    // row + 5
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + tw / 2, 5 * th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - tw / 2, 5 * th / 2, tw, th});

    // row - 5

    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx + tw / 2, -5 * th / 2, tw, th});
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID,
                                  {sx - tw / 2, -5 * th / 2, tw, th});

    // row + 6
    //
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx, 3 * th, tw, th});

    // row - 6
    //
    getSpriteHolder()->drawSprite(TERRAIN_OP_SPRITE_ID, {sx, -3 * th, tw, th});
}

std::shared_ptr<GameObject> Container::getFollowObject() {
    // int x = getRandomValue(0, 1);
    // if (x == 0)
    if (form->isKeyWarriorAlive()) return form->getKeyWarrior();

    return form->dummyWarrior;
    // else
    // return castle;
}

Vector2 Container::getFormPos() {
    return Vector2{form->x, form->y};
}
