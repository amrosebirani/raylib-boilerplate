#pragma once

#include <vector>
class Animation {
    public:
        Animation(std::vector<int> frames, bool loop, float spf = 0.42);
        ~Animation();
        void update(float dt);
        bool isFinished();
        int getCurrentFrame();
        void reset();
        void setFrames(std::vector<int> frames);

    private:
        bool loop;
        int index = 0;
        std::vector<int> frames;
        float spf = 0.42;
        float timer = 0;
};
