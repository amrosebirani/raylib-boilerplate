#include "horde_manager.hpp"
#include "constants.h"
#include "enemy_horde.hpp"
#include "firebase.hpp"
#include "globals.h"
#include "horde_config.hpp"
#include "property_type.hpp"
#include "raylib.h"
#include "textbox.hpp"
#include "timer.h"

HordeManager::HordeManager(int totalWaves) : totalWaves(totalWaves) {
    timer = Timer();
    timer.after(10, [this](float dt) { this->spawnHorde(); }, "");
}

void HordeManager::launchTutorial(PropertyType type, std::string text,
                                  std::string sprite_id) {
    if (getWorldState()->isPopupActive()) {
        return;
    }
    std::shared_ptr<GameObject> tower =
        getContainer()->region->getTutorialTower(type);
    if (tower != nullptr) {
        getViewCamera()->follow(tower);
        std::shared_ptr<Building> b =
            std::dynamic_pointer_cast<Building>(tower);
        b->highlight();
        getStateStack()->push(std::make_shared<TextBox>(
            text, sprite_id, true, 40, 2, true, -GetScreenWidth() / 4, 8));
    }
}

void HordeManager::launchTutBoxes() {
    if (currentWave == 2) {
        launchTutorial(
            PropertyType::DEFENSE_TOWER,
            "Esteemed Lord, our archers stand prepared, yet our sight "
            "is limited by the horizon. A new tower would extend our "
            "watch and strengthen our defenses. Raise the tower by "
            "visiting this location!!",
            ARCHER_BUST_SPRITE_ID);
    }
    if (currentWave == 3) {
        launchTutorial(
            PropertyType::HOUSE,
            "My lord, our lands are vast but underworked. If we build more "
            "houses, we can attract settlers who will till our fields and fill "
            "our coffers with their tributes. Please start by raising your "
            "first house over here!",
            FOOTMAN_BUST_SPRITE_ID);
    }
    if (currentWave == 4) {
        launchTutorial(
            PropertyType::BARRACKS,
            "My lord, our current forces are spread thin across the realm. By "
            "constructing barracks, we can train and summon additional troops "
            "to strengthen our army. Shall we begin building to ensure our "
            "survival? Please build the barracks over here!",
            SPEARMAN_BUST_SPRITE_ID);
    }
    if (currentWave == 5) {
        launchTutorial(
            PropertyType::ARCHERY,
            "My lord, as our foes adapt, so must we. By constructing an "
            "archery range, we can summon skilled archers to our ranks, "
            "allowing us to strike enemies from a distance. Shall we begin "
            "building to enhance our defenses?",
            FOOTMAN_BUST_SPRITE_ID);
    }
    if (currentWave == 6) {
        if (!getContainer()->region->castleUpgraded) {

            std::shared_ptr<GameObject> castle = getContainer()->region->castle;
            getViewCamera()->follow(castle);
            std::shared_ptr<Building> b =
                std::dynamic_pointer_cast<Building>(castle);
            b->highlight();
            getStateStack()->push(std::make_shared<TextBox>(
                "Esteemed Leader, the tides of war are ever-shifting. By "
                "enhancing our castle, we can fortify our structures, amplify "
                "our troops' might, and harness magical forces to decimate our "
                "enemies with spells.",
                MAGE_BUST_SPRITE_ID, false, 40, 2, true, -GetScreenWidth() / 4,
                8));
            this->timer.after(
                4,
                [this](float dt) {
                    getViewCamera()->follow(getContainer()->getFollowObject());
                },
                "");
        }
    }
}

void HordeManager::update(float dt) {
    if (victory) {
        return;
    }
    timer.update(dt);
    // std::cout << "countToCheck: " << countToCheck << std::endl;
    if (countToCheck == 0) {
        countToCheck = -1;
        currentWave++;
        launchTutBoxes();
        if (currentWave > totalWaves) {
            victory = true;
            return;
        }
        getAudioManager()->switchBGM("normal");
        timer.after(59, [this](float dt) { this->spawnHorde(); }, "");
    }
}

void HordeManager::spawnHorde() {
    sendFirebaseEvent("SpawnHorde", {{"wave", TextFormat("%d", currentWave)}});
    std::vector<HordeConfig *> hordeConfigs =
        getHordeConfigsForLevel(currentWave);
    int totalEnemyCount = 0;
    for (auto &hordeConfig : hordeConfigs) {
        totalEnemyCount +=
            hordeConfig->width * hordeConfig->depth + hordeConfig->leaderCount;
        EnemyHorde *eh = new EnemyHorde(hordeConfig);
        delete eh;
    }
    countToCheck = totalEnemyCount;
    float hh = getContainer()->getCastleHealth();
    if (hh < 0.5) {
        getAudioManager()->switchBGM("under_attack_fucked");
    } else {
        getAudioManager()->switchBGM("under_attack_in_control");
    }
}

void HordeManager::decreaseCount() {
    countToCheck--;
}

bool HordeManager::isWaveActive() {
    return countToCheck > 0;
}

bool HordeManager::isVictory() {
    return victory;
}
