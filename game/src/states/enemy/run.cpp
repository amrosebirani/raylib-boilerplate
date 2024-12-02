#include "states/enemy/run.hpp"
#include "constants.h"
#include "globals.h"
#include "enemy_types.h"
#include <string>
#include "utils.h"

RunningEnemy::RunningEnemy() {
}

RunningEnemy::~RunningEnemy() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
    if (enemyParams != nullptr) {
        delete enemyParams;
        enemyParams = nullptr;
    }
}

void RunningEnemy::draw() {
    int frame = animation->getCurrentFrame();
    Enemy *enemy = enemyParams->enemy;
    std::string sprite_id = get_enemy_sprite_ids(enemyParams->type)[2];
    // draw sprite
    float size = 2 * get_enemy_size(enemyParams->type);
    getSpriteHolder()->drawSprite(
        sprite_id, frame,
        {enemy->x - size, enemy->y - size, 2 * size, 2 * size});
}

void RunningEnemy::update(float dt) {
    animation->update(dt);
}

void RunningEnemy::Enter(StateParams *params) {
    enemyParams = (EnemyStateParams *)params;
    enemyParams->enemy->mvspd = ENEMY_BASE_MVSPD;
    Direction d = enemyParams->enemy->directionFacing;
    int dd = get_direction_rows()[d];
    int sf = dd * 8;
    animation = new Animation(
        {sf, sf + 1, sf + 2, sf + 3, sf + 4, sf + 5, sf + 6, sf + 7}, true);
}

void RunningEnemy::Exit() {
    enemyParams->enemy->mvspd = 0;
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
    if (enemyParams != nullptr) {
        delete enemyParams;
        enemyParams = nullptr;
    }
}
