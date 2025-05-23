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
#include "raymath.h"

Formation::Formation(int level, float x, float y, float starter_radius,
                     float orbit_margin)
    : level(level), x(x), y(y), starter_radius(starter_radius),
      orbit_margin(orbit_margin) {
    LevelConfig *config = get_level_config(level);
    collider_data = new ColliderUserData();
    collider_data->type = ColliderUserData::Type::Formation;
    collider = ColliderFactory::newCircleCollider(
        collider_data, x, y, get_warrior_size(config->key_warrior),
        b2_dynamicBody, CATEGORY_FORMATION, CATEGORY_CASTLE,
        getContainer()->getWorld());
    collider->SetFixedRotation(true);
    // create the collider here
    // collider will need to be updated every time a warrior is dead or a new
    // warrior is added
}

void Formation::initOrbits() {
    LevelConfig *config = get_level_config(level);
    keyWarrior = WarriorFactory::createWarrior(config->key_warrior, 0, 0);
    keyWarrior->init();
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
        delete orbit;
    }
    delete collider_data;
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
                slot.warrior =
                    WarriorFactory::createWarrior(orbit->type, slot.x, slot.y);
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

void Formation::update(float dt) {
    x = collider->GetPosition().x * PIXEL_TO_METER_SCALE;
    y = collider->GetPosition().y * PIXEL_TO_METER_SCALE;
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
    Vector2 dir_to_move = {x_move, y_move};
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
    b2Vec2 linear_velocity = {dir_to_move.x * mvspd / PIXEL_TO_METER_SCALE,
                              dir_to_move.y * mvspd / PIXEL_TO_METER_SCALE};
    collider->SetLinearVelocity(linear_velocity);
    for (FormationOrbit *&orbit : orbits) {
        orbit->update(dt, {x, y}, isIdle, directionFacing);
    }
    keyWarrior->x = x;
    keyWarrior->y = y;
    keyWarrior->isIdle = isIdle;
    keyWarrior->directionFacing = directionFacing;
    keyWarrior->update(dt);
}

float Formation::getMvSpd() {
    return mvspd;
}

void Formation::draw() {
    keyWarrior->draw();
    for (FormationOrbit *&orbit : orbits) {
        orbit->draw();
    }
}

void Formation::getGameObjects(
    std::vector<std::shared_ptr<GameObject>> &gameObjects) {
    gameObjects.push_back(keyWarrior);
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
            WarriorFactory::createWarrior(type, rel_x, rel_y);
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
            warrior = WarriorFactory::createWarrior(type, rel_x, rel_y);
            warrior->init();
        }
        WarriorSlot slot(rel_x, rel_y, warrior);
        slots.push_back(slot);
    }
}

void Formation::FormationOrbit::update(float dt, Vector2 origin, bool isIdle,
                                       Direction directionFacing) {
    for (WarriorSlot &slot : slots) {
        if (slot.warrior == nullptr) {
            continue;
        }
        if (!slot.warrior->isAlive()) {
            slot.warrior->cleanupData();
            slot.warrior = nullptr;
        }
        slot.update(dt, origin, isIdle, directionFacing);
    }
}

void Formation::FormationOrbit::draw() {
    for (WarriorSlot &slot : slots) {
        slot.draw();
    }
}

void Formation::FormationOrbit::WarriorSlot::update(float dt, Vector2 origin,
                                                    bool isIdle,
                                                    Direction directionFacing) {
    if (warrior == nullptr) {
        return;
    }
    warrior->x = origin.x + warrior->rel_x;
    warrior->y = origin.y + warrior->rel_y;
    warrior->isIdle = isIdle;
    warrior->directionFacing = directionFacing;
    warrior->update(dt);
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
