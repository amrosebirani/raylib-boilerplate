#include "animation.hpp"

Animation::Animation(std::vector<int> frames, bool loop, float spf) {
    this->frames = frames;
    this->loop = loop;
    this->spf = spf;
}

void Animation::setFrames(std::vector<int> frames) {
    this->frames = frames;
}

Animation::~Animation() {
}

void Animation::reset() {
    index = 0;
    timer = 0;
}

void Animation::update(float dt) {
    timer += dt;
    if (timer >= spf) {
        timer = 0;
        index++;
        if (loop) {
            if (index >= frames.size()) {
                index = 0;
            }
        } else {
            if (index > frames.size()) index = frames.size();
        }
    }
}

bool Animation::isFinished() {
    return !loop && index == frames.size();
}

int Animation::getCurrentFrame() {
    return frames[index];
}
