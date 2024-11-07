#include "castle.hpp"
#include "collider_user_data.h"
#include "globals.h"
#include "utils.h"
#include "raylib.h"

Castle::Castle(float x, float y, float health, int level)
    : Building(x, y, PropertyType::CASTLE, level, health), level(level) {
    graphics_types.push_back("castle");
    maxHealth = getMaxCastleHealthByLevel(level);
    changeAttackTimeout = getCastleAttackTimer(level);
}

void Castle::init() {

    initiate();
    initAuraPoints();
    setUpgradeInfo();
    setColliders();
}

Castle::~Castle() {
}

void Castle::cleanupData() {
    colliderCleanup();
}

void Castle::update(float dt) {
    if (!alive) {
        cdata->obj = nullptr;
        return;
    }
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
    changeAttackCounter += dt;
    if (changeAttackCounter >= changeAttackTimeout) {
        changeAttackCounter = 0;
        isAttacked = false;
    }
    timer.update(dt);
    awakenColliders(dt);
}

void Castle::hit(float damage) {
    isAttacked = true;
    takeDamage(damage);
}

void Castle::repair(float repairAmount) {
    health += repairAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Castle::draw() {
    // DrawRectangle(x - width / 2, y - height / 2, width, height, RAYWHITE);
    // float mt = MeasureText(TextFormat("Castle"), 20);
    // DrawText("Castle", x - mt / 2, y - 10, 20, BLACK);
    // getSpriteHolder()->drawSprite(
    // CASTLE_SPRITE_ID, {x - width / 2, y - height / 2, width, height});
    // DrawConvexPolygon(auraPoints, DrawMode::FILL,
    // {YELLOW.r, YELLOW.g, YELLOW.b, 60});
    getSpriteHolder()->drawSprite(buildingData, x, y, perc_cover,
                                  buildingAlpha);

    // float rx = width / 2178;
    // float ry = height / 1516;
    // DrawConvexPolygon({Vector2{x - 1073 * rx, y + 234 * ry},
    // Vector2{x - 97 * rx, y + 730 * ry},
    // Vector2{x + 903 * rx, y + 218 * ry},
    // Vector2{x - 73 * rx, y - 94 * ry}},
    // DrawMode::FILL, WHITE);
    // DrawRectangle(x - width / 2, y - height / 2, width, height,
    //               {255, 255, 255, 100});
    // DrawCircle(x, y, 5, RED);
    drawUpgradeView(YELLOW);
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

void Castle::onUpgrade(int level) {
}
