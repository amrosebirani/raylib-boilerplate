#include "fire_ball.hpp"
#include "animation.hpp"
#include "constants.h"
#include "container.h"
#include "game_object.h"
#include "game_object_types.h"
#include "globals.h"
#include "collider_factory.hpp"
#include "raymath.h"
#include <cmath>
#include <fstream>
#include <iostream>

FireBall::FireBall(float x, float y, Vector2 direction)
    : GameObject(x, y), direction(direction) {
    anim = new Animation({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, true, 0.12f);
}

void FireBall::init() {
    data = new ColliderUserData();
    data->type = ColliderUserData::FireBall;
    data->obj = get_shared_ptr();
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    std::vector<std::vector<Vector2>> points;
    rotation = atan2(direction.y, direction.x) * RAD2DEG;
    points.push_back(
        {Vector2{-width / 2, -height / 2}, Vector2{-width / 2, height / 2},
         Vector2{width / 2, height / 2}, Vector2{width / 2, -height / 2}});
    collider = ColliderFactory::newPolygonSensor(
        data, x, y, points, b2_dynamicBody, CATEGORY_PROJECTILE, CATEGORY_ENEMY,
        world);
    Vector2 dirToMove = Vector2Normalize(direction);
    b2Vec2 linearVelocity = {dirToMove.x * mvspd / PIXEL_TO_METER_SCALE,
                             dirToMove.y * mvspd / PIXEL_TO_METER_SCALE};
    collider->SetLinearVelocity(linearVelocity);
    // rotate the collider
    collider->SetTransform(collider->GetPosition(), rotation * DEG2RAD);
}

FireBall::~FireBall() {
    delete data;
    collider = nullptr;
    delete anim;
}

void FireBall::cleanupData() {
    data->obj = nullptr;
}

void FireBall::update(float dt) {
    if (!alive) {
        data->obj = nullptr;
        return;
    }
    tt += dt;
    if (tt >= ttl) {
        die();
    }
    x = collider->GetPosition().x * PIXEL_TO_METER_SCALE;
    y = collider->GetPosition().y * PIXEL_TO_METER_SCALE;
    anim->update(dt);
}

bool FireBall::isAlive() {
    return alive;
}

void FireBall::die() {
    alive = false;
}

void FireBall::draw() {
    if (!alive) {
        return;
    }
    std::cout << "fireball drawn" << std::endl;
    int frame = anim->getCurrentFrame();
    getSpriteHolder()->drawSprite(
        FIRE_BALL_SPRITE_ID, frame,
        {x - width / 2, y - height / 2, width, height}, {width / 2, height / 2},
        rotation);
}

float FireBall::getDamage() {
    return damage *
           getContainer()->getUpgradeContent()->get_stat(CASTLE_DAMAGE_M);
}

void FireBall::Save(std::ofstream &out) const {
}

GameObjectType FireBall::getObjectType() {
    return GameObjectType::FIREBALL;
}
