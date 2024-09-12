#pragma once

#include "box2d/b2_body.h"
#include "box2d/b2_world.h"
struct BodyDeleter {
        void operator()(b2Body *body) const {
            if (body) {
                b2World *w = body->GetWorld();
                if (w == nullptr) {
                    return;
                }

                w->DestroyBody(body);
            }
        }
};
