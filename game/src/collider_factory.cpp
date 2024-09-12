#include "collider_factory.hpp"
#include "box2d/b2_body.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_settings.h"
#include "box2d/b2_world.h"
#include "constants.h"
#include "body_deleter.h"
#include <cstdint>

std::shared_ptr<b2Body> ColliderFactory::newCircleCollider(
    ColliderUserData *ud, float x, float y, float radius, b2BodyType type,
    uint16 categoryBits, uint16 maskBits, std::shared_ptr<b2World> world,
    float density, float friction, float restitution) {
    b2BodyDef bodyDef;
    bodyDef.type = type;

    bodyDef.position.Set(x / PIXEL_TO_METER_SCALE, y / PIXEL_TO_METER_SCALE);
    b2BodyUserData bb;
    bb.pointer = reinterpret_cast<uintptr_t>(ud);
    bodyDef.userData = bb;

    b2Body *body = world->CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = radius / PIXEL_TO_METER_SCALE;

    b2FixtureDef fixtureDef;

    fixtureDef.shape = &circle;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.filter.maskBits = maskBits;
    fixtureDef.filter.categoryBits = categoryBits;

    body->CreateFixture(&fixtureDef);

    std::shared_ptr<b2Body> sharedBody(body, BodyDeleter());
    return sharedBody;
}

std::shared_ptr<b2Body> ColliderFactory::newPolygonCollider(
    ColliderUserData *ud, float x, float y,
    std::vector<std::vector<Vector2>> polygons, b2BodyType type,
    uint16 categoryBits, uint16 maskBits, std::shared_ptr<b2World> world,
    float density, float friction, float restitution) {
    b2BodyDef bodyDef;
    bodyDef.type = type;
    bodyDef.position.Set(x / PIXEL_TO_METER_SCALE, y / PIXEL_TO_METER_SCALE);

    b2BodyUserData bb;
    bb.pointer = reinterpret_cast<uintptr_t>(ud);
    bodyDef.userData = bb;

    b2Body *body = world->CreateBody(&bodyDef);

    for (auto &points : polygons) {
        b2PolygonShape polygonShape;

        b2Vec2 vertices[points.size()];
        int i = 0;
        for (auto &v2 : points) {
            vertices[i].Set(v2.x / PIXEL_TO_METER_SCALE,
                            v2.y / PIXEL_TO_METER_SCALE);
            i++;
        }

        bool sety = polygonShape.Set(vertices, points.size());

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &polygonShape;
        fixtureDef.friction = friction;
        fixtureDef.density = density;
        fixtureDef.restitution = restitution;
        fixtureDef.filter.maskBits = maskBits;
        fixtureDef.filter.categoryBits = categoryBits;

        body->CreateFixture(&fixtureDef);
    }

    std::shared_ptr<b2Body> sharedBody(body, BodyDeleter());

    return sharedBody;
}
