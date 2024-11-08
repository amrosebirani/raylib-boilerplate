#include "states/enemy/cooldown.hpp"
#include "enemy_types.h"
#include "globals.h"
#include "state_params.hpp"
#include "utils.h"

CooldownEnemy::CooldownEnemy() {
}

CooldownEnemy::~CooldownEnemy() {

    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
    if (enemyParams != nullptr) {
        delete enemyParams;
        enemyParams = nullptr;
    }
}

void CooldownEnemy::draw() {
    int frame = animation->getCurrentFrame();
    Enemy *enemy = enemyParams->enemy;
    std::string sprite_id = get_enemy_sprite_ids(enemyParams->type)[3];
    float size = 2 * get_enemy_size(enemyParams->type);
    getSpriteHolder()->drawSprite(
        sprite_id, frame,
        {enemy->x - size, enemy->y - size, 2 * size, 2 * size});
}

void CooldownEnemy::update(float dt) {
    animation->update(dt);
}

void CooldownEnemy::Enter(StateParams *params) {

    enemyParams = (EnemyStateParams *)params;
    enemyParams->enemy->mvspd = 0;
    enemyParams->enemy->mvspd = 25;
    Direction d = enemyParams->enemy->directionFacing;
    int dd = get_direction_rows()[d];
    int sf = dd * 3;
    if (animation == nullptr)
        animation = new Animation({sf, sf + 1, sf + 2}, true);
}

void CooldownEnemy::Exit() {
    enemyParams->enemy->mvspd = 25;
    if (enemyParams != nullptr) {
        delete enemyParams;
        enemyParams = nullptr;
    }
}
