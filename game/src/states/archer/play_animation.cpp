#include "states/archer/play_animation.hpp"
#include "arrow.hpp"
#include "constants.h"
#include "globals.h"
#include "utils.h"
#include "warrior_types.h"

PlayAnimation::PlayAnimation(WarriorType type, bool hasTower) {
    this->type = type;
    this->hasTower = hasTower;
}

PlayAnimation::~PlayAnimation() {
    if (animation != nullptr) {
        delete animation;
        animation = nullptr;
    }
}

void PlayAnimation::draw() {
    int frame = animation->getCurrentFrame();
    Archer *archer = archerParams->archer;
    float cr = DEFENSE_TOWER_RADIUS;
    std::string attack_sprite_id = get_warrior_sprite_ids(type)[1];
    float size = 2 * get_warrior_size(type);
    if (hasTower) {
        getSpriteHolder()->drawSprite(
            attack_sprite_id, frame,
            {archer->x - cr, archer->y - 2 * cr + .2f * cr, 2 * cr, 2 * cr});
    } else {
        bool flipped = false;
        if (type == WarriorType::WARRIOR_TYPE_CROSSBOWMAN ||
            type == WarriorType::WARRIOR_TYPE_JAVELINER) {
            Direction d = archer->directionAttacking;
            if (d == Direction::NORTH_WEST || d == Direction::WEST ||
                d == Direction::SOUTH_WEST) {
                flipped = true;
            }
        }
        getSpriteHolder()->drawSprite(
            attack_sprite_id, frame,
            {archer->x - size, archer->y - size, 2 * size, 2 * size}, {0, 0}, 0,
            flipped);
    }
}

void PlayAnimation::update(float dt) {
    animation->update(dt);
    if (animation->isFinished()) {
        archerParams->archer->changeState("Idle", archerParams);
    }
}

void PlayAnimation::Enter(StateParams *params) {
    archerParams = (ArcherStateParams *)params;
    // calculate the direction of attack here
    archerParams->archer->directionAttacking =
        get_direction({archerParams->enemy_x - archerParams->archer->x,
                       archerParams->enemy_y - archerParams->archer->y});
    float atime = get_warrior_attack_time(type);

    int d = get_direction_rows()[archerParams->archer->directionAttacking];
    int sf;
    if (type == WarriorType::WARRIOR_TYPE_ARCHER) {
        sf = d * 3;
    } else {
        sf = 0;
    }
    animation = new Animation({sf, sf + 1, sf + 2}, false, 0.12f * atime);
}

void PlayAnimation::Exit() {
    delete animation;
    animation = nullptr;
    // here also need to create the arrow projectile
    // and add it to the container
    std::shared_ptr<Arrow> arrow = nullptr;
    arrow = std::make_shared<Arrow>(
        archerParams->archer->x, archerParams->archer->y,
        Vector2{archerParams->enemy_x - archerParams->archer->x,
                archerParams->enemy_y - archerParams->archer->y},
        type);
    archerParams->enemy = nullptr;
    arrow->init();
    getContainer()->addGameObject(arrow);
}
