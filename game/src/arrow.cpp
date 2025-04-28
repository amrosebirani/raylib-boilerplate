#include "arrow.hpp"
#include "collider_factory.hpp"
#include "constants.h"
#include "container.h"
#include "game_object_types.h"
#include "globals.h"
#include "raymath.h"

Arrow::Arrow(float x, float y, Vector2 direction, float damage)
    : GameObject(x, y), direction(direction), damage(damage) {
}

void Arrow::init() {
    data = new ColliderUserData();
    data->type = ColliderUserData::Projectile;
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
    // damage = get_warrior_damage(type);
    getAudioManager()->playRandomSwishSound();
}

Arrow::~Arrow() {
    delete data;
    collider = nullptr;
}

void Arrow::cleanupData() {
    data->obj = nullptr;
}

void Arrow::update(float dt) {
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
}

bool Arrow::isAlive() {
    return alive;
}

void Arrow::die() {
    alive = false;
}

void Arrow::draw() {
    if (!alive) return;
    Vector2 ss = getSpriteHolder()->getSpriteSize(ARROW_SPRITE_ID);
    getSpriteHolder()->drawSprite(
        ARROW_SPRITE_ID, {x - radius, y - radius, 2 * radius, 2 * radius},
        {radius, radius}, rotation);
}

float Arrow::getDamage() {
    return damage;
}

void Arrow::Save(std::ofstream &out) const {
}

GameObjectType Arrow::getObjectType() {
    return GameObjectType::ARROW;
}
