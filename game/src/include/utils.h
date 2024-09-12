#pragma once

#include "ParticleSystem.h"
#include "box2d/b2_body.h"
#include "enemy_types.h"
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
                                           ColliderUserData *data);

std::shared_ptr<b2Body> getEnemyCollider(EnemyType wtype, float x, float y,
                                         ColliderUserData *data);

float getMaxCastleHealthByLevel(int level);
float getCastleAttackTimer(int level);

b2FixtureDef *getFormationFixtureDef(float radius, float offset_x,
                                     float offset_y);

std::unordered_map<Direction, int> get_direction_rows();

Direction get_direction(Vector2 dir);

std::shared_ptr<ParticleSystem> getParticleSystem(Texture2D *texture,
                                                  std::vector<Color> colors);
