#pragma once

#include "ParticleSystem.h"
#include "box2d/b2_body.h"
#include "building_data.hpp"
#include "enemy_types.h"
#include "magic_types.hpp"
#include "property_type.hpp"
#include "warrior_types.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <random>
#include <ctime>
#include <memory>
#include "collider_user_data.h"
#include "box2d/b2_fixture.h"
#include "raylib.h"

float randomFloatInRange(float min, float max);
int getRandomValue(int a, int b);
int getRandomIntInRange(int a, int b);

template <typename T> T getRandomElement(const std::vector<T> &vec) {
    if (vec.empty()) {
        throw std::runtime_error(
            "Cannot get a random element from an empty vector.");
    }

    // Random number generation setup
    std::mt19937 rng(
        std::time(nullptr)); // Random number generator initialized with seed
    std::uniform_int_distribution<std::size_t> dist(0, vec.size() - 1);

    // Get a random index and return the element at that index
    return vec[dist(rng)];
}

std::shared_ptr<b2Body> getWarriorCollider(WarriorType wtype, float x, float y,
                                           ColliderUserData *data,
                                           bool inFormation = true);
std::shared_ptr<b2Body> getArcherCollider(WarriorType wtype, float x, float y,
                                          ColliderUserData *data,
                                          bool inFormation = false);
std::shared_ptr<b2Body> getWarriorSensor(WarriorType wtype, float x, float y,
                                         ColliderUserData *data);

std::shared_ptr<b2Body> getEnemyCollider(EnemyType wtype, float x, float y,
                                         ColliderUserData *data);

float getMaxCastleHealthByLevel(int level);
float getCastleAttackTimer(int level);

float getMaxHealthByLevel(int level, PropertyType type);

b2FixtureDef *getFormationFixtureDef(float radius, float offset_x,
                                     float offset_y);

std::unordered_map<Direction, int> get_direction_rows();

Direction get_direction(Vector2 dir);

std::shared_ptr<ParticleSystem> getParticleSystem(Texture2D *texture,
                                                  std::vector<Color> colors);

BuildingData *getBuildingData(std::string building_id);
void setBuildingData();

Vector2 getWorldIsometricCoordinated(Vector2 position);

std::vector<Vector2> getBuildingColliderPolygon(BuildingData *buildingData,
                                                float x, float y,
                                                float perc_cover,
                                                bool isSensor = false);
Vector2 getBuildingUpgradePoint(BuildingData *buildingData, float x, float y,
                                float perc_cover);
Vector2 getReverseWorldIsometricCoordinated(Vector2 position);

int getMaxLevel(PropertyType type);
float getBuildingProduceTime(PropertyType type, int level);
Vector2 getBuildingSummonDim(PropertyType type, int level);
std::vector<WarriorType> getSummonChoices(PropertyType type, int level);
std::vector<MagicType> getMagicChoices(int level);

int getUpgradeCoins(PropertyType type, int level);
std::string getBuildingId(PropertyType type, int level);
void initLevelUpgradeData();
float getPercentCover(PropertyType type, int level);
float getUIPercentCover(PropertyType type, int level);
Rectangle getBuildingTransparencyRect(BuildingData *buildingData, float x,
                                      float y, float perc_cover);
std::vector<Vector2> getAttackTowerArcherPos(int level);
Vector2 translateBuildingPoint(BuildingData *buildingData, float x, float y,
                               float perc_cover, Vector2 pp);

void drawFullScreenTextureWithAspect(Texture2D texture);
std::vector<Vector2> getBuildingUpgradePolygon(BuildingData *buildingData,
                                               float perc_cover);
Vector2 getBuildingUpgradeOrigin(BuildingData *buildingData, float x, float y,
                                 float perc_cover);
Vector2 getBuildingSummonOrigin(BuildingData *buildingData, float x, float y,
                                float perc_cover);
void drawSummonTextRotated(Vector2 pos);
void initSummonText();
