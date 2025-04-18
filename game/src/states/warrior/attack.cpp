#include "states/warrior/attack.hpp"
#include "warrior_types.h"
#include "globals.h"

AttackingWarrior::AttackingWarrior() {
}

AttackingWarrior::~AttackingWarrior() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
}

void AttackingWarrior::draw() {
    int frame = animation->getCurrentFrame();
    Warrior *warrior = warriorParams->warrior;
    std::string sprite_id = get_warrior_sprite_ids(warriorParams->type)[1];
    float size = 2 * get_warrior_size(warriorParams->type);
    bool flipped = false;
    if (warriorParams->type == WarriorType::WARRIOR_TYPE_SPEARMAN) {
        Direction d = warrior->directionFacing;
        if (d == Direction::NORTH_EAST || d == Direction::EAST ||
            d == Direction::SOUTH_EAST) {
            flipped = true;
        }
    }
    getSpriteHolder()->drawSprite(
        sprite_id, frame,
        {warrior->x - size, warrior->y - size, 2 * size, 2 * size}, {0, 0}, 0,
        flipped);
}

void AttackingWarrior::update(float dt) {
    animation->update(dt);
    if (animation->isFinished()) {
        warriorParams->warrior->isAttacking = false;
    }
}

void AttackingWarrior::Enter(StateParams *params) {
    warriorParams = (WarriorStateParams *)params;
    Direction d = warriorParams->warrior->directionFacing;
    int dd = get_direction_rows(warriorParams->type)[d];
    int sf;
    if (warriorParams->type == WarriorType::WARRIOR_TYPE_SPEARMAN) {
        sf = 0;
    } else {
        sf = dd * 3;
    }
    animation = new Animation({sf, sf + 1, sf + 2}, false, 0.12f);
}

void AttackingWarrior::Exit() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
}
