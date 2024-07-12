#include "draft.h"
#include <functional>
#include "box2d/b2_math.h"
#include "raylib.h"
#include <vector>

void DrawConvexPolygon(std::vector<Vector2> vertices, DrawMode mode,
                       Color color, float thickness) {
    if (mode == DrawMode::LINE) {
        for (int i = 0; i < vertices.size() - 1; i++) {
            DrawLineEx(vertices[i], vertices[i + 1], thickness, color);
        }
        DrawLineEx(vertices[vertices.size() - 1], vertices[0], thickness,
                   color);
    } else if (mode == DrawMode::FILL) {
        for (int i = 1; i <= vertices.size() - 2; i++) {
            DrawTriangle(vertices[0], vertices[i], vertices[i + 1], color);
        }
    }
}

void rhombus(float cx, float cy, float width, float height, DrawMode mode,
             Color color, float thickness) {
    float wr = width / 2;
    float hr = height / 2;

    std::vector<Vector2> vertices = {
        {cx - wr, cy}, {cx, cy + hr}, {cx + wr, cy}, {cx, cy - hr}};

    DrawConvexPolygon(vertices, mode, color, thickness);
};

void DraftLine(std::vector<Vector2> points, DrawMode mode, Color color,
               float thickness) {
    for (int i = 0; i < points.size() - 1; i++) {
        DrawLineEx(points[i], points[i + 1], thickness, color);
    }
}

void DraftTriangleIsoceles(float cx, float cy, float width, float height,
                           DrawMode mode, Color color, float thickness) {
    float wr = width / 2;
    float hr = height / 2;

    std::vector<Vector2> vertices = {
        {cx - wr, cy + hr}, {cx, cy - hr}, {cx + wr, cy + hr}};

    if (mode == LINE) {
        DrawLineEx(vertices[0], vertices[1], thickness, color);
        DrawLineEx(vertices[1], vertices[2], thickness, color);
        DrawLineEx(vertices[2], vertices[0], thickness, color);
    } else if (mode == FILL) {
        DrawTriangle(vertices[0], vertices[1], vertices[2], color);
    }
}

void DraftTriangleRight(float cx, float cy, float width, float height,
                        DrawMode mode, Color color, float thickness) {
    float wr = width / 2;
    float hr = height / 2;

    std::vector<Vector2> vertices = {
        {cx - wr, cy - hr}, {cx - wr, cy + hr}, {cx + wr, cy + hr}};

    if (mode == LINE) {
        DrawLineEx(vertices[0], vertices[1], thickness, color);
        DrawLineEx(vertices[1], vertices[2], thickness, color);
        DrawLineEx(vertices[2], vertices[0], thickness, color);
    } else if (mode == FILL) {
        DrawTriangle(vertices[0], vertices[1], vertices[2], color);
    }
}

void DraftPolygon(std::vector<Vector2> vertices, DrawMode mode, Color color,
                  float thickness) {
    if (mode == LINE) {
        for (int i = 0; i < vertices.size() - 1; i++) {
            DrawLineEx(vertices[i], vertices[i + 1], thickness, color);
        }
        DrawLineEx(vertices[vertices.size() - 1], vertices[0], thickness,
                   color);
    } else if (mode == FILL) {
        for (int i = 1; i <= vertices.size() - 2; i++) {
            DrawTriangle(vertices[0], vertices[i + 1], vertices[i], color);
        }
    }
}

void DraftTriangleEquilateral(float cx, float cy, float width, DrawMode mode,
                              Color color, float thickness) {
    float height = sqrt(pow(width, 2) - pow(width / 2, 2));
    DraftTriangleIsoceles(cx, cy, width, height, mode, color, thickness);
}

void DraftTrapezoid(float cx, float cy, float width, float height, float top,
                    DrawMode mode, Color color, float thickness) {
    float wr = width / 2;
    float hr = height / 2;
    float tr = top / 2;

    std::vector<Vector2> vertices = {{cx - wr, cy - hr},
                                     {cx + wr, cy - hr},
                                     {cx + tr, cy + hr},
                                     {cx - tr, cy + hr}};

    if (mode == LINE) {
        DrawLineEx(vertices[0], vertices[1], thickness, color);
        DrawLineEx(vertices[1], vertices[2], thickness, color);
        DrawLineEx(vertices[2], vertices[3], thickness, color);
        DrawLineEx(vertices[3], vertices[0], thickness, color);
    } else if (mode == FILL) {
        DrawTriangle(vertices[0], vertices[1], vertices[2], color);
        DrawTriangle(vertices[2], vertices[3], vertices[0], color);
    }
}

void DraftTrapezoid(float cx, float cy, float widht, float height,
                    float widhtTop, float widthTopOffset, DrawMode mode,
                    Color color, float thickness) {
    float wr = widht / 2;
    float hr = height / 2;
    float wtro = widhtTop / 2 + widthTopOffset;

    std::vector<Vector2> vertices = {{cx - wtro, cy - hr},
                                     {cx + wtro, cy - hr},
                                     {cx + wr, cy + hr},
                                     {cx - wr, cy + hr}};

    DraftPolygon(vertices, mode, color, thickness);
}

void DraftTrapezium(float cx, float cy, float widthLeft, float widthRight,
                    float height, float depth, DrawMode mode, Color color,
                    float thickness) {

    std::vector<Vector2> vertices = {{cx - widthLeft, cy},
                                     {cx, cy - height},
                                     {cx + widthRight, cy},
                                     {cx, cy + depth}};

    DraftPolygon(vertices, mode, color, thickness);
}

void DraftGem(float cx, float cy, float widthTop, float widthMiddle,
              float height, float depth, DrawMode mode, Color color,
              float thickness) {
    float wto = widthTop / 2;
    float wmo = widthMiddle / 2;

    std::vector<Vector2> vertices = {{cx - wto, cy - height},
                                     {cx + wto, cy - height},
                                     {cx + wmo, cy - height},
                                     {cx, cy + depth},
                                     {cx - wmo, cy}};

    DraftPolygon(vertices, mode, color, thickness);
}

void DraftDiamond(float cx, float cy, float width, DrawMode mode, Color color,
                  float thickness) {
    float wr = width / 2;
    float depth = sqrt(pow(width, 2) - pow(width / 2, 2)) / 2;
    float height = depth / 2;
    float topOffset = wr / 3 * 2;

    std::vector<Vector2> vertices = {{cx - wr, cy},
                                     {cx - topOffset, cy - height},
                                     {cx + topOffset, cy - height},
                                     {cx + wr, cy},
                                     {cx, cy + depth}};

    DraftPolygon(vertices, mode, color, thickness);
}

void DraftLozenge(float cx, float cy, float width, DrawMode mode, Color color,
                  float thickness) {
    rhombus(cx, cy, width, width * 2, mode, color, thickness);
}

void DraftKite(float cx, float cy, float width, float height, float depth,
               DrawMode mode, Color color, float thickness) {
    DraftTrapezium(cx, cy, width, width, height, depth, mode, color, thickness);
}

void DraftParallelogram(float cx, float cy, float width, float height,
                        float widthOffset, DrawMode mode, Color color,
                        float thickness) {
    DraftTrapezoid(cx, cy, width, height, width, widthOffset, mode, color,
                   thickness);
}

void DraftCompass(float cx, float cy, float width, float arcAngle,
                  float startAngle, int numSegments, bool wrap, DrawMode mode,
                  Color color, float thickness) {
    float radius = width / 2;
    int wrapi = wrap ? 0 : -1;
    float theta = arcAngle / (numSegments - wrapi);
    float cosine = std::cos(theta);
    float sine = std::sin(theta);
    float x = radius * std::cos(startAngle);
    float y = radius * std::sin(startAngle);
    std::vector<Vector2> vertices;
    for (int i = 0; i < numSegments; i++) {
        float vx, vy;
        vx = x;
        vy = y;
        vertices.push_back({vx + cx, vy + cy});
        float t = x;
        x = (cosine * x) - (sine * y);
        y = (sine * t) + (cosine * y);
    }
    if (wrap) {
        DraftPolygon(vertices, mode, color, thickness);
    } else {
        DraftLine(vertices, mode, color, thickness);
    }
}

void DraftCompass(float cx, float cy, float width, float arcAngle,
                  float startAngle, int numSegments, bool wrap,
                  std::function<Vector2(float, float, int, int)> scale,
                  DrawMode mode, Color color, float thickness) {
    float radius = width / 2;
    int wrapi = wrap ? 0 : -1;
    float theta = arcAngle / (numSegments - wrapi);
    float cosine = std::cos(theta);
    float sine = std::sin(theta);
    float x = radius * std::cos(startAngle);
    float y = radius * std::sin(startAngle);
    std::vector<Vector2> vertices;
    for (int i = 0; i < numSegments; i++) {
        Vector2 scaled = scale(x, y, i, numSegments);
        vertices.push_back({scaled.x + cx, scaled.y + cy});
        float t = x;
        x = (cosine * x) - (sine * y);
        y = (sine * t) + (cosine * y);
    }
    if (wrap) {
        DraftPolygon(vertices, mode, color, thickness);
    } else {
        DraftLine(vertices, mode, color, thickness);
    }
}
void DraftCompass(float cx, float cy, float width, float arcAngle,
                  float startAngle, int numSegments, bool wrap, Vector2 scale,
                  DrawMode mode, Color color, float thickness) {
    float radius = width / 2;
    int wrapi = wrap ? 0 : -1;
    float theta = arcAngle / (numSegments - wrapi);
    float cosine = std::cos(theta);
    float sine = std::sin(theta);
    float x = radius * std::cos(startAngle);
    float y = radius * std::sin(startAngle);
    std::vector<Vector2> vertices;
    for (int i = 0; i < numSegments; i++) {
        float vx, vy;
        vx = x * scale.x;
        vy = y * scale.y;
        vertices.push_back({vx + cx, vy + cy});
        float t = x;
        x = (cosine * x) - (sine * y);
        y = (sine * t) + (cosine * y);
    }
    if (wrap) {
        DraftPolygon(vertices, mode, color, thickness);
    } else {
        DraftLine(vertices, mode, color, thickness);
    }
}

void DraftCircle(float cx, float cy, float radius, float numSegments,
                 DrawMode mode, Color color, float thickness) {
    DraftCompass(cx, cy, radius * 2, 2 * M_PI, 0, numSegments, true, mode,
                 color, thickness);
}

void DraftArc(float cx, float cy, float radius, float arcAngle,
              float startAngle, float numSegments, DrawMode mode, Color color,
              float thickness) {
    DraftCompass(cx, cy, radius * 2, arcAngle, startAngle, numSegments, false,
                 mode, color, thickness);
}

void DraftBow(float cx, float cy, float radius, float arcAngle,
              float startAngle, float numSegments, DrawMode mode, Color color,
              float thickness) {
    DraftCompass(cx, cy, radius * 2, arcAngle, startAngle, numSegments, true,
                 mode, color, thickness);
}

void DraftEllipse(float cx, float cy, float width, float height,
                  int numSegments, DrawMode mode, Color color,
                  float thickness) {
    DraftCompass(cx, cy, width, 2 * M_PI, 0, numSegments, true,
                 {1, height / width}, mode, color, thickness);
}

void DraftEllipticArc(float cx, float cy, float width, float height,
                      float arcAngle, float startAngle, int numSegments,
                      DrawMode mode, Color color, float thickness) {
    DraftCompass(cx, cy, width, arcAngle, startAngle, numSegments, false,
                 {1, height / width}, mode, color, thickness);
}

void DraftEllipticBow(float cx, float cy, float width, float height,
                      float arcAngle, float startAngle, int numSegments,
                      DrawMode mode, Color color, float thickness) {
    DraftCompass(cx, cy, width, arcAngle, startAngle, numSegments, true,
                 {1, height / width}, mode, color, thickness);
}

void DraftSemiCircle(float cx, float cy, float radius, float startAngle,
                     int numSegments, DrawMode mode, Color color,
                     float thickness) {
    DraftCompass(cx, cy, radius * 2, M_PI, startAngle, numSegments, true, mode,
                 color, thickness);
}

void DraftEgg(float cx, float cy, float width, float syBottom, float syTop,
              int numSegments, DrawMode mode, Color color, float thickness) {
    std::function<Vector2(float, float, int, int)> scale =
        [syBottom, syTop](float x, float y, int i, int n) {
            if (i < n / 2) {
                return Vector2{x, y * syBottom};
            } else {
                return Vector2{x, y * syTop};
            }
        };
    DraftCompass(cx, cy, width, 2 * M_PI, 0, numSegments, true, scale, mode,
                 color, thickness);
}
