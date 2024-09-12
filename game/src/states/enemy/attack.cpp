#include "states/enemy/attack.hpp"
#include "enemy_types.h"
#include "globals.h"
#include "state_params.hpp"
#include "utils.h"

AttackingEnemy::AttackingEnemy() {
}

AttackingEnemy::~AttackingEnemy() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
}

void AttackingEnemy::draw() {
    int frame = animation->getCurrentFrame();
    Enemy *enemy = enemyParams->enemy;
    std::string sprite_id = get_enemy_sprite_ids(enemyParams->type)[0];
    // draw sprite
    float size = 2 * get_enemy_size(enemyParams->type);
    bool flipped = false;
    Direction d = enemy->directionFacing;
    if (d == Direction::NORTH_EAST || d == Direction::EAST ||
        d == Direction::SOUTH_EAST) {
        flipped = true;
    }
    getSpriteHolder()->drawSprite(
        sprite_id, frame,
        {enemy->x - size, enemy->y - size, 2 * size, 2 * size}, {0, 0}, 0,
        flipped);
}

void AttackingEnemy::update(float dt) {
    animation->update(dt);
    if (animation->isFinished()) {
        enemyParams->enemy->isAttacking = false;
    }
}

void AttackingEnemy::Enter(StateParams *params) {
    enemyParams = (EnemyStateParams *)params;
    Direction d = enemyParams->enemy->directionFacing;
    int dd = get_direction_rows()[d];
    int sf = 0;
    animation = new Animation({sf, sf + 1, sf + 2}, false, 0.12f);
}

void AttackingEnemy::Exit() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
}
