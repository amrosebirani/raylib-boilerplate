#include "world_state.hpp"
#include "constants.h"
#include "gem_type.hpp"
#include "raylib.h"
#include "globals.h"
#include "raymath.h"

using std::string;
float lastDistance = 0.0f;
bool isDragging = false;
Vector2 previousDragPosition;

WorldState::WorldState() {
    gem_chance_list = new TempChanceList<GemType>(
        {{GemType::Coin, 7}, {GemType::Diamond, 3}});

    gems_for_next_upgrade.push_back(5);
    for (int i = 1; i < 1024; i = i + 4) {
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i - 1] + 3);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i]);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i + 1] * 1.3);
        gems_for_next_upgrade.push_back(gems_for_next_upgrade[i + 2]);
    }
    gem_progress_bar =
        new ProgressBar(0, 60, GetScreenWidth(), 60, 1, 0, {54, 137, 179, 255});
    summon_manager = std::make_shared<SummonManager>();
}

void WorldState::draw() {
    DrawRectangle(0, 0, GetScreenWidth(), 120, {0, 0, 0, 125});
    const char *tg = TextFormat("%d", gems_for_next_upgrade[gem_round] - gems);
    int tgm = MeasureText(tg, 30);
    DrawText(tg, 45 - tgm, 15, 30, WHITE);
    getSpriteHolder()->drawSprite(COINS_AND_GEMS, 5, {50, 15, 30, 30});

    DrawText("to next upgrade", 85, 15, 30, WHITE);

    getSpriteHolder()->drawSprite(COINS_AND_GEMS, 1,
                                  {GetScreenWidth() - 60.0f, 15, 30, 30});
    const char *tt = TextFormat("%d", coins);
    int ttm = MeasureText(tt, 30);
    DrawText(tt, GetScreenWidth() - 60.0f - ttm - 5, 15, 30, WHITE);
    gem_progress_bar->draw();
    summon_manager->draw();
    if (isPlatformAndroid()) {
        getJoystick()->draw();
    }
    // DrawRectangle(0, GetScreenHeight() - 60, GetScreenWidth(), 60,
    // {0, 0, 0, 125});
    std::string waveText = getContainer()->hmm->getWaveText();
    float waveTextWidth = MeasureText(waveText.c_str(), 30);
    DrawText(waveText.c_str(), 20, 140, 30, BLACK);
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
    }
    gem_progress_bar->setCurrent(getCurrentGemPercent());
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

bool WorldState::update(float dt) {
    summon_manager->update(dt);
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
