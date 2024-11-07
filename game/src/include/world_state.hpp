#pragma once
#include "gem_type.hpp"
#include "progress_bar.hpp"
#include "temp_chance_list.hpp"

class WorldState {
    public:
        WorldState();
        ~WorldState();
        void addCoins(int coins);
        void addGems(int gems);
        void removeCoins(int coins);
        void removeGems(int gems);
        void draw();
        int getCoins();
        int getGems();
        GemType getNextGemType();
        float getCurrentGemPercent();

    private:
        int coins = 100;
        int gems = 0;
        TempChanceList<GemType> *gem_chance_list;
        int gem_round = 0;
        std::vector<int> gems_for_next_upgrade;
        ProgressBar *gem_progress_bar;
};
