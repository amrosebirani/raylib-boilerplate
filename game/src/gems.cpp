#include "gems.hpp"
#include "constants.h"
#include "globals.h"
#include "collider_factory.hpp"

Gem::Gem(float x, float y, GemType type) : Collectible(x, y), type(type) {
}

Gem::~Gem() {
    collider = nullptr;
    delete data;
}

void Gem::init() {
    data = new ColliderUserData();
    data->type = ColliderUserData::Collectable;
    data->obj = get_shared_ptr();
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    collider = ColliderFactory::newCircleCollider(
        data, x, y,
        5.0f *
            getContainer()->getUpgradeContent()->get_stat(GOLD_PICKUP_RANGE_M),
        b2_dynamicBody, CATEGORY_COLLECTIBLE,
        CATEGORY_WARRIOR | CATEGORY_BUILDING, world);
}

void Gem::update(float dt) {
    if (!alive) {
        data->obj = nullptr;
        return;
    }
    collider->SetAwake(true);
}

void Gem::cleanupData() {
    data->obj = nullptr;
}

bool Gem::isAlive() {
    return alive;
}

void Gem::die() {
    alive = false;
}

void Gem::draw() {
    switch (type) {
    case GemType::Coin:
        getSpriteHolder()->drawSprite(COINS_AND_GEMS, 1,
                                      {x - 5, y - 5, 10, 10});
        break;
    case GemType::Diamond:
        getSpriteHolder()->drawSprite(COINS_AND_GEMS, 5,
                                      {x - 5, y - 5, 10, 10});
        break;
    }
}

void Gem::collect() {
    // getContainer()->initAppend(1);
    if (type == GemType::Coin) {
        getWorldState()->addCoins(1);
        getAudioManager()->playSound("coin_collected");
    } else {
        getWorldState()->addGems(1);
        getAudioManager()->playSound("gem_collected");
    }
    die();
}
