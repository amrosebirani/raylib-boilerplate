#include "state_stack.hpp"

StateStack::StateStack() {
}

void StateStack::push(std::shared_ptr<Room> room) {
    stack.push_back(room);
}

std::shared_ptr<Room> StateStack::top() {
    return stack.back();
}

std::shared_ptr<Room> StateStack::pop() {
    std::shared_ptr<Room> room = stack.back();
    stack.pop_back();
    return room;
}

void StateStack::update(float dt) {
    // reverse loop and update till update returns false
    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        std::shared_ptr<Room> room = *it;
        if (!room->update(dt)) {
            break;
        }
    }

    // remove finished rooms
    std::vector<size_t> indicesR;
    for (size_t i = 0; i < stack.size(); i++) {
        std::shared_ptr<Room> room = stack[i];
        if (room->isFinished()) {
            indicesR.push_back(i);
        }
    }
    for (size_t i = indicesR.size(); i > 0; --i) {
        size_t index = indicesR[i - 1];
        stack.erase(stack.begin() + index);
    }
}

void StateStack::draw() {
    for (auto it = stack.begin(); it != stack.end(); ++it) {
        std::shared_ptr<Room> room = *it;
        room->draw();
    }
}

void StateStack::clear() {
    stack.clear();
}
