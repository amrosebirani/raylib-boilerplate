#pragma once
#include "box2d/b2_body.h"
#include "box2d/b2_world_callbacks.h"
#include <memory>
#include "collider_user_data.h"
#include "raylib.h"
#include <vector>

class ColliderFactory {
    public:
        static std::shared_ptr<b2Body>
        newCircleCollider(ColliderUserData *ud, float x, float y, float radius,
                          b2BodyType type, uint16 categoryBits, uint16 maskBits,
                          std::shared_ptr<b2World> world, float density = 1.0f,
                          float friction = 1.0f, float restitution = 0.1f);
        static std::shared_ptr<b2Body>
        newPolygonCollider(ColliderUserData *ud, float x, float y,
                           std::vector<std::vector<Vector2>> points,
                           b2BodyType type, uint16 categoryBits,
                           uint16 maskBits, std::shared_ptr<b2World> world,
                           float density = 1.0f, float friction = 1.0f,
                           float restitution = 0.1f);
};
