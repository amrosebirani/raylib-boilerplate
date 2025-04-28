#pragma once
#include "game_object.h"

class Collectible : public GameObject {
    public:
        Collectible(float x, float y) : GameObject(x, y) {};
        virtual void collect() = 0;
        Collectible(std::ifstream &in) : GameObject(in) {};
};
