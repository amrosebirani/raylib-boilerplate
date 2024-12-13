#pragma once

#include "state.hpp"
#include "state_params.hpp"
#include "animation.hpp"
#include "archer_state_params.hpp"
#include "warrior_types.h"

class PlayAnimation : public State {
    public:
        PlayAnimation(WarriorType type, bool hasTower);
        ~PlayAnimation();
        void draw() override;
        void update(float dt) override;
        void Enter(StateParams *params) override;
        void Exit() override;

    private:
        Animation *animation = nullptr;
        ArcherStateParams *archerParams = nullptr;
        WarriorType type;
        bool hasTower;
};
