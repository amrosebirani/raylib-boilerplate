#include "formation.h"
#include "box2d/b2_body.h"
#include "collider_factory.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "globals.h"
#include "level_config.h"
#include "warrior_types.h"
#include "utils.h"
#include "warrior_factory.h"
#include "warrior_para.hpp"
#include "raymath.h"
#include "platform.hpp"

Formation::Formation(int level, float x, float y, float starter_radius,
                     float orbit_margin)
    : level(level), x(x), y(y), starter_radius(starter_radius),
      orbit_margin(orbit_margin) {
    // create the collider here
    // collider will need to be updated every time a warrior is dead or a new
    // warrior is added
    setColliders();
}

void Formation::setColliders() {
    LevelConfig *config = get_level_config(level);
    collider_data = new ColliderUserData();
    collider_data->type = ColliderUserData::Type::Formation;
    collider = ColliderFactory::newCircleCollider(
        collider_data, x, y, get_warrior_size(config->key_warrior),
        b2_dynamicBody, CATEGORY_FORMATION, CATEGORY_BUILDING,
        getContainer()->getWorld());
    collider->SetFixedRotation(true);
    collider->SetBullet(true);
}

void Formation::initOrbits() {
    LevelConfig *config = get_level_config(level);
    keyWarrior = WarriorFactory::createWarrior(config->key_warrior, 0, 0, true);
    keyWarrior->init();
    dummyWarrior = std::make_shared<WarriorDummy>(
        get_warrior_size(config->key_warrior), 0, 0, config->key_warrior);
    dummyWarrior->init();
    collider_data->obj = keyWarrior->get_shared_ptr();
    float sr = get_warrior_size(config->key_warrior);
    sr += orbit_margin;
    for (OrbitConfig *orbitConfig : config->orbits) {
        FormationOrbit *orbit =
            new FormationOrbit(sr, orbitConfig->type, orbitConfig->count);
        orbits.push_back(orbit);
        sr += orbit_margin;
        sr += get_warrior_size(orbitConfig->type) * 2;
    }
}

Formation::~Formation() {
    for (FormationOrbit *&orbit : orbits) {
        for (FormationOrbit::WarriorSlot &slot : orbit->slots) {
            if (slot.warrior != nullptr) {
                slot.warrior->cleanupData();
                slot.warrior = nullptr;
            }
        }
        delete orbit;
    }
    if (keyWarrior != nullptr) {
        keyWarrior->cleanupData();
        keyWarrior = nullptr;
    }
    collider_data->obj = nullptr;
    delete collider_data;
    collider = nullptr;
}

b2Fixture *Formation::addFormationFixture(b2FixtureDef *fixtureDef) {
    return collider->CreateFixture(fixtureDef);
    delete fixtureDef->shape;
    delete fixtureDef;
}

void Formation::removeFormationFixture(b2Fixture *fixture) {
    collider->DestroyFixture(fixture);
}

float Formation::getFormationRadii() {
    float lastOrbitRadius = orbits.back()->starter_radius + orbit_margin +
                            get_warrior_size(orbits.back()->type) * 2;
    return lastOrbitRadius;
}

void Formation::appendWarriors(int count) {
    int totalToAdd = count;
    for (FormationOrbit *&orbit : orbits) {
        for (FormationOrbit::WarriorSlot &slot : orbit->slots) {
            if (slot.warrior == nullptr) {
                slot.warrior = WarriorFactory::createWarrior(
                    orbit->type, slot.x, slot.y, true);
                slot.warrior->init();
                totalToAdd--;
            }
            if (totalToAdd == 0) {
                return;
            }
        }
    }

    // now we need to add new orbits here

    WarriorType moreWarriorsType = get_level_config(level)->expansion_warrior;

    float lastOrbitRadius = orbits.back()->starter_radius + orbit_margin +
                            get_warrior_size(orbits.back()->type) * 2;
    while (totalToAdd > 0) {
        float angle_per_warrior =
            2 * std::asin(
                    get_warrior_size(moreWarriorsType) /
                    (lastOrbitRadius + get_warrior_size(moreWarriorsType))) +
            (PI / 12.0f) / (orbits.size() + 1);
        int orbit_size = floor(2 * PI / angle_per_warrior);

        FormationOrbit *orbit =
            new FormationOrbit(lastOrbitRadius, moreWarriorsType, orbit_size,
                               std::min(orbit_size, totalToAdd));
        orbits.push_back(orbit);
        lastOrbitRadius += orbit_margin;
        lastOrbitRadius += get_warrior_size(moreWarriorsType) * 2;
        totalToAdd -= orbit_size;
    }
}

void Formation::keyBoardMove() {

    float x_move = 0;
    float y_move = 0;
    if (IsKeyDown(KEY_W)) {
        y_move -= 1;
    }
    if (IsKeyDown(KEY_S)) {
        y_move += 1;
    }
    if (IsKeyDown(KEY_A)) {
        x_move -= 1;
    }
    if (IsKeyDown(KEY_D)) {
        x_move += 1;
    }
    dir_to_move = {x_move, y_move};
    if (x_move == 0 && y_move == 0) {
        isIdle = true;
    } else {
        isIdle = false;
        if (x_move == 1) {
            if (y_move == 1) {
                directionFacing = Direction::SOUTH_EAST;
            } else if (y_move == -1) {
                directionFacing = Direction::NORTH_EAST;
            } else {
                directionFacing = Direction::EAST;
            }
        } else if (x_move == -1) {
            if (y_move == 1) {
                directionFacing = Direction::SOUTH_WEST;
            } else if (y_move == -1) {
                directionFacing = Direction::NORTH_WEST;
            } else {
                directionFacing = Direction::WEST;
            }
        } else {
            if (y_move == 1) {
                directionFacing = Direction::SOUTH;
            } else {
                directionFacing = Direction::NORTH;
            }
        }
    }
    dir_to_move = Vector2Normalize(dir_to_move);
}

void Formation::joyStickMove() {
    Vector2 dir = getJoystick()->getDirection();
    if (dir.x == 0 && dir.y == 0) {
        isIdle = true;
    } else {
        isIdle = false;
    }

    directionFacing = get_direction(dir);
    dir_to_move = dir;
}

void Formation::update(float dt) {
    x = collider->GetPosition().x * PIXEL_TO_METER_SCALE;
    y = collider->GetPosition().y * PIXEL_TO_METER_SCALE;
    mvspd = FORMATION_BASE_MVSPD *
            getContainer()->getUpgradeContent()->get_stat(HERO_SPEED_M);
    if (isPlatformAndroid()) {
        joyStickMove();
    } else {
        keyBoardMove();
    }
    if (!isIdle) {
        getAudioManager()->playSound("troop_walking");
    } else {
        getAudioManager()->stopSound("troop_walking");
    }
    b2Vec2 linear_velocity = {dir_to_move.x * mvspd / PIXEL_TO_METER_SCALE,
                              dir_to_move.y * mvspd / PIXEL_TO_METER_SCALE};
    collider->SetLinearVelocity(linear_velocity);
    for (FormationOrbit *&orbit : orbits) {
        orbit->update(dt, {x, y}, isIdle, directionFacing);
    }
    if (isKeyWarriorAlive()) {

        keyWarrior->x = x;
        keyWarrior->y = y;
        keyWarrior->isIdle = isIdle;
        keyWarrior->directionFacing = directionFacing;
        keyWarrior->update(dt);
    }
    dummyWarrior->x = x;
    dummyWarrior->y = y;
    dummyWarrior->isIdle = isIdle;
    dummyWarrior->directionFacing = directionFacing;
    dummyWarrior->update(dt);
    if (!respawning && !isKeyWarriorAlive()) {
        setRespawning();
    }
    if (respawning) {
        respawnTracker += dt;
        getWorldState()->setFormationRespawnTime(respawnCT - respawnTracker);
        if (respawnTracker >= respawnCT) {
            respawning = false;
            getWorldState()->setRespawning(false);
            getWorldState()->setFormationRespawnTime(0);
            getWorldState()->setSummonEnabled(true);
            respawnFormation();
            getViewCamera()->follow(keyWarrior);
            respawnTracker = 0;
        }
    }
}

float Formation::getMvSpd() {
    return mvspd;
}

void Formation::draw() {
    if (!isKeyWarriorAlive()) {
        dummyWarrior->draw();
    } else {

        keyWarrior->draw();
    }
    for (FormationOrbit *&orbit : orbits) {
        orbit->draw();
    }
}

void Formation::clearOtherUnits() {
    for (FormationOrbit *&orbit : orbits) {
        for (FormationOrbit::WarriorSlot &slot : orbit->slots) {
            slot.eliminate();
        }
    }
}

void Formation::respawnFormation() {
    keyWarrior->cleanupData();
    keyWarrior = nullptr;

    LevelConfig *config = get_level_config(level);
    keyWarrior = WarriorFactory::createWarrior(config->key_warrior, 0, 0, true);
    keyWarrior->init();
    keyWarrior->x = x;
    keyWarrior->y = y;
    keyWarrior->isIdle = isIdle;
    keyWarrior->directionFacing = directionFacing;
    for (FormationOrbit *&orbit : orbits) {
        for (FormationOrbit::WarriorSlot &slot : orbit->slots) {
            slot.bringToLife(x, y, isIdle, directionFacing, orbit->type);
        }
    }
}

void Formation::getGameObjects(
    std::vector<std::shared_ptr<GameObject>> &gameObjects) {
    if (!isKeyWarriorAlive()) {
        gameObjects.push_back(dummyWarrior);
    } else {
        gameObjects.push_back(keyWarrior);
    }
    for (FormationOrbit *&orbit : orbits) {
        for (FormationOrbit::WarriorSlot &slot : orbit->slots) {
            if (slot.warrior != nullptr) {
                gameObjects.push_back(slot.warrior);
            }
        }
    }
}

Formation::FormationOrbit::FormationOrbit(float starter_radius,
                                          WarriorType type, int totalSlots)
    : starter_radius(starter_radius), type(type), totalSlots(totalSlots) {
    starter_angle = randomFloatInRange(0, PI / 4.0f);
    float turn_angle = 2 * PI / totalSlots;
    float dist_origin = starter_radius + get_warrior_size(type);
    for (int i = 0; i < totalSlots; i++) {
        float angle = starter_angle + i * turn_angle;
        float rel_x = dist_origin * cos(angle);
        float rel_y = dist_origin * sin(angle);
        std::shared_ptr<Warrior> warrior =
            WarriorFactory::createWarrior(type, rel_x, rel_y, true);
        warrior->init();
        WarriorSlot slot(rel_x, rel_y, warrior);
        slots.push_back(slot);
    }
}

Formation::FormationOrbit::FormationOrbit(float starter_radius,
                                          WarriorType type, int totalSlots,
                                          int activatedSlots)
    : starter_radius(starter_radius), type(type), totalSlots(totalSlots) {
    starter_angle = randomFloatInRange(0, PI / 4.0f);
    float turn_angle = 2 * PI / totalSlots;
    float dist_origin = starter_radius + get_warrior_size(type);
    for (int i = 0; i < totalSlots; i++) {
        float angle = starter_angle + i * turn_angle;
        float rel_x = dist_origin * cos(angle);
        float rel_y = dist_origin * sin(angle);

        std::shared_ptr<Warrior> warrior = nullptr;
        if (activatedSlots > 0) {
            activatedSlots--;
            warrior = WarriorFactory::createWarrior(type, rel_x, rel_y, true);
            warrior->init();
        }
        WarriorSlot slot(rel_x, rel_y, warrior);
        slots.push_back(slot);
    }
}

void Formation::FormationOrbit::WarriorSlot::respawnWarrior(WarriorType type) {
    respawntype = type;
    respawn = true;
    respawnTime =
        WARRIOR_RESPAWN_TIME *
        getContainer()->getUpgradeContent()->get_stat(HERO_UNIT_RESPAWN_TIME_M);
    respawnTracker = 0;
}

void Formation::FormationOrbit::update(float dt, Vector2 origin, bool isIdle,
                                       Direction directionFacing) {
    for (WarriorSlot &slot : slots) {
        slot.update(dt, origin, isIdle, directionFacing);
        if (slot.warrior == nullptr) {
            continue;
        }
        if (!slot.warrior->isAlive()) {
            slot.warrior->cleanupData();
            slot.warrior = nullptr;
            slot.respawnWarrior(type);
        }
    }
}

void Formation::FormationOrbit::save(std::ofstream &out) const {
    out.write(reinterpret_cast<const char *>(&starter_radius),
              sizeof(starter_radius));
    out.write(reinterpret_cast<const char *>(&starter_angle),
              sizeof(starter_angle));
    out.write(reinterpret_cast<const char *>(&type), sizeof(type));
    out.write(reinterpret_cast<const char *>(&totalSlots), sizeof(totalSlots));
    for (const WarriorSlot &slot : slots) {
        slot.save(out);
    }
}

Formation::FormationOrbit::FormationOrbit(std::ifstream &in) {
    in.read(reinterpret_cast<char *>(&starter_radius), sizeof(starter_radius));
    in.read(reinterpret_cast<char *>(&starter_angle), sizeof(starter_angle));
    in.read(reinterpret_cast<char *>(&type), sizeof(type));
    in.read(reinterpret_cast<char *>(&totalSlots), sizeof(totalSlots));
    for (int i = 0; i < totalSlots; i++) {
        WarriorSlot slot(in);
        slots.push_back(slot);
    }
}

void Formation::FormationOrbit::draw() {
    for (WarriorSlot &slot : slots) {
        slot.draw();
    }
}

void Formation::setLoadRespawning() {

    getViewCamera()->follow(dummyWarrior);
    clearOtherUnits();
    getWorldState()->setRespawning(true);
    getWorldState()->setFormationRespawnTime(respawnCT);
    getWorldState()->setStartX(respawnCT);
    getWorldState()->setSummonEnabled(false);
}

void Formation::setRespawning() {
    respawning = true;
    respawnCT =
        FORMATION_RESPAWN_TIME *
        getContainer()->getUpgradeContent()->get_stat(HERO_RESPAWN_TIME_M);
    getViewCamera()->follow(dummyWarrior);
    clearOtherUnits();
    getWorldState()->setRespawning(true);
    getWorldState()->setFormationRespawnTime(respawnCT);
    getWorldState()->setStartX(respawnCT);
    getWorldState()->setSummonEnabled(false);
}

Formation::FormationOrbit::WarriorSlot::WarriorSlot(std::ifstream &in) {
    in.read(reinterpret_cast<char *>(&x), sizeof(x));
    in.read(reinterpret_cast<char *>(&y), sizeof(y));
    in.read(reinterpret_cast<char *>(&respawn), sizeof(respawn));
    in.read(reinterpret_cast<char *>(&respawnTime), sizeof(respawnTime));
    in.read(reinterpret_cast<char *>(&respawnTracker), sizeof(respawnTracker));
    bool hasWarrior = false;
    in.read(reinterpret_cast<char *>(&hasWarrior), sizeof(hasWarrior));
    if (hasWarrior) {
        warrior = std::make_shared<WarriorPara>(in);
        warrior->init();
    }
}

void Formation::FormationOrbit::WarriorSlot::save(std::ofstream &out) const {
    out.write(reinterpret_cast<const char *>(&x), sizeof(x));
    out.write(reinterpret_cast<const char *>(&y), sizeof(y));
    out.write(reinterpret_cast<const char *>(&respawn), sizeof(respawn));
    out.write(reinterpret_cast<const char *>(&respawnTime),
              sizeof(respawnTime));
    out.write(reinterpret_cast<const char *>(&respawnTracker),
              sizeof(respawnTracker));
    bool hasWarrior = warrior != nullptr;
    out.write(reinterpret_cast<const char *>(&hasWarrior), sizeof(hasWarrior));
    if (hasWarrior) {
        warrior->save(out);
    }
}

void Formation::FormationOrbit::WarriorSlot::eliminate() {
    if (warrior != nullptr) {
        warrior->die();
        warrior->cleanupData();
        warrior = nullptr;
    }
    respawn = false;
    respawnTracker = 0;
}

void Formation::FormationOrbit::WarriorSlot::update(float dt, Vector2 origin,
                                                    bool isIdle,
                                                    Direction directionFacing) {
    if (respawn) {
        respawnTracker += dt;
        if (respawnTracker >= respawnTime) {
            warrior = WarriorFactory::createWarrior(respawntype, x, y, true);
            warrior->init();
            respawn = false;
        }
    }
    if (warrior == nullptr) {
        return;
    }
    warrior->x = origin.x + warrior->rel_x;
    warrior->y = origin.y + warrior->rel_y;
    warrior->isIdle = isIdle;
    warrior->directionFacing = directionFacing;
    warrior->update(dt);
}

void Formation::FormationOrbit::WarriorSlot::bringToLife(
    float xx, float yy, bool isIdle, Direction directionFacing,
    WarriorType type) {
    warrior = WarriorFactory::createWarrior(type, x, y, true);
    warrior->init();
    warrior->x = xx + x;
    warrior->y = yy + y;
    warrior->isIdle = isIdle;
    warrior->directionFacing = directionFacing;
}

void Formation::FormationOrbit::WarriorSlot::draw() {
    if (warrior == nullptr) {
        return;
    }
    warrior->draw();
}

std::shared_ptr<Warrior> Formation::getKeyWarrior() {
    return keyWarrior;
}

bool Formation::isKeyWarriorAlive() {
    return keyWarrior->isAlive();
}

void Formation::save(std::ofstream &out) const {
    out.write(reinterpret_cast<const char *>(&level), sizeof(level));
    out.write(reinterpret_cast<const char *>(&x), sizeof(x));
    out.write(reinterpret_cast<const char *>(&y), sizeof(y));
    out.write(reinterpret_cast<const char *>(&starter_radius),
              sizeof(starter_radius));
    out.write(reinterpret_cast<const char *>(&orbit_margin),
              sizeof(orbit_margin));
    out.write(reinterpret_cast<const char *>(&mvspd), sizeof(mvspd));
    // do for respawning, respawnCT, respawnTracker
    out.write(reinterpret_cast<const char *>(&respawning), sizeof(respawning));
    out.write(reinterpret_cast<const char *>(&respawnCT), sizeof(respawnCT));
    out.write(reinterpret_cast<const char *>(&respawnTracker),
              sizeof(respawnTracker));

    // Save orbits
    size_t orbitsSize = orbits.size();
    out.write(reinterpret_cast<const char *>(&orbitsSize), sizeof(orbitsSize));
    for (const auto &orbit : orbits) {
        // Save orbit data
        orbit->save(out);
    }

    // Save key warrior
    bool hasKeyWarrior = keyWarrior != nullptr;
    out.write(reinterpret_cast<const char *>(&hasKeyWarrior),
              sizeof(hasKeyWarrior));
    if (hasKeyWarrior) {
        keyWarrior->save(out);
    }
}

Formation::Formation(std::ifstream &in) {
    in.read(reinterpret_cast<char *>(&level), sizeof(level));
    in.read(reinterpret_cast<char *>(&x), sizeof(x));
    in.read(reinterpret_cast<char *>(&y), sizeof(y));
    in.read(reinterpret_cast<char *>(&starter_radius), sizeof(starter_radius));
    in.read(reinterpret_cast<char *>(&orbit_margin), sizeof(orbit_margin));
    in.read(reinterpret_cast<char *>(&mvspd), sizeof(mvspd));
    in.read(reinterpret_cast<char *>(&respawning), sizeof(respawning));
    in.read(reinterpret_cast<char *>(&respawnCT), sizeof(respawnCT));
    in.read(reinterpret_cast<char *>(&respawnTracker), sizeof(respawnTracker));

    if (respawning) {
        setLoadRespawning();
    }

    // Initialize collider
    setColliders();

    // Load orbits
    size_t orbitsSize;
    in.read(reinterpret_cast<char *>(&orbitsSize), sizeof(orbitsSize));
    for (size_t i = 0; i < orbitsSize; i++) {
        FormationOrbit *orbit = new FormationOrbit(in);
        orbits.push_back(orbit);
    }

    // Load key warrior
    bool hasKeyWarrior;
    in.read(reinterpret_cast<char *>(&hasKeyWarrior), sizeof(hasKeyWarrior));
    if (hasKeyWarrior) {
        keyWarrior = std::make_shared<WarriorPara>(in);
        collider_data->obj = keyWarrior->get_shared_ptr();
    }
    LevelConfig *config = get_level_config(level);
    dummyWarrior = std::make_shared<WarriorDummy>(
        get_warrior_size(config->key_warrior), 0, 0, config->key_warrior);
    dummyWarrior->init();
}
