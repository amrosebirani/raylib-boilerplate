#include "utils.h"
#include "box2d/b2_circle_shape.h"
#include "collider_user_data.h"
#include "collider_factory.hpp"
#include "direction.hpp"
#include "enemy_types.h"
#include "globals.h"
#include "constants.h"
#include "box2d/b2_body.h"
#include "warrior_types.h"
#include "ParticleSystem.h"

int getRandomValue(int value1, int value2) {
    // Initialize random seed the first time this function is called
    static int seedInitialized = 0;
    if (!seedInitialized) {
        srand(time(NULL)); // Use current time as seed for random generator
        seedInitialized = 1;
    }

    // Generate a random number (0 or 1) and return value1 or value2 accordingly
    if (rand() % 2 == 0) {
        return value1;
    } else {
        return value2;
    }
}

int getRandomIntInRange(int value1, int value2) {
    // Initialize random seed the first time this function is called
    static int seedInitialized = 0;
    if (!seedInitialized) {
        srand(time(NULL)); // Use current time as seed for random generator
        seedInitialized = 1;
    }

    // Generate a random number between value1 and value2 (inclusive)
    return rand() % (value2 - value1 + 1) + value1;
}

// Function to generate a random floating point number between min and max
float randomFloatInRange(float min, float max) {
    float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min);       /* [min, max] */
}

std::shared_ptr<b2Body> getWarriorCollider(WarriorType wtype, float x, float y,
                                           ColliderUserData *data) {

    data->type = ColliderUserData::Warrior;
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    std::shared_ptr<b2Body> collider = ColliderFactory::newCircleCollider(
        data, x, y, get_warrior_size(wtype), b2_dynamicBody, CATEGORY_WARRIOR,
        CATEGORY_COLLECTIBLE | CATEGORY_ENEMY | CATEGORY_ENEMY_PROJECTILE |
            CATEGORY_CASTLE,
        world);
    return collider;
}

std::shared_ptr<b2Body> getEnemyCollider(EnemyType wtype, float x, float y,
                                         ColliderUserData *data) {

    data->type = ColliderUserData::Enemy;
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    std::shared_ptr<b2Body> collider = ColliderFactory::newCircleCollider(
        data, x, y, get_enemy_size(wtype), b2_dynamicBody, CATEGORY_ENEMY,
        CATEGORY_WARRIOR | CATEGORY_PROJECTILE | CATEGORY_ENEMY |
            CATEGORY_CASTLE | CATEGORY_DEFENSE_TOWER |
            CATEGORY_DEFENSE_TOWER_SENSOR,
        world);
    return collider;
}

float getMaxCastleHealthByLevel(int level) {
    return 10000 + (level - 1) * 500;
}

float getCastleAttackTimer(int level) {
    switch (level) {
    case 1: {
        return 6;
    }
    case 2: {
        return 5.5;
    }
    case 3: {
        return 5.1;
    }
    default: {
        return 2;
    }
    }
}

b2FixtureDef *getFormationFixtureDef(float radius, float offset_x,
                                     float offset_y) {
    b2CircleShape *circle = new b2CircleShape();
    circle->m_radius = radius / PIXEL_TO_METER_SCALE;
    circle->m_p.Set(offset_x / PIXEL_TO_METER_SCALE,
                    offset_y / PIXEL_TO_METER_SCALE);

    b2FixtureDef *fixtureDef = new b2FixtureDef();

    fixtureDef->shape = circle;
    fixtureDef->density = 1.0f;
    fixtureDef->friction = 1.0f;
    fixtureDef->restitution = 1.0f;
    fixtureDef->filter.maskBits = CATEGORY_CASTLE | CATEGORY_DEFENSE_TOWER;
    fixtureDef->filter.categoryBits = CATEGORY_FORMATION;
    return fixtureDef;
}

std::unordered_map<Direction, int> get_direction_rows() {
    std::unordered_map<Direction, int> direction_rows;
    direction_rows[Direction::NORTH] = 3;
    direction_rows[Direction::NORTH_EAST] = 7;
    direction_rows[Direction::EAST] = 2;
    direction_rows[Direction::SOUTH_EAST] = 6;
    direction_rows[Direction::SOUTH] = 0;
    direction_rows[Direction::SOUTH_WEST] = 4;
    direction_rows[Direction::WEST] = 1;
    direction_rows[Direction::NORTH_WEST] = 5;
    return direction_rows;
}

Direction get_direction(Vector2 dir) {
    float angle = atan2(-dir.y, dir.x);
    Direction d = Direction::NORTH;
    if (angle >= -PI / 8 && angle < PI / 8) {
        d = Direction::EAST;
    } else if (angle >= PI / 8 && angle < 3 * PI / 8) {
        d = Direction::NORTH_EAST;
    } else if (angle >= 3 * PI / 8 && angle < 5 * PI / 8) {
        d = Direction::NORTH;
    } else if (angle >= 5 * PI / 8 && angle < 7 * PI / 8) {
        d = Direction::NORTH_WEST;
    } else if (angle >= 7 * PI / 8 || angle < -7 * PI / 8) {
        d = Direction::WEST;
    } else if (angle >= -7 * PI / 8 && angle < -5 * PI / 8) {
        d = Direction::SOUTH_WEST;
    } else if (angle >= -5 * PI / 8 && angle < -3 * PI / 8) {
        d = Direction::SOUTH;
    } else if (angle >= -3 * PI / 8 && angle < -PI / 8) {
        d = Direction::SOUTH_EAST;
    }
    return d;
}

std::shared_ptr<ParticleSystem> getParticleSystem(Texture2D *texture,
                                                  std::vector<Color> colors) {
    std::shared_ptr<ParticleSystem> ps =
        std::make_shared<ParticleSystem>(texture, 1000);
    ps->setEmissionRate(22);
    ps->setParticleLifetime(0.85, 1.5);
    ps->setEmissionArea(
        ParticleSystem::AreaSpreadDistribution::DISTRIBUTION_NORMAL, 4.67, 4.67,
        0, false);
    ps->setSpeed(10, 30);
    ps->setRadialAcceleration(0);
    ps->setLinearDamping(.9, 1.8);
    ps->setDirection(0);
    ps->setSpread(0);
    ps->setSizes({.003f, .014f, .005f});
    ps->setSizeVariation(1.0f);
    ps->setColor(colors);
    ps->setEmitterLifetime(1.1f);
    return ps;
}
