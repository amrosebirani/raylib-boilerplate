#pragma once
#include "raylib.h"
#include <string>

void startGameLoop();

void slow(float amount, float duration);
void shake(float intensity, float duration, float frequency);
void flash(float duration, Color color);
const Font &GetFont(int id);
bool isPlatformAndroid();
bool isPlatformDesktop();
bool isPlatformWeb();
void setPlatform(std::string platform);
const char *getAssetPath(const char *path);
