#include "debug_draw.hpp"
#include "constants.h"
#include "draft.h"

void DrawCircleRaylib(float cx, float cy, float radius, Color color) {
    DrawCircle(cx, cy, radius, color);
}

Color RaylibDebugDraw::Box2dColorToRaylib(const b2Color &color) {
    return Color{(unsigned char)(color.r * 255), (unsigned char)(color.g * 255),
                 (unsigned char)(color.b * 255), 255};
}

void RaylibDebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount,
                                  const b2Color &color) {
    std::vector<Vector2> raylibVertices;
    for (int i = 0; i < vertexCount; i++) {
        raylibVertices.push_back(Vector2{vertices[i].x * PIXEL_TO_METER_SCALE,
                                         vertices[i].y * PIXEL_TO_METER_SCALE});
    }
    DrawConvexPolygon(raylibVertices, DrawMode::FILL,
                      Box2dColorToRaylib(color));
}

void RaylibDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices,
                                       int32 vertexCount,
                                       const b2Color &color) {
    // draw raylib DrawPolygon
    std::vector<Vector2> raylibVertices;
    for (int i = 0; i < vertexCount; i++) {
        raylibVertices.push_back(Vector2{vertices[i].x * PIXEL_TO_METER_SCALE,
                                         vertices[i].y * PIXEL_TO_METER_SCALE});
    }
    DrawConvexPolygon(raylibVertices, DrawMode::FILL,
                      Box2dColorToRaylib(color));
}

void RaylibDebugDraw::DrawCircle(const b2Vec2 &center, float radius,
                                 const b2Color &color) {
    DrawCircleLines(center.x * PIXEL_TO_METER_SCALE,
                    center.y * PIXEL_TO_METER_SCALE,
                    radius * PIXEL_TO_METER_SCALE, Box2dColorToRaylib(color));
}

void RaylibDebugDraw::DrawSolidCircle(const b2Vec2 &center, float radius,
                                      const b2Vec2 &axis,
                                      const b2Color &color) {
    DrawCircleLines(center.x * PIXEL_TO_METER_SCALE,
                    center.y * PIXEL_TO_METER_SCALE,
                    radius * PIXEL_TO_METER_SCALE, Box2dColorToRaylib(color));
}

void RaylibDebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2,
                                  const b2Color &color) {
    DrawLine(p1.x * PIXEL_TO_METER_SCALE, p1.y * PIXEL_TO_METER_SCALE,
             p2.x * PIXEL_TO_METER_SCALE, p2.y * PIXEL_TO_METER_SCALE,
             Box2dColorToRaylib(color));
}

void RaylibDebugDraw::DrawTransform(const b2Transform &xf) {
}

void RaylibDebugDraw::DrawPoint(const b2Vec2 &p, float size,
                                const b2Color &color) {
    DrawCircleRaylib(p.x * PIXEL_TO_METER_SCALE, p.y * PIXEL_TO_METER_SCALE,
                     size, Box2dColorToRaylib(color));
}
