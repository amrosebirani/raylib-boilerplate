#pragma once
#include "raylib.h"

void startGameLoop();

void slow(float amount, float duration);
void shake(float intensity, float duration, float frequency);
void flash(float duration, Color color);
const Font &GetFont(int id);
const char *getAssetPath(const char *path);
