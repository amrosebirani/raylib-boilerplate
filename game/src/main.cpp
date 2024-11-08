#include "ParticleSystem.h"
#include "add_warriors.hpp"
#include "horde_config.hpp"
#include "property_type.hpp"
#include "raylib.h"
#include "level_config.h"
#include "camera.h"
#include "constants.h"
#include "sprite_holder.hpp"
#include "state_stack.hpp"
#include "warrior_types.h"
#include "enemy_types.h"
#include "container.h"
#include "utils.h"
#include "globals.h"
#include "signal.h"
#include "world_state.hpp"
#if defined(PLATFORM_DESKTOP)
#include <execinfo.h>
#endif
#include "unistd.h"
#include <iostream>

std::string platform = "Desktop";
bool paused = false;
bool spacePressed = false;

bool isPaused() {
    return paused;
}

bool isPlatformAndroid() {
    return platform == "Android";
}

bool isPlatformDesktop() {
    return platform == "Desktop";
}

bool isPlatformWeb() {
    return platform == "Web";
}

void setPlatform(std::string p) {
    platform = p;
}

const char *getAssetPath(const char *path) {
    if (isPlatformDesktop() || isPlatformWeb()) {
        return path;
    } else {
        return TextFormat("resources/%s", path);
    }
}

void handler(int sig) {
#if defined(PLATFORM_DESKTOP)
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
#endif
    exit(1);
}

std::shared_ptr<CameraEnhanced> viewCam;
std::shared_ptr<WorldState> worldState;
std::shared_ptr<StateStack> mStateStack;
std::shared_ptr<Container> container;
std::shared_ptr<SpriteHolder> spriteHolder;

std::shared_ptr<Container> getContainer() {
    return container;
}

std::shared_ptr<StateStack> getStateStack() {
    return mStateStack;
}

std::shared_ptr<CameraEnhanced> getViewCamera() {
    return viewCam;
}

std::shared_ptr<SpriteHolder> getSpriteHolder() {
    return spriteHolder;
}

std::shared_ptr<WorldState> getWorldState() {
    return worldState;
}

std::shared_ptr<ParticleSystem> bloodSplatter;
std::shared_ptr<ParticleSystem> enemyBloodSplatter;
std::shared_ptr<ParticleSystem> getBloodSplatter() {
    return bloodSplatter;
}

std::shared_ptr<ParticleSystem> getEnemyBloodSplatter() {
    return enemyBloodSplatter;
}

bool SearchAndSetResourceDir(const char *folderName) {
    if (DirectoryExists(folderName)) {
        ChangeDirectory(TextFormat("%s/%s", GetWorkingDirectory(), folderName));
        return true;
    }

    const char *appDir = GetApplicationDirectory();

    const char *dir = TextFormat("%s%s", appDir, folderName);
    if (DirectoryExists(dir)) {
        ChangeDirectory(dir);
        return true;
    }

    dir = TextFormat("%s../%s", appDir, folderName);
    if (DirectoryExists(dir)) {
        ChangeDirectory(dir);
        return true;
    }

    dir = TextFormat("%s../../%s", appDir, folderName);
    if (DirectoryExists(dir)) {
        ChangeDirectory(dir);
        return true;
    }

    dir = TextFormat("%s../../../%s", appDir, folderName);
    if (DirectoryExists(dir)) {
        ChangeDirectory(dir);
        return true;
    }

    return false;
}

int getSpriteGridMetric(std::string enemyId) {
    if (enemyId == "rat" || enemyId == "zombie_giant" ||
        enemyId == "skeleton_prince") {
        return 256;
    }
    if (enemyId == "wolf") {
        return 96;
    }
    return 128;
}

void initEnemySprites() {
    std::vector<std::string> enemyIds2 = {
        "ghoul",        "necromancer", "skeleton", "skeleton_prince", "zombie1",
        "zombie2",      "zombie3",     "zombie4",  "zombie5",         "zombie6",
        "zombie_giant", "buck",        "rat",      "spider",          "wolf"};
    for (auto id : enemyIds2) {
        std::vector<SpriteConfig *> spriteConfigs;
        std::vector<std::string> spriteIds = {"attack", "move", "idle", "dead"};
        int metric = getSpriteGridMetric(id);
        for (auto spriteId : spriteIds) {
            std::string path = "textures/enemy/" + id + "/" + spriteId + ".png";
            spriteConfigs.push_back(new SpriteConfig(id + "_" + spriteId,
                                                     getAssetPath(path.c_str()),
                                                     metric, metric));
        }
        spriteConfigs.push_back(new SpriteConfig(
            id + "_bust",
            getAssetPath(("textures/enemy/" + id + "/bust.png").c_str()), 0,
            0));
        spriteHolder->initSprites(spriteConfigs);
    }
    spriteHolder->initSprites({
        new SpriteConfig(ENEMY1_BUST_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_bust.png"), 0, 0),
        new SpriteConfig(ENEMY1_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_attack.png"), 128,
                         128),
        new SpriteConfig(ENEMY1_RUN_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_run.png"), 128,
                         128),
        new SpriteConfig(ENEMY1_DEAD_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_dead.png"), 128,
                         128),
        new SpriteConfig(ENEMY1_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/enemy1/enemy1_paperdoll.png"),
                         0, 0),
    });
}

void initTreeSprites() {
    spriteHolder->initSprites({
        new SpriteConfig(TREE1_SPRITE_ID, getAssetPath("textures/tree_02.png"),
                         0, 0),
        new SpriteConfig(TREE2_SPRITE_ID, getAssetPath("textures/tree_03.png"),
                         0, 0),
        new SpriteConfig(TREE3_SPRITE_ID, getAssetPath("textures/tree_04.png"),
                         0, 0),
        new SpriteConfig(TREE4_SPRITE_ID, getAssetPath("textures/tree_05.png"),
                         0, 0),
        new SpriteConfig(TREE5_SPRITE_ID, getAssetPath("textures/tree_06.png"),
                         0, 0),
        new SpriteConfig(TREE6_SPRITE_ID, getAssetPath("textures/tree_08.png"),
                         0, 0),
        new SpriteConfig(TREE7_SPRITE_ID, getAssetPath("textures/tree_09.png"),
                         0, 0),
    });
}

void initWarriorSprites() {
    spriteHolder->initSprites({

        new SpriteConfig(ARCHER_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/archer_all_roe_attack.png"),
                         160, 160),
        new SpriteConfig(ARCHER_BUST_SPRITE_ID,
                         getAssetPath("textures/archer_all_roe_bust.png"), 0,
                         0),
        new SpriteConfig(ARCHER_IDLE_BOW_SPRITE_ID,
                         getAssetPath("textures/archer_all_roe_idle_bow.png"),
                         160, 160),
        new SpriteConfig(ARCHER_IDLE_SPRITE_ID,
                         getAssetPath("textures/archer_all_ROE_Idle2.png"), 160,
                         160),
        new SpriteConfig(FOOTMAN_IDLE_SPRITE_ID,
                         getAssetPath("textures/footman/footman_idle.png"), 160,
                         160),
        new SpriteConfig(FOOTMAN_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/footman/footman_attack.png"),
                         160, 160),
        new SpriteConfig(FOOTMAN_BUST_SPRITE_ID,
                         getAssetPath("textures/footman/footman_bust.png"), 0,
                         0),
        new SpriteConfig(FOOTMAN_RUN_SPRITE_ID,
                         getAssetPath("textures/footman/footman_run.png"), 160,
                         160),
        new SpriteConfig(FOOTMAN_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/footman/footman_paperdoll.png"),
                         0, 0),
        new SpriteConfig(FOOTMAN_DEAD_SPRITE_ID,
                         getAssetPath("textures/footman/footman_dead.png"), 160,
                         160),
        new SpriteConfig(SPEARMAN_IDLE_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_idle.png"),
                         128, 128),
        new SpriteConfig(SPEARMAN_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_attack.png"),
                         128, 128),
        new SpriteConfig(SPEARMAN_BUST_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_bust.png"), 0,
                         0),
        new SpriteConfig(SPEARMAN_RUN_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_run.png"),
                         128, 128),
        new SpriteConfig(
            SPEARMAN_PAPERDOLL_SPRITE_ID,
            getAssetPath("textures/spearman/spearman_paperdoll.png"), 0, 0),
        new SpriteConfig(SPEARMAN_DEAD_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_dead.png"),
                         128, 128),
        new SpriteConfig(SPEARMAN_KO_SPRITE_ID,
                         getAssetPath("textures/spearman/spearman_ko.png"), 128,
                         128),
        new SpriteConfig(KNIGHT_IDLE_SPRITE_ID,
                         getAssetPath("textures/knight/knight_idle.png"), 160,
                         160),
        new SpriteConfig(KNIGHT_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/knight/knight_attack.png"), 160,
                         160),
        new SpriteConfig(KNIGHT_BUST_SPRITE_ID,
                         getAssetPath("textures/knight/knight_bust.png"), 0, 0),
        new SpriteConfig(KNIGHT_RUN_SPRITE_ID,
                         getAssetPath("textures/knight/knight_run.png"), 160,
                         160),
        new SpriteConfig(KNIGHT_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/knight/knight_paperdoll.png"),
                         0, 0),
        new SpriteConfig(KNIGHT_DEAD_SPRITE_ID,
                         getAssetPath("textures/knight/knight_dead.png"), 160,
                         160),
        new SpriteConfig(MAGE_IDLE_SPRITE_ID,
                         getAssetPath("textures/mage/mage_idle.png"), 160, 160),
        new SpriteConfig(MAGE_ATTACKS_SPRITE_ID,
                         getAssetPath("textures/mage/mage_attack.png"), 160,
                         160),
        new SpriteConfig(MAGE_BUST_SPRITE_ID,
                         getAssetPath("textures/mage/mage_bust.png"), 0, 0),
        new SpriteConfig(MAGE_RUN_SPRITE_ID,
                         getAssetPath("textures/mage/mage_run.png"), 160, 160),
        new SpriteConfig(MAGE_PAPERDOLL_SPRITE_ID,
                         getAssetPath("textures/mage/mage_paperdoll.png"), 0,
                         0),
        new SpriteConfig(MAGE_DEAD_SPRITE_ID,
                         getAssetPath("textures/mage/mage_dead.png"), 160, 160),
    });
}

void initBuildingSprites() {
    spriteHolder->initSprites({
        new SpriteConfig(
            ARCHERY1_SPRITE_ID,
            getAssetPath("textures/buildings/archery/archery1.png"), 0, 0),

        new SpriteConfig(
            ARCHERY2_SPRITE_ID,
            getAssetPath("textures/buildings/archery/archery2.png"), 0, 0),
        new SpriteConfig(
            ARCHERY3_SPRITE_ID,
            getAssetPath("textures/buildings/archery/archery3.png"), 0, 0),
        new SpriteConfig(
            BARRACKS1_SPRITE_ID,
            getAssetPath("textures/buildings/barracks/barrack1.png"), 0, 0),
        new SpriteConfig(
            BARRACKS2_SPRITE_ID,
            getAssetPath("textures/buildings/barracks/barrack2.png"), 0, 0),
        new SpriteConfig(
            BARRACKS3_SPRITE_ID,
            getAssetPath("textures/buildings/barracks/barrack3.png"), 0, 0),
        new SpriteConfig(
            BARRACKS4_SPRITE_ID,
            getAssetPath("textures/buildings/barracks/barrack4.png"), 0, 0),
        new SpriteConfig(CASTLE2_SPRITE_ID,
                         getAssetPath("textures/buildings/castle/castle2.png"),
                         0, 0),
        new SpriteConfig(CASTLE1_SPRITE_ID,
                         getAssetPath("textures/buildings/castle/castle1.png"),
                         0, 0),
        new SpriteConfig(CASTLE3_SPRITE_ID,
                         getAssetPath("textures/buildings/castle/castle3.png"),
                         0, 0),
        new SpriteConfig(CASTLE4_SPRITE_ID,
                         getAssetPath("textures/buildings/castle/castle4.png"),
                         0, 0),
        new SpriteConfig(HOUSE1_SPRITE_ID,
                         getAssetPath("textures/buildings/houses/house1.png"),
                         0, 0),
        new SpriteConfig(HOUSE2_SPRITE_ID,
                         getAssetPath("textures/buildings/houses/house2.png"),
                         0, 0),
        new SpriteConfig(HOUSE3_SPRITE_ID,
                         getAssetPath("textures/buildings/houses/house3.png"),
                         0, 0),
        new SpriteConfig(HOUSE4_SPRITE_ID,
                         getAssetPath("textures/buildings/houses/house4.png"),
                         0, 0),
        new SpriteConfig(
            WIZARDRY1_SPRITE_ID,
            getAssetPath("textures/buildings/wizardry/wizardry1.png"), 0, 0),
        new SpriteConfig(
            WIZARDRY2_SPRITE_ID,
            getAssetPath("textures/buildings/wizardry/wizardry2.png"), 0, 0),
        new SpriteConfig(
            WIZARDRY3_SPRITE_ID,
            getAssetPath("textures/buildings/wizardry/wizardry3.png"), 0, 0),
        new SpriteConfig(
            LIGHTNING_TOWER_SPRITE_ID,
            getAssetPath("textures/buildings/towers/lightning_tower.png"), 0,
            0),
        new SpriteConfig(
            BLOCK_TOWER1_SPRITE_ID,
            getAssetPath("textures/buildings/defence_blocks/block_04a.png"), 0,
            0),
        new SpriteConfig(
            BLOCK_TOWER2_SPRITE_ID,
            getAssetPath("textures/buildings/defence_blocks/block_06a.png"), 0,
            0),

    });
}

void initSprites() {

    spriteHolder->initSprites({
        new SpriteConfig(BOW_SPRITE_ID, getAssetPath("textures/bow.png"), 128,
                         128),
        new SpriteConfig(ARROW_SPRITE_ID, getAssetPath("textures/arrow.png"), 0,
                         0),
        new SpriteConfig(CASTLE_SPRITE_ID,
                         getAssetPath("textures/castlekeep_14.png"), 0, 0),
        new SpriteConfig(TERRAIN_OP_SPRITE_ID,
                         getAssetPath("textures/ground_02_edit.png"), 0, 0),
        new SpriteConfig(TOWER_SPRITE_ID, getAssetPath("textures/tower.png"), 0,
                         0),
        new SpriteConfig(COINS_AND_GEMS,
                         getAssetPath("textures/combined_gems.png"), 16, 16),
        new SpriteConfig(INFANTRY_ICON,
                         getAssetPath("textures/icons/infantry.png"), 0, 0),
        new SpriteConfig(ARCHERY_ICON,
                         getAssetPath("textures/icons/archers.png"), 0, 0),
        new SpriteConfig(WIZARDRY_ICON,
                         getAssetPath("textures/icons/magic.png"), 0, 0),
        new SpriteConfig(UI_ICONS, getAssetPath("textures/ui-icons.png"), 48,
                         48),
    });
    initTreeSprites();
    initWarriorSprites();
    initBuildingSprites();
}

void setPlatform() {
#if defined(PLATFORM_DESKTOP)
    setPlatform("Desktop");
#elif defined(PLATFORM_ANDROID)
    setPlatform("Android");
    SetGesturesEnabled(GESTURE_DRAG | GESTURE_PINCH_IN | GESTURE_PINCH_OUT);
#elif defined(PLATFORM_RPI)
    setPlatform("Raspberry Pi");
#elif defined(PLATFORM_WEB)
    setPlatform("HTML5");
#else
    setPlatform("Unknown");
#endif
}

int main() {
    setPlatform();
    signal(SIGSEGV, handler);
    InitWindow(0, 0, "Roots of Evil!!");
    initLevels();
    initLevelUpgradeData();
    initWarriorSizes();
    initEnemySizes();
    initPropertyTributeRates();
    initHordeConfigs();
    SetWindowSize(GetScreenWidth(), GetScreenHeight());
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);
    SearchAndSetResourceDir("resources");
    spriteHolder = std::make_shared<SpriteHolder>();
    initSprites();
    setBuildingData();
    initEnemySprites();

    // initialize formation here
    // then call update and draw on formation in the game loop
    float sh = GetScreenHeight();
    float sw = GetScreenWidth();
    float scaleX = (float)sw / VIRTUAL_WIDTH;
    float scaleY = (float)sh / VIRTUAL_HEIGHT;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    scale *= .9;
    viewCam = std::make_shared<CameraEnhanced>(VIRTUAL_WIDTH / 2.0f,
                                               VIRTUAL_HEIGHT / 2.0f, sw, sh,
                                               scale, 0.0f, nullptr);

    container = std::make_shared<Container>();
    container->init();
    mStateStack = std::make_shared<StateStack>();
    mStateStack->push(container);
    // overtime we will check for world state in saved game implementation
    worldState = std::make_shared<WorldState>();
    mStateStack->push(worldState);
    Texture2D texturePP = LoadTexture(getAssetPath("textures/soft_sphere.png"));
    bloodSplatter = getParticleSystem(&texturePP, bloodColors);
    enemyBloodSplatter = getParticleSystem(&texturePP, enemyBloodColors);
    DrawRectangle(0, 0, 128, 128, WHITE);
    viewCam->follow(container->getFollowObject());
    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_SPACE)) {
            if (!spacePressed) {
                paused = !paused;
                spacePressed = true;
            }
        } else {
            spacePressed = false;
        }
        if (paused) {
            continue;
        }
        float dt = GetFrameTime();
        if (dt > 3.0f) {
            dt = 1 / 60.0f;
        }
        viewCam->update(dt);
        mStateStack->update(dt);
        bloodSplatter->update(dt);
        enemyBloodSplatter->update(dt);
        BeginDrawing();

        ClearBackground(GRAY);
        mStateStack->draw();
        // container->draw();
        viewCam->attach(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
        bloodSplatter->draw();
        enemyBloodSplatter->draw();
        // DrawCircleV(viewCam->getMousePosition(), 5, RED);
        viewCam->detach();
        // DrawFPS(10, 10);
        // worldState->draw();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
