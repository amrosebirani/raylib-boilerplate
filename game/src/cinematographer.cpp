#include "cinematographer.hpp"
#include "cine_experience.hpp"

Cinematographer::Cinematographer() {
}

void Cinematographer::update(float dt) {
    if (currentExp != nullptr) {
        if (currentExp->active) {
            currentExp->update(dt);
        } else {
            currentExp = nullptr;
        }
    }

    if (currentExp == nullptr) {
        if (requestQueue.size() > 0) {
            currentExp = requestQueue.front();
            currentExp->startExp();
            requestQueue.pop();
        }
    }
}

void Cinematographer::addExperience(std::shared_ptr<CineExperience> exp) {
    requestQueue.push(exp);
}
