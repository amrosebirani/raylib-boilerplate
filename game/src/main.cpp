#include "ParticleSystem.h"
#include "add_warriors.hpp"
#include "raylib.h"
#include "level_config.h"
#include "camera.h"
#include "constants.h"
#include "warrior_types.h"
#include "enemy_types.h"
#include "container.h"
#include "utils.h"
#include "globals.h"
#include "signal.h"
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
std::shared_ptr<Container> container;
std::shared_ptr<SpriteHolder> spriteHolder;

std::shared_ptr<Container> getContainer() {
    return container;
}

std::shared_ptr<SpriteHolder> getSpriteHolder() {
    return spriteHolder;
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

void initSprites() {

    spriteHolder->initSprites(
        {new SpriteConfig(BOW_SPRITE_ID, getAssetPath("textures/bow.png"), 128,
                          128),
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
                          getAssetPath("textures/archer_all_ROE_Idle2.png"),
                          160, 160),
         new SpriteConfig(FOOTMAN_IDLE_SPRITE_ID,
                          getAssetPath("textures/footman/footman_idle.png"),
                          160, 160),
         new SpriteConfig(FOOTMAN_ATTACKS_SPRITE_ID,
                          getAssetPath("textures/footman/footman_attack.png"),
                          160, 160),
         new SpriteConfig(FOOTMAN_BUST_SPRITE_ID,
                          getAssetPath("textures/footman/footman_bust.png"), 0,
                          0),
         new SpriteConfig(FOOTMAN_RUN_SPRITE_ID,
                          getAssetPath("textures/footman/footman_run.png"), 160,
                          160),
         new SpriteConfig(
             FOOTMAN_PAPERDOLL_SPRITE_ID,
             getAssetPath("textures/footman/footman_paperdoll.png"), 0, 0),
         new SpriteConfig(FOOTMAN_DEAD_SPRITE_ID,
                          getAssetPath("textures/footman/footman_dead.png"),
                          160, 160),
         new SpriteConfig(SPEARMAN_IDLE_SPRITE_ID,
                          getAssetPath("textures/spearman/spearman_idle.png"),
                          128, 128),
         new SpriteConfig(SPEARMAN_ATTACKS_SPRITE_ID,
                          getAssetPath("textures/spearman/spearman_attack.png"),
                          128, 128),
         new SpriteConfig(SPEARMAN_BUST_SPRITE_ID,
                          getAssetPath("textures/spearman/spearman_bust.png"),
                          0, 0),
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
                          getAssetPath("textures/spearman/spearman_ko.png"),
                          128, 128),
         new SpriteConfig(KNIGHT_IDLE_SPRITE_ID,
                          getAssetPath("textures/knight/knight_idle.png"), 160,
                          160),
         new SpriteConfig(KNIGHT_ATTACKS_SPRITE_ID,
                          getAssetPath("textures/knight/knight_attack.png"),
                          160, 160),
         new SpriteConfig(KNIGHT_BUST_SPRITE_ID,
                          getAssetPath("textures/knight/knight_bust.png"), 0,
                          0),
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
                          getAssetPath("textures/mage/mage_idle.png"), 160,
                          160),
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
                          getAssetPath("textures/mage/mage_dead.png"), 160,
                          160),
         new SpriteConfig(ENEMY1_BUST_SPRITE_ID,
                          getAssetPath("textures/enemy1/enemy1_bust.png"), 0,
                          0),
         new SpriteConfig(ENEMY1_ATTACKS_SPRITE_ID,
                          getAssetPath("textures/enemy1/enemy1_attack.png"),
                          128, 128),
         new SpriteConfig(ENEMY1_RUN_SPRITE_ID,
                          getAssetPath("textures/enemy1/enemy1_run.png"), 128,
                          128),
         new SpriteConfig(ENEMY1_DEAD_SPRITE_ID,
                          getAssetPath("textures/enemy1/enemy1_dead.png"), 128,
                          128),
         new SpriteConfig(ENEMY1_PAPERDOLL_SPRITE_ID,
                          getAssetPath("textures/enemy1/enemy1_paperdoll.png"),
                          0, 0),
         new SpriteConfig(ARROW_SPRITE_ID, getAssetPath("textures/arrow.png"),
                          0, 0),
         new SpriteConfig(CASTLE_SPRITE_ID,
                          getAssetPath("textures/castlekeep_14.png"), 0, 0),
         new SpriteConfig(TOWER_SPRITE_ID, getAssetPath("textures/tower.png"),
                          0, 0)});
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
    initWarriorSizes();
    initEnemySizes();
    SetWindowSize(GetScreenWidth(), GetScreenHeight());
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);
    SearchAndSetResourceDir("resources");
    spriteHolder = std::make_shared<SpriteHolder>();
    initSprites();

    // initialize formation here
    // then call update and draw on formation in the game loop
    float sh = GetScreenHeight();
    float sw = GetScreenWidth();
    float scaleX = (float)sw / VIRTUAL_WIDTH;
    float scaleY = (float)sh / VIRTUAL_HEIGHT;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    scale *= 1.5;
    viewCam = std::make_shared<CameraEnhanced>(VIRTUAL_WIDTH / 2.0f,
                                               VIRTUAL_HEIGHT / 2.0f, sw, sh,
                                               scale, 0.0f, nullptr);

    container = std::make_shared<Container>();
    container->init();
    Texture2D texturePP = LoadTexture(getAssetPath("textures/soft_sphere.png"));
    bloodSplatter = getParticleSystem(&texturePP, bloodColors);
    enemyBloodSplatter = getParticleSystem(&texturePP, enemyBloodColors);
    DrawRectangle(0, 0, 128, 128, WHITE);
    viewCam->follow(container->getFollowObject().get());
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
        container->update(dt);
        bloodSplatter->update(dt);
        enemyBloodSplatter->update(dt);
        BeginDrawing();

        ClearBackground(GRAY);
        viewCam->attach(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
        container->draw();
        bloodSplatter->draw();
        enemyBloodSplatter->draw();
        viewCam->detach();
        DrawFPS(10, 10);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
