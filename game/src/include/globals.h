#pragma once
#include "ParticleSystem.h"
#include "camera.h"
#include "container.h"
#include "sprite_holder.hpp"
#include "world_state.hpp"
#include <memory>

std::shared_ptr<Container> getContainer();
std::shared_ptr<SpriteHolder> getSpriteHolder();
std::shared_ptr<ParticleSystem> getBloodSplatter();
std::shared_ptr<ParticleSystem> getEnemyBloodSplatter();
std::shared_ptr<CameraEnhanced> getViewCamera();
std::shared_ptr<WorldState> getWorldState();
bool isPlatformAndroid();
bool isPlatformDesktop();
bool isPlatformWeb();
void setPlatform(std::string platform);
const char *getAssetPath(const char *path);
bool isPaused();
void initEnemySprites();
