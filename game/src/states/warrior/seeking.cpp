#include "states/warrior/seeking.hpp"
#include "constants.h"
#include "direction.hpp"
#include "game_object.h"
#include "globals.h"
#include "raymath.h"
#include "utils.h"

SeekingWarrior::SeekingWarrior() {
}

SeekingWarrior::~SeekingWarrior() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
    if (warriorParams != nullptr) {
        delete warriorParams;
        warriorParams = nullptr;
    }
}

void SeekingWarrior::draw() {
    int frame = animation->getCurrentFrame();
    std::string sprite_id = get_warrior_sprite_ids(warriorParams->type)[3];
    // draw sprite
    float size = 2 * get_warrior_size(warriorParams->type);
    getSpriteHolder()->drawSprite(
        sprite_id, frame,
        {warrior->x - size, warrior->y - size, 2 * size, 2 * size});
}

void SeekingWarrior::update(float dt) {
    if (target == nullptr || !target->isAlive()) {
        std::vector<std::shared_ptr<GameObject>> targets =
            warrior->inRangeEnemyUnits;
        if (targets.empty()) {
            return;
        }
        target = targets[getRandomIntInRange(0, targets.size() - 1)];
    }
    Vector2 dirToMove = {target->x - warriorParams->warrior->x,
                         target->y - warriorParams->warrior->y};
    dirToMove = Vector2Normalize(dirToMove);
    warrior->dirToMove = dirToMove;
    Direction newDir = get_direction(dirToMove);
    if (newDir != warrior->directionFacing) {
        warrior->directionFacing = newDir;
        int dd = get_direction_rows(warriorParams->type)[newDir];
        int sf = dd * 8;
        animation->reset();
        animation->setFrames(
            {sf, sf + 1, sf + 2, sf + 3, sf + 4, sf + 5, sf + 6, sf + 7});
    }
    animation->update(dt);
}

void SeekingWarrior::Enter(StateParams *params) {
    warriorParams = (WarriorStateParams *)params;
    GameObject *go = warriorParams->warrior;
    warrior = dynamic_cast<Warrior *>(go);
    warrior->mvspd = WARRIOR_BASE_MVSPD;
    Direction d = warrior->directionFacing;
    int dd = get_direction_rows(warriorParams->type)[d];
    int sf = dd * 8;
    animation = new Animation(
        {sf, sf + 1, sf + 2, sf + 3, sf + 4, sf + 5, sf + 6, sf + 7}, true);
}

void SeekingWarrior::Exit() {
    warrior->mvspd = 0;
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
    if (warriorParams != nullptr) {
        delete warriorParams;
        warriorParams = nullptr;
    }
}
