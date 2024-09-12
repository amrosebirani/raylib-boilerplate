#include "states/warrior/idle.hpp"
#include "globals.h"
#include "warrior_types.h"
#include <string>

IdleWarrior::IdleWarrior() {
}

IdleWarrior::~IdleWarrior() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
}

void IdleWarrior::draw() {
    int frame = animation->getCurrentFrame();
    Warrior *warrior = warriorParams->warrior;
    std::string sprite_id = get_warrior_sprite_ids(warriorParams->type)[0];
    // draw sprite
    float size = 2 * get_warrior_size(warriorParams->type);
    getSpriteHolder()->drawSprite(
        sprite_id, frame,
        {warrior->x - size, warrior->y - size, 2 * size, 2 * size});
}

void IdleWarrior::update(float dt) {
    animation->update(dt);
}

void IdleWarrior::Enter(StateParams *params) {
    warriorParams = (WarriorStateParams *)params;
    Direction d = warriorParams->warrior->directionFacing;
    int dd = get_direction_rows(warriorParams->type)[d];
    int sf = dd * 3;
    animation = new Animation({sf, sf + 1, sf + 2}, true);
}

void IdleWarrior::Exit() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
}
