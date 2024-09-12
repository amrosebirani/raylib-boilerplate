#include "giant.hpp"
#include "raylib.h"
#include "warrior_types.h"

void Giant::draw() {
    DrawCircle(x, y, get_warrior_size(WarriorType::WARRIOR_TYPE_GIANT), GREEN);
}

void Giant::update(float dt) {
}

bool Giant::isAlive() {
    return true;
}

void Giant::die() {
}

void Giant::attack() {
    DrawText("Giant attacks!", 10, 10, 20, DARKGRAY);
}

void Giant::takeAttack(float damage) {
}

void Giant::tryAttack(GameObject *enemy) {
}
