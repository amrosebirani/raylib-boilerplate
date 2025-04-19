#pragma once

#include "int.h"
struct ScoreBoard {
        int64 score;
        int totalKills;
        float timeSurvived;
        int wavesSurvived;

        // constructor;

        ScoreBoard(int64 score = 0, int totalKills = 0, float timeSurvived = 0,
                   int wavesSurvived = 0)
            : score(score), totalKills(totalKills), timeSurvived(timeSurvived),
              wavesSurvived(wavesSurvived) {
        }
};
