#pragma once

#include "state.hpp"
#include "state_params.hpp"
#include "animation.hpp"
#include "archer_state_params.hpp"

class PlayAnimation : public State {
    public:
        PlayAnimation();
        ~PlayAnimation();
        void draw() override;
        void update(float dt) override;
        void Enter(StateParams *params) override;
        void Exit() override;

    private:
        Animation *animation;
        ArcherStateParams *archerParams;
};
