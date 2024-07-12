#pragma once
#include <string>
#include "raylib.h"
#include <functional>
enum DrawMode { LINE, FILL };

void rhombus(float cx, float cy, float width, float height, DrawMode mode,
             Color color, float thickness = 1.0f);

void DrawConvexPolygon(std::vector<Vector2> vertices, DrawMode mode,
                       Color color, float thickness = 1.0f);

void DraftLine(std::vector<Vector2> points, DrawMode mode, Color color,
               float thickness = 1.0f);

void DraftTriangleIsoceles(float cx, float cy, float width, float height,
                           DrawMode mode, Color color, float thickness = 1.0f);

void DraftTriangleEquilateral(float cx, float cy, float width, DrawMode mode,
                              Color color, float thickness = 1.0f);

void DraftTriangleRight(float cx, float cy, float width, float height,
                        DrawMode mode, Color color, float thickness = 1.0f);

void DraftPolygon(std::vector<Vector2> vertices, DrawMode mode, Color color,
                  float thickness = 1.0f);

void DraftTrapezoid(float cx, float cy, float width, float height, float top,
                    DrawMode mode, Color color, float thickness = 1.0f);

void DraftTrapezoid(float cx, float cy, float width, float height,
                    float widthTop, float widthTopOffset, DrawMode mode,
                    Color color, float thickness = 1.0f);

void DraftTrapezium(float cx, float cy, float widthLeft, float widthRight,
                    float height, float depth, DrawMode mode, Color color,
                    float thickness = 1.0f);

void DraftGem(float cx, float cy, float widthTop, float widthMiddle,
              float height, float depth, DrawMode mode, Color color,
              float thickness = 1.0f);

void DraftDiamond(float cx, float cy, float width, DrawMode mode, Color color,
                  float thickness = 1.0f);

void DraftLozenge(float cx, float cy, float width, DrawMode mode, Color color,
                  float thickness = 1.0f);

void DraftKite(float cx, float cy, float width, float height, float depth,
               DrawMode mode, Color color, float thickness = 1.0f);

void DraftParallelogram(float cx, float cy, float width, float height,
                        float widthOffset, DrawMode mode, Color color,
                        float thickness = 1.0f);

void DraftCompass(float cx, float cy, float width, float arcAngle,
                  float startAngle, int numSegments, bool wrap, DrawMode mode,
                  Color color, float thickness = 1.0f);

void DraftCompass(float cx, float cy, float width, float arcAngle,
                  float startAngle, int numSegments, bool wrap,
                  std::function<Vector2(float, float, int, int)> scale,
                  DrawMode mode, Color color, float thickness = 1.0f);

void DraftCompass(float cx, float cy, float width, float arcAngle,
                  float startAngle, int numSegments, bool wrap, Vector2 scale,
                  DrawMode mode, Color color, float thickness = 1.0f);

void DraftCircle(float cx, float cy, float radius, float numSegments,
                 DrawMode mode, Color color, float thickness = 1.0f);

void DraftArc(float cx, float cy, float radius, float arcAngle,
              float startAngle, float numSegments, DrawMode mode, Color color,
              float thickness = 1.0f);

void DraftBow(float cx, float cy, float radius, float arcAngle,
              float startAngle, float numSegments, DrawMode mode, Color color,
              float thickness = 1.0f);

void DraftEllipse(float cx, float cy, float width, float height,
                  int numSegments, DrawMode mode, Color color,
                  float thickness = 1.0f);

void DraftEllipticArc(float cx, float cy, float width, float height,
                      float arcAngle, float startAngle, int numSegments,
                      DrawMode mode, Color color, float thickness = 1.0f);

void DraftEllipticBow(float cx, float cy, float width, float height,
                      float arcAngle, float startAngle, int numSegments,
                      DrawMode mode, Color color, float thickness = 1.0f);

void DraftSemiCircle(float cx, float cy, float radius, float startAngle,
                     int numSegments, DrawMode mode, Color color,
                     float thickness = 1.0f);

void DraftEgg(float cx, float cy, float width, float syBottom, float syTop,
              int numSegments, DrawMode mode, Color color,
              float thickness = 1.0f);
