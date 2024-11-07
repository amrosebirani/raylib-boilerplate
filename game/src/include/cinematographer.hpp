#pragma once
#include "cine_experience.hpp"
#include <queue>
#include <memory>

class Cinematographer {
    public:
        Cinematographer();

        void update(float dt);
        void addExperience(std::shared_ptr<CineExperience> exp);

    private:
        std::queue<std::shared_ptr<CineExperience>> requestQueue;
        std::shared_ptr<CineExperience> currentExp;
};
