#include "warrior_factory.h"
// #include "giant.hpp"
#include "shield_bearer.hpp"
#include "warrior_types.h"
#include "spearman.h"
#include "swordsman.h"

std::shared_ptr<Warrior>
WarriorFactory::createWarrior(WarriorType type, float rel_x, float rel_y) {
    switch (type) {
    case WarriorType::WARRIOR_TYPE_SHIELD_BEARER: {
        return std::make_shared<ShieldBearer>(rel_x, rel_y);
    }
    // case WarriorType::WARRIOR_TYPE_GIANT: {
    // return std::make_shared<Giant>(rel_x, rel_y);
    // }
    case WarriorType::WARRIOR_TYPE_SWORDSMAN: {
        return std::make_shared<Swordsman>(rel_x, rel_y);
    }
    case WarriorType::WARRIOR_TYPE_SPEARMAN: {
        return std::make_shared<Spearman>(rel_x, rel_y);
    }
    default: {
        return std::make_shared<ShieldBearer>(rel_x, rel_y);
    }
    }
}
