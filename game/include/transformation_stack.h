#pragma once

#include <memory>
#include "raylib.h"

void pushTransformation(std::shared_ptr<Camera2D> transformation);
void popTransformation();
void beginTransformationStack();
void endTransformation();
Vector2 toWorldCoords(Camera2D cam, float xx, float yy);
Vector2 toCameraCoords(Camera2D cam, float xx, float yy);
Vector2 toWorldCoords(float xx, float yy);
Vector2 toCameraCoords(float xx, float yy);
