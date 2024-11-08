#pragma once

#include "room.hpp"
#include <vector>
#include <memory>

class StateStack {
    public:
        StateStack();
        void push(std::shared_ptr<Room> room);
        std::shared_ptr<Room> top();
        std::shared_ptr<Room> pop();
        void update(float dt);
        void draw();
        void clear();

    private:
        std::vector<std::shared_ptr<Room>> stack;
};
