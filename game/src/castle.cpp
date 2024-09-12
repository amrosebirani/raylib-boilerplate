#include "castle.hpp"
#include "box2d/b2_body.h"
#include "collider_factory.hpp"
#include "collider_user_data.h"
#include "constants.h"
#include "globals.h"
#include "utils.h"
#include "raylib.h"

Castle::Castle(float x, float y, float health, int level)
    : GameObject(x, y), health(health), level(level) {
    graphics_types.push_back("castle");
    maxHealth = getMaxCastleHealthByLevel(level);
    changeAttackTimeout = getCastleAttackTimer(level);
    float region_dimension = CASTLE_WIDTH + DEFAULT_PROPERTY_RINGS *
                                                (ROAD_WIDTH + PROPERTY_WIDTH) *
                                                2;
    region = new Region(x, y, region_dimension, region_dimension, 0, 0, 0, 0);
    region->init();
}

void Castle::init() {

    data = new ColliderUserData();
    data->type = ColliderUserData::Castle;
    data->obj = get_shared_ptr();
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    std::vector<std::vector<Vector2>> points;
    float rx = width / 2718;
    float ry = height / 1516;
    points.push_back({Vector2{-1073 * rx, 234 * ry},
                      Vector2{-97 * rx, 730 * ry}, Vector2{903 * rx, 218 * ry},
                      Vector2{-73 * rx, -94 * ry}});
    collider = ColliderFactory::newPolygonCollider(
        data, VIRTUAL_WIDTH / 2.0f, VIRTUAL_HEIGHT / 2.0f, points,
        b2_staticBody, CATEGORY_CASTLE,
        CATEGORY_ENEMY | CATEGORY_ENEMY_PROJECTILE | CATEGORY_FORMATION, world);
    healthBar = std::make_shared<ProgressBar>(x - 15, y - height / 2, 30, 3,
                                              maxHealth, health);
}

Castle::~Castle() {
    delete data;
    delete region;
    collider = nullptr;
}

void Castle::cleanupData() {
    data->obj = nullptr;
}

void Castle::update(float dt) {
    if (!alive) {
        data->obj = nullptr;
        return;
    }
    collider->SetAwake(true);
    if (health > .5f * maxHealth) {
        state = CastleState::OPERATIONAL;
    } else if (health <= .5f * maxHealth && health > 0) {
        state = CastleState::UNOPERATIONAL;
    } else {
        state = CastleState::CAPTURED;
    }
    if (isAttacked) {
        if (state == CastleState::OPERATIONAL) {
            state = CastleState::UNOPERATIONAL_UNDER_ATTACK;
        }
        if (state == CastleState::UNOPERATIONAL) {
            state = CastleState::UNOPERATIONAL_UNDER_ATTACK;
        }
    }
    if (health <= 0) {
        die();
    }
    healthBar->setCurrent(health);
    changeAttackCounter += dt;
    if (changeAttackCounter >= changeAttackTimeout) {
        changeAttackCounter = 0;
        isAttacked = false;
    }
    region->setCurrentHeightAndWidth(health / maxHealth);
    region->update(dt);
}

void Castle::hit(float damage) {
    isAttacked = true;
    health -= damage;
}

void Castle::draw() {
    // region->draw();
    // DrawRectangle(x - width / 2, y - height / 2, width, height, RAYWHITE);
    // float mt = MeasureText(TextFormat("Castle"), 20);
    // DrawText("Castle", x - mt / 2, y - 10, 20, BLACK);
    getSpriteHolder()->drawSprite(
        CASTLE_SPRITE_ID, {x - width / 2, y - height / 2, width, height});
    float rx = width / 2178;
    float ry = height / 1516;
    // DrawConvexPolygon({Vector2{x - 1073 * rx, y + 234 * ry},
    // Vector2{x - 97 * rx, y + 730 * ry},
    // Vector2{x + 903 * rx, y + 218 * ry},
    // Vector2{x - 73 * rx, y - 94 * ry}},
    // DrawMode::FILL, WHITE);
    // DrawRectangle(x - width / 2, y - height / 2, width, height,
    //               {255, 255, 255, 100});

    healthBar->draw();
}

bool Castle::isAlive() {
    return alive;
}

void Castle::die() {
    alive = false;
}

Vector2 Castle::getDimensions() {
    return Vector2{width, height};
}

Castle::CastleState Castle::getState() {
    return state;
}
