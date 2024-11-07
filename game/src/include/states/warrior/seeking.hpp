#pragma once

#include "state.hpp"
#include "animation.hpp"
#include "warrior_state_params.hpp"

class SeekingWarrior : public State {
    public:
        SeekingWarrior();
        ~SeekingWarrior();

        void draw() override;
        void update(float dt) override;
        void Enter(StateParams *params) override;
        void Exit() override;

    private:
        Animation *animation = nullptr;
        WarriorStateParams *warriorParams = nullptr;
        std::shared_ptr<GameObject> target = nullptr;
};
