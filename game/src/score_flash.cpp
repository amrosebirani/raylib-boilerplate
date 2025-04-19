#include "score_flash.hpp"

void ScoreFlash::update(float dt) {
    y += vy * dt;
    tt += dt;
    if (tt > ttl) {
        die();
    }
}

void ScoreFlash::draw() {
    DrawText(("+" + std::to_string(score)).c_str(), x, y, 5, WHITE);
}

bool ScoreFlash::isAlive() {
    return alive;
}

void ScoreFlash::die() {
    alive = false;
}

void ScoreFlash::init() {
    // Do nothing
}

void ScoreFlash::cleanupData() {
    // Do nothing
}
