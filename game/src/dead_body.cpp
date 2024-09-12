#include "dead_body.h"
#include "globals.h"
#include "utils.h"
#include "warrior_types.h"

DeadBody::DeadBody(float x, float y, bool isWarrior, EnemyType enemyType,
                   WarriorType warriorType)
    : GameObject(x, y), isWarrior(isWarrior) {
    this->enemyType = enemyType;
    this->warriorType = warriorType;
    init();
}

void DeadBody::draw() {

    getSpriteHolder()->drawSpriteWithColor(
        sprite_id, frame_no, {x - size, y - size, 2 * size, 2 * size},
        drawColor);
}

void DeadBody::update(float dt) {
    total_time += dt;
    drawColor.a = 255 - (total_time / 2.0f) * 255;
    timer.update(dt);
}

bool DeadBody::isAlive() {
    return alive;
}

void DeadBody::die() {
    alive = false;
}

void DeadBody::init() {
    if (isWarrior) {
        size = 2 * get_warrior_size(warriorType);
    } else {
        size = 2 * get_enemy_size(enemyType);
    }
    if (isWarrior) {
        if (warriorType == WarriorType::WARRIOR_TYPE_SPEARMAN) {
            frame_no = getRandomIntInRange(0, 2);
        } else {
            frame_no = getRandomIntInRange(0, 7);
        }
        sprite_id = get_warrior_sprite_ids(warriorType)[2];
    } else {
        frame_no = getRandomIntInRange(0, 2);
        sprite_id = get_enemy_sprite_ids(enemyType)[1];
    }
    timer.after(2.0f, [this](float dt) { die(); }, "");
}

void DeadBody::cleanupData() {
}
