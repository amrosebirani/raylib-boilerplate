#pragma once

#include "state_params.hpp"
#include "archer.hpp"

struct ArcherStateParams : public StateParams {
        Archer *archer;
        float enemy_x;
        float enemy_y;
        std::shared_ptr<GameObject> enemy;

        ArcherStateParams(Archer *archer) {
            this->archer = archer;
        }
};
