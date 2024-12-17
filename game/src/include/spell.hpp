#pragma once

#include "game_object.h"
#include "magic_types.hpp"
class Spell : public GameObject {
    public:
        Spell(int level, MagicType mtype, float x, float y)
            : level(level), magicType(mtype), GameObject(x, y) {};
        virtual ~Spell() {};
        virtual void execute() = 0;
        virtual void addEnemy(std::shared_ptr<GameObject> enemy) = 0;
        int level;
        MagicType magicType;

    private:
};
