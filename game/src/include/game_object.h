#pragma once
#include "timer.h"
#include <time.h>
#include <unordered_map>

class GameObject {
    public:
        virtual void draw() = 0;
        virtual void update(float dt) = 0;
        virtual bool isAlive() = 0;
        virtual void die() = 0;

        float x;
        float y;

        GameObject(float x, float y) : x(x), y(y) {
            timer = Timer();
            creation_time = time(NULL);
        };

        float depth = 50;
        time_t creation_time;
        std::vector<std::string> graphics_types;

    protected:
        Timer timer;
        std::shared_ptr<std::unordered_map<std::string, float>> params;
};
