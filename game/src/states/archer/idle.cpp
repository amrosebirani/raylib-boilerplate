#include "states/archer/idle.hpp"
#include "attack_tower.hpp"
#include "constants.h"
#include "globals.h"
#include "animation.hpp"
#include "utils.h"
#include "warrior_types.h"
#include <iostream>

IdleArcher::IdleArcher() {
    attackCooldown = get_warrior_attack_time(WarriorType::WARRIOR_TYPE_ARCHER);
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
    getSpriteHolder()->drawSpriteWithColor(
        ARCHER_IDLE_SPRITE_ID, frame,
        {archer->x - cr, archer->y - 2 * cr + .2f * cr, 2 * cr, 2 * cr},
        {WHITE.r, WHITE.g, WHITE.b, archer->alpha});
}

void IdleArcher::update(float dt) {
    animation->update(dt);
    // here we need to check the queue and see if an attack is possible and then
    // change the state
    if (!archerParams->archer->isOperational) return;

    if (!canAttack) {
        attackCooldownTracker += dt;
        if (attackCooldownTracker >= attackCooldown) {
            canAttack = true;
        }
    }
    std::shared_ptr<AttackTower> tower =
        std::dynamic_pointer_cast<AttackTower>(archerParams->archer->tower);
    if (tower->enemies.size() > 0 && canAttack) {
        while (tower->enemies.size() > 0) {
            // pick out enemy randomly from the list
            auto enemy = tower->enemies[getRandomIntInRange(
                0, tower->enemies.size() - 1)];
            // changing logic
            // if the enemy is in the range of tower, it's in the range of the
            // archer check if the enemy is in range Vector2 ss =
            // getSpriteHolder()->getSpriteSize(TOWER_SPRITE_ID); float kr
            // = 2.0f * DEFENSE_TOWER_RADIUS / ss.x; float distance =
            // Vector2Distance( {archerParams->archer->x,
            // archerParams->archer->y + 260 * kr}, {enemy->x, enemy->y});
            // if (distance >= archerParams->archer->getAttackRange()) {
            // tower->enemies.pop();
            // continue;
            // }
            // change state here and break out of the loop
            std::cout << "Archer attacks!" << std::endl;
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
