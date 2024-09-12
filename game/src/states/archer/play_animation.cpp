#include "states/archer/play_animation.hpp"
#include "arrow.hpp"
#include "constants.h"
#include "globals.h"
#include "utils.h"

PlayAnimation::PlayAnimation() {
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
    getSpriteHolder()->drawSprite(
        ARCHER_ATTACKS_SPRITE_ID, frame,
        {archer->x - cr, archer->y - 2 * cr + .2f * cr, 2 * cr, 2 * cr});
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

    int d = get_direction_rows()[archerParams->archer->directionAttacking];
    int sf = d * 3;
    animation = new Animation({sf, sf + 1, sf + 2}, false);
}

void PlayAnimation::Exit() {
    delete animation;
    animation = nullptr;
    // here also need to create the arrow projectile
    // and add it to the container
    std::shared_ptr<Arrow> arrow = nullptr;
    if (archerParams->enemy->isAlive()) {
        arrow = std::make_shared<Arrow>(
            archerParams->archer->x, archerParams->archer->y,
            Vector2{archerParams->enemy->x - archerParams->archer->x,
                    archerParams->enemy->y - archerParams->archer->y});
    } else {
        arrow = std::make_shared<Arrow>(
            archerParams->archer->x, archerParams->archer->y,
            Vector2{archerParams->enemy_x - archerParams->archer->x,
                    archerParams->enemy_y - archerParams->archer->y});
    }
    archerParams->enemy = nullptr;
    arrow->init();
    getContainer()->addGameObject(arrow);
}
