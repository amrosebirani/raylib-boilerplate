#include "warrior_factory.h"
// #include "giant.hpp"
#include "warrior_types.h"
#include "warrior_para.hpp"

std::shared_ptr<Warrior> WarriorFactory::createWarrior(WarriorType type,
                                                       float rel_x, float rel_y,
                                                       bool inFormation) {
    return std::make_shared<WarriorPara>(rel_x, rel_y, type, inFormation);
}

std::shared_ptr<Warrior>
WarriorFactory::createWarrior(WarriorType type, float x, float y, int i) {
    return std::make_shared<WarriorPara>(x, y, type);
}
