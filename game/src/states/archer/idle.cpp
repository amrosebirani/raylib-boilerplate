#include "states/archer/idle.hpp"
#include "constants.h"
#include "defense_tower.hpp"
#include "globals.h"
#include "animation.hpp"
#include "raymath.h"
#include "utils.h"

IdleArcher::IdleArcher() {
}

IdleArcher::~IdleArcher() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
}

void IdleArcher::draw() {
    int frame = animation->getCurrentFrame();
    Archer *archer = archerParams->archer;
    float cr = 1.3 * DEFENSE_TOWER_RADIUS;
    getSpriteHolder()->drawSprite(
        ARCHER_IDLE_SPRITE_ID, frame,
        {archer->x - cr, archer->y - 2 * cr + .2f * cr, 2 * cr, 2 * cr});
}

void IdleArcher::update(float dt) {
    animation->update(dt);
    // here we need to check the queue and see if an attack is possible and then
    // change the state

    if (!canAttack) {
        attackCooldownTracker += dt;
        if (attackCooldownTracker >= attackCooldown) {
            canAttack = true;
        }
    }
    std::shared_ptr<DefenseTower> tower =
        std::dynamic_pointer_cast<DefenseTower>(archerParams->archer->tower);
    if (tower->enemies.size() > 0 && canAttack) {
        while (tower->enemies.size() > 0) {
            auto enemy = tower->enemies.front();
            if (!enemy->isAlive()) {
                // enemy is not alive anymore
                tower->enemies.pop();
                continue;
            }
            // check if the enemy is in range
            Vector2 ss = getSpriteHolder()->getSpriteSize(TOWER_SPRITE_ID);
            float kr = 2.0f * DEFENSE_TOWER_RADIUS / ss.x;
            float distance = Vector2Distance(
                {archerParams->archer->x, archerParams->archer->y + 260 * kr},
                {enemy->x, enemy->y});
            if (distance >= archerParams->archer->getAttackRange()) {
                tower->enemies.pop();
                continue;
            }
            // change state here and break out of the loop
            archerParams->enemy_x = enemy->x;
            archerParams->enemy_y = enemy->y;
            archerParams->enemy = enemy;
            archerParams->archer->changeState("PlayAnimation", archerParams);
            // canAttack = false;
            // attackCooldownTracker = 0.0f;
            break;
        }
    }
}

void IdleArcher::Enter(StateParams *params) {
    archerParams = (ArcherStateParams *)params;
    Direction d = archerParams->archer->directionAttacking;
    int dd = get_direction_rows()[d];
    int sf = dd * 3;
    animation = new Animation({sf, sf + 1, sf + 2}, true);
    // now it plays this animation in loop
}

void IdleArcher::Exit() {
    delete animation;
    animation = nullptr;
}
