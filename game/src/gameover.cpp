#include "gameover.hpp"
#include "game.h"
#include "globals.h"
#include "raylib.h"
#include "utils.h"

GameOver::GameOver() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    float rsh = 0.15f * sh;
    rsf = 0.1f * sh;
    rtm = MeasureText(restartText.c_str(), rsf);
    if (sw > sh) {
        restartRect = {sw / 2 - rtm / 2 - 30, .75f * sh, rtm + 60, rsh};

    } else {
        restartRect = {sw / 2 - rtm / 2 - 10, .75f * sh, rtm + 20, rsh};
    }
    rth = .75f * sh + rsh / 2 - rsf / 2;
    sbs = .35f * sh;
    sbh = .3f * .25f * sh;
    sbf = sbh * .8f;
    sbm = sbh;

    gameoverL =
        LoadTexture(getAssetPath("textures/backgrounds/gameover_l.png"));
    gameoverP =
        LoadTexture(getAssetPath("textures/backgrounds/gameover_p.png"));
}

bool GameOver::update(float dt) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, restartRect)) {
            finished = true;
            reinitializeGame();
        }
    }
    return true;
}

bool GameOver::isFinished() {
    return finished;
}

void GameOver::reset() {
    sb = getScoreBoard();
    setScoreBoardVars();
    finished = false;
}

void GameOver::setScoreBoardVars() {

    if (sb != nullptr) {
        scoreText = "Score: " + std::to_string(sb->score);
        killsText = "Kills: " + std::to_string(sb->totalKills);
        waveText = "Waves Survived: " + std::to_string(sb->wavesSurvived);
        // get Xm Ys text from the time Survived which is in secs
        std::string timeString =
            TextFormat("%dmin %dsec", int(sb->timeSurvived) / 60,
                       int(sb->timeSurvived) % 60);
        timeText = "Time Survived: " + timeString;
        scoreTextM = MeasureText(scoreText.c_str(), sbf);
        killsTextM = MeasureText(killsText.c_str(), sbf);
        waveTextM = MeasureText(waveText.c_str(), sbf);
        timeTextM = MeasureText(timeText.c_str(), sbf);
    }
}

GameOver::~GameOver() {

    // UnloadTexture(deadBody);
    UnloadTexture(gameoverL);
    UnloadTexture(gameoverP);
}

void GameOver::drawScoreBoard() {
    if (sb != nullptr) {
        float sw = GetScreenWidth();
        float y = sbs + .1f * sbh;
        DrawText(scoreText.c_str(), sw / 2 - scoreTextM / 2, y, sbf, LIGHTGRAY);
        y += sbm;
        DrawText(killsText.c_str(), sw / 2 - killsTextM / 2, y, sbf, LIGHTGRAY);
        y += sbm;
        DrawText(waveText.c_str(), sw / 2 - waveTextM / 2, y, sbf, LIGHTGRAY);
        y += sbm;
        DrawText(timeText.c_str(), sw / 2 - timeTextM / 2, y, sbf, LIGHTGRAY);
    }
}

void GameOver::draw() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, BLACK);
    if (sw > sh) {
        drawFullScreenTextureWithAspect(gameoverL);
    } else {
        drawFullScreenTextureWithAspect(gameoverP);
    }
    float rw;
    float rh;
    if (sw > sh) {
        rw = sw * .66f;
        rh = sh * .9f;
    } else {
        rw = sw * .9f;
        rh = sh * .9f;
    }
    DrawRectangle((sw - rw) / 2, (sh - rh) / 2, rw, rh, {0, 0, 0, 200});

    DrawRectangleRoundedLinesEx(restartRect, 0.5f, 50, 2, RAYWHITE);
    DrawText(restartText.c_str(),
             restartRect.x + restartRect.width / 2 - rtm / 2, rth, rsf,
             RAYWHITE);
    DrawText("Game Over", sw / 2 - MeasureText("Game Over", .15f * sh) / 2.0f,
             .075f * sh, .15f * sh, RAYWHITE);
    drawScoreBoard();
}
