#pragma once
#include "gem_type.hpp"
#include "progress_bar.hpp"
#include "temp_chance_list.hpp"
#include "room.hpp"
#include "summon_manager.hpp"

class WorldState : public Room {
    public:
        WorldState();
        ~WorldState();
        void addCoins(int coins);
        void addGems(int gems);
        void removeCoins(int coins);
        void removeGems(int gems);
        void draw() override;
        bool update(float dt) override;
        bool isFinished() override;
        int getCoins();
        int getGems();
        GemType getNextGemType();
        float getCurrentGemPercent();
        std::shared_ptr<SummonManager> summon_manager;
        void finalize();
        void setFormationRespawnTime(float time);
        void setRespawning(bool respawning);
        void setStartX(float ct);
        void setSummonEnabled(bool enabled);
        void setPopupActive(bool active);
        bool isPopupActive();

    private:
        int coins = 100;
        int gems = 0;
        TempChanceList<GemType> *gem_chance_list;
        int gem_round = 0;
        std::vector<int> gems_for_next_upgrade;
        ProgressBar *gem_progress_bar;
        bool finished = false;
        float formation_respawn_time = 0;
        bool is_formation_respawning = false;
        float startX;
        bool popup_active = false;
};
