#pragma once

#include "cine_experience.hpp"
#include "game_object.h"
#include "timer.h"

class HordeCineExperience : public CineExperience {
    public:
        HordeCineExperience(std::shared_ptr<GameObject> hordeLead);
        void startExp() override;
        void update(float dt) override;

    private:
        Timer timer;
        std::shared_ptr<std::unordered_map<std::string, float>> params;
        std::shared_ptr<GameObject> hordeLead;
        float original_scale;
};
