#include "world_state.hpp"
#include "constants.h"
#include "gem_type.hpp"
#include "multiplier_dialog.hpp"
#include "raylib.h"
#include "globals.h"
#include "raymath.h"
#include "platform.hpp"

using std::string;
float lastDistance = 0.0f;
bool isDragging = false;
Vector2 previousDragPosition;

WorldState::WorldState() {
    setBasics();
    gem_progress_bar =
        new ProgressBar(0, topHeight / 2, GetScreenWidth(), topHeight / 2, 1, 0,
                        {54, 137, 179, 255});
    summon_manager = std::make_shared<SummonManager>();
}

void WorldState::setBasics() {

    gem_chance_list = new TempChanceList<GemType>(
        {{GemType::Coin, 7}, {GemType::Diamond, 3}});

    gems_for_next_upgrade.push_back(5);
    for (int i = 1; i < 1024; i = i + 4) {
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i - 1] + 3);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i]);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i + 1] * 1.3);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i + 2]);
    }
    topHeight = GetScreenHeight() * topPercentCover / 100;
    fs = topHeight / 2 * .7f;
    topMargin = topHeight / 2 * .15f;
}

float WorldState::getMiniMapH() {
    return topHeight + fs;
}

void WorldState::draw() {
    DrawRectangle(0, 0, GetScreenWidth(), topHeight / 2, {0, 0, 0, 125});
    const char *tg = TextFormat("%d", gems_for_next_upgrade[gem_round] - gems);
    int tgm = MeasureText(tg, fs);
    DrawText(tg, 20, topMargin, fs, WHITE);
    getSpriteHolder()->drawSprite(COINS_AND_GEMS, 5,
                                  {20.0f + tgm + 5, topMargin, fs, fs});

    DrawText("to next upgrade", 30 + tgm + fs, topMargin, fs, WHITE);

    getSpriteHolder()->drawSprite(
        COINS_AND_GEMS, 1, {GetScreenWidth() - 40.0f - fs, topMargin, fs, fs});
    const char *tt = TextFormat("%d", coins);
    int ttm = MeasureText(tt, fs);
    DrawText(tt, GetScreenWidth() - ttm - 40 - 1.5f * fs, topMargin, fs, WHITE);
    gem_progress_bar->draw();
    summon_manager->draw();
    if (isPlatformAndroid()) {
        getJoystick()->draw();
    }
    // DrawRectangle(0, GetScreenHeight() - 60, GetScreenWidth(), 60,
    // {0, 0, 0, 125});
    std::string waveText = getContainer()->hmm->getWaveText();
    float waveTextWidth = MeasureText(waveText.c_str(), fs);
    DrawRectangleRec({20, topHeight + fs, waveTextWidth + 2 * fs, 3 * fs},
                     {RED.r, RED.g, RED.b, 200});
    DrawText(waveText.c_str(), 20 + fs, topHeight + 2 * fs, fs, WHITE);
    if (is_formation_respawning) {
        // need text in format "Respawn in 3.00"
        std::string formText = "Respawn in ";
        formText += TextFormat("%.1f", formation_respawn_time);
        formText += "s";
        DrawText(formText.c_str(), startX, GetScreenHeight() * 1.0f / 3, 40,
                 WHITE);
    }
}

void WorldState::setSummonEnabled(bool enabled) {
    summon_manager->enabled = enabled;
}

void WorldState::setPopupActive(bool active) {
    popup_active = active;
}

bool WorldState::isPopupActive() {
    return popup_active;
}

void WorldState::setStartX(float ct) {
    startX = ct;

    std::string formText = "Respawn in ";
    formText += TextFormat("%.1f", ct);
    formText += "s";
    float formTextWidth = MeasureText(formText.c_str(), 40);
    startX = GetScreenWidth() * 1.0f / 2 - formTextWidth / 2;
}

WorldState::~WorldState() {
    delete gem_chance_list;
    delete gem_progress_bar;
}

void WorldState::addCoins(int coins) {
    this->coins += coins;
}

void WorldState::addGems(int g) {
    this->gems += g;
    if (this->gems >= gems_for_next_upgrade[gem_round]) {
        this->gems -= gems_for_next_upgrade[gem_round];
        gem_round += 1;
        getStateStack()->push(std::make_shared<MultiplierDialog>());
    }
    gem_progress_bar->setCurrent(getCurrentGemPercent());
    // popup the multiplier dialog here
}

void WorldState::removeCoins(int coins) {
    this->coins -= coins;
}

void WorldState::removeGems(int gems) {
    this->gems -= gems;
}

int WorldState::getCoins() {
    return coins;
}

int WorldState::getGems() {
    return gems;
}

GemType WorldState::getNextGemType() {
    return gem_chance_list->next();
}

float WorldState::getCurrentGemPercent() {
    return (float)gems / gems_for_next_upgrade[gem_round];
}

float GetGesturePinchDistance() {
    if (GetTouchPointCount() >= 2) {
        Vector2 touch1 = GetTouchPosition(0);
        Vector2 touch2 = GetTouchPosition(1);

        // Calculate the distance between the two touch points
        float dx = touch2.x - touch1.x;
        float dy = touch2.y - touch1.y;
        return sqrtf(dx * dx + dy * dy);
    }

    return 0.0f; // Return 0 if fewer than two touches are detected
}

void setPinchZoom() {
    if (IsGestureDetected(GESTURE_PINCH_IN) ||
        IsGestureDetected(GESTURE_PINCH_OUT)) {
        // Pinch zoom detected
        float currentDistance = GetGesturePinchDistance();

        if (lastDistance != 0.0f) {
            // Change zoom level based on the difference in distance
            float zoom = (currentDistance - lastDistance) *
                         0.001f; // Adjust scale factor as needed
            getViewCamera()->editScale(zoom);
        }
        lastDistance = currentDistance;
    } else {
        lastDistance = 0.0f; // Reset distance when pinch is not detected
    }
}

bool checkDragPosition(Vector2 dragPosition) {
    // should not be within joystick area
    Rectangle joyRect = getJoystickRect();
    if (CheckCollisionPointRec(dragPosition, joyRect)) {
        return false;
    }
    return true;
}

void WorldState::setRespawning(bool respawning) {
    is_formation_respawning = respawning;
}

void WorldState::setFormationRespawnTime(float time) {
    formation_respawn_time = time;
}

void dragAndMoveAround() {
    if (IsGestureDetected(GESTURE_DRAG)) {
        Vector2 dragPosition = GetTouchPosition(0);
        if (!checkDragPosition(dragPosition)) {
            return;
        }
        if (!isDragging) {
            isDragging = true;
            previousDragPosition = dragPosition;
        } else {
            Vector2 delta = Vector2Subtract(dragPosition, previousDragPosition);
            getViewCamera()->editPosition(delta);
            previousDragPosition = dragPosition;
        }
    } else {
        isDragging = false;
    }
}

void WorldState::addKill() {
    totalKills++;
}

void WorldState::addScore(int score) {
    this->score += score;
}

int64 WorldState::getScore() {
    return score;
}

int WorldState::getTotalKills() {
    return totalKills;
}

float WorldState::getTimeSurvived() {
    return timeSurvived;
}

bool WorldState::update(float dt) {
    summon_manager->update(dt);
    timeSurvived += dt;
    if (isPlatformAndroid()) {
        getJoystick()->update(dt);
        setPinchZoom();
        // dragAndMoveAround();
    }
    return true;
}

bool WorldState::isFinished() {
    return finished;
}

void WorldState::finalize() {
    finished = true;
}

void WorldState::save(std::ofstream &out) const {
    out.write(reinterpret_cast<const char *>(&coins), sizeof(coins));
    out.write(reinterpret_cast<const char *>(&gems), sizeof(gems));
    out.write(reinterpret_cast<const char *>(&gem_round), sizeof(gem_round));
    out.write(reinterpret_cast<const char *>(&formation_respawn_time),
              sizeof(formation_respawn_time));
    out.write(reinterpret_cast<const char *>(&is_formation_respawning),
              sizeof(is_formation_respawning));
    out.write(reinterpret_cast<const char *>(&totalKills), sizeof(totalKills));
    out.write(reinterpret_cast<const char *>(&score), sizeof(score));
    out.write(reinterpret_cast<const char *>(&timeSurvived),
              sizeof(timeSurvived));
    summon_manager->save(out);
}

WorldState::WorldState(std::ifstream &in) {
    setBasics();
    in.read(reinterpret_cast<char *>(&coins), sizeof(coins));
    in.read(reinterpret_cast<char *>(&gems), sizeof(gems));
    in.read(reinterpret_cast<char *>(&gem_round), sizeof(gem_round));
    in.read(reinterpret_cast<char *>(&formation_respawn_time),
            sizeof(formation_respawn_time));
    in.read(reinterpret_cast<char *>(&is_formation_respawning),
            sizeof(is_formation_respawning));
    in.read(reinterpret_cast<char *>(&totalKills), sizeof(totalKills));
    in.read(reinterpret_cast<char *>(&score), sizeof(score));
    in.read(reinterpret_cast<char *>(&timeSurvived), sizeof(timeSurvived));
    gem_progress_bar =
        new ProgressBar(0, topHeight / 2, GetScreenWidth(), topHeight / 2, 1, 0,
                        {54, 137, 179, 255});
    gem_progress_bar->setCurrent(getCurrentGemPercent());
    summon_manager = std::make_shared<SummonManager>(in);
}
