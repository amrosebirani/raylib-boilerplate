#include "states/warrior/run.hpp"
#include "globals.h"
#include "warrior_dummy.hpp"
#include "warrior_types.h"
#include <string>

RunningWarrior::RunningWarrior() {
}

RunningWarrior::~RunningWarrior() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
    if (warriorParams != nullptr) {
        delete warriorParams;
        warriorParams = nullptr;
    }
}

void RunningWarrior::draw() {
    int frame = animation->getCurrentFrame();
    GameObject *go = warriorParams->warrior;
    std::string sprite_id = get_warrior_sprite_ids(warriorParams->type)[3];
    // draw sprite
    float size = 2 * get_warrior_size(warriorParams->type);
    getSpriteHolder()->drawSpriteWithColor(
        sprite_id, frame, {go->x - size, go->y - size, 2 * size, 2 * size},
        {255, 255, 255, alpha});
}

void RunningWarrior::update(float dt) {
    animation->update(dt);
}

void RunningWarrior::Enter(StateParams *params) {
    warriorParams = (WarriorStateParams *)params;
    GameObject *go = warriorParams->warrior;
    Direction d;
    Warrior *warrior = dynamic_cast<Warrior *>(go);
    if (warrior != nullptr) {
        d = warrior->directionFacing;
    } else {
        WarriorDummy *warriorDummy = dynamic_cast<WarriorDummy *>(go);
        d = warriorDummy->directionFacing;
        alpha = 100;
    }
    int dd = get_direction_rows(warriorParams->type)[d];
    int sf = dd * 8;
    animation = new Animation(
        {sf, sf + 1, sf + 2, sf + 3, sf + 4, sf + 5, sf + 6, sf + 7}, true);
}

void RunningWarrior::Exit() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
    if (warriorParams != nullptr) {
        delete warriorParams;
        warriorParams = nullptr;
    }
}
