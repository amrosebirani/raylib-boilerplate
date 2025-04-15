#include "game.h"
#include "platform.hpp"
#include "audio_manager.hpp"
#include "gameover.hpp"
#include "globals.h"
#include "horde_config.hpp"
#include "joystick.hpp"
#include "loading.h"
#include "raylib.h"
#include "timer.h"
#include <memory>
#include <unordered_map>
#include "upgrade_info.hpp"
#include "utils.h"
#include "level_config.h"
#include "victory.hpp"
#include "firebase.hpp"
#include "session.hpp"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include <iostream>

std::shared_ptr<std::unordered_map<std::string, float>> slowParams =
    std::make_shared<std::unordered_map<std::string, float>>();
Timer timer;
std::vector<Font> LoadedFonts;
std::string platform = "Desktop";
RenderTexture2D renderTexture;
std::string session_id = Timer::generateUUID();

std::string getSessionId() {
    return session_id;
}

void slow(float amount, float duration) {
    (*slowParams)["slow"] = amount;
    timer.tween(duration, slowParams, {{"slow", 1}}, "in-out-cubic", []() {},
                "slow_time", {});
}

void shake(float intensity, float duration, float frequency) {
    getViewCamera()->shake(intensity, duration, frequency);
}

void flash(float duration, Color color) {
    getViewCamera()->flash(duration, color);
}

const Font &GetFont(int id) {
    return LoadedFonts[id];
}

bool isPlatformDesktop() {
    return platform == "Desktop";
}

bool isPlatformWeb() {

    return platform == "HTML5";
}

const char *getAssetPath(const char *path) {
    if (isPlatformDesktop() || isPlatformWeb()) {
        return path;
    } else {
        return TextFormat("resources/%s", path);
    }
}

bool isPlatformAndroid() {
    return platform == "Android";
}

void setPlatform(std::string p) {
    platform = p;
}

bool paused = false;
bool shakeDetected = false;

bool isPaused() {
    return paused;
}

bool isShakeDetected() {
    return shakeDetected;
}

void resetShakeDetected() {
    shakeDetected = false;
}

std::shared_ptr<CameraEnhanced> viewCam;
std::shared_ptr<WorldState> worldState;
std::shared_ptr<StateStack> mStateStack;
std::shared_ptr<Container> container;
std::shared_ptr<AudioManager> audioManager;
std::shared_ptr<SpriteHolder> spriteHolder;
std::shared_ptr<Joystick> joystick;
Rectangle joystickRect;
std::shared_ptr<Loading> loading;
std::shared_ptr<GameOver> gameOver;
std::shared_ptr<MainMenu> mainMenu;
std::shared_ptr<Victory> victory;
std::shared_ptr<ScoreBoard> scoreBoard = nullptr;

std::shared_ptr<Container> getContainer() {
    return container;
}

std::shared_ptr<AudioManager> getAudioManager() {
    return audioManager;
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

std::shared_ptr<Joystick> getJoystick() {
    return joystick;
}

std::shared_ptr<GameOver> getGameOver() {
    return gameOver;
}

std::shared_ptr<ScoreBoard> getScoreBoard() {
    return scoreBoard;
}

void setScoreBoard(std::shared_ptr<ScoreBoard> sb) {
    scoreBoard = sb;
}

std::shared_ptr<MainMenu> getMainMenu() {
    return mainMenu;
}

std::shared_ptr<Victory> getVictory() {
    return victory;
}

void reinitializeGame() {
    container = nullptr;
    worldState = nullptr;
    container = std::make_shared<Container>();
    worldState = std::make_shared<WorldState>();
    container->init();
    mStateStack->push(container);
    mStateStack->push(worldState);
    getAudioManager()->switchBGM("normal");
}

void resetGame() {

    container = nullptr;
    worldState = nullptr;
    container = std::make_shared<Container>();
    worldState = std::make_shared<WorldState>();
}

void startGame() {

    getContainer()->init();
    getStateStack()->push(getContainer());
    getStateStack()->push(getWorldState());
    // getAudioManager()->switchBGM("normal");
}

Rectangle getJoystickRect() {
    return joystickRect;
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

#if defined(PLATFORM_ANDROID)
#include <jni.h>

extern "C" {
void Java_com_example_rootsofevil_MainActivity_onPauseNative(JNIEnv *env,
                                                             jobject obj);
void Java_com_example_rootsofevil_MainActivity_onResumeNative(JNIEnv *env,
                                                              jobject obj);
void Java_com_example_rootsofevil_MainActivity_passAccelerometerData(
    JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z);
void Java_com_example_rootsofevil_MainActivity_OnDestroyNative(JNIEnv *env,
                                                               jobject obj);
void Java_com_example_roosofevil_MainActivity_InitNative(JNIEnv *env,
                                                         jobject obj,
                                                         jobject activity);

void Java_com_example_rootsofevil_MainActivity_InitNative(JNIEnv *env,
                                                          jobject thiz,
                                                          jobject activity);
}

// Implement the onPauseNative method
void Java_com_example_rootsofevil_MainActivity_onPauseNative(JNIEnv *env,
                                                             jobject obj) {
    paused = true;
}

// Implement the onResumeNative method
void Java_com_example_rootsofevil_MainActivity_onResumeNative(JNIEnv *env,
                                                              jobject obj) {
    paused = false;
}

void Java_com_example_rootsofevil_MainActivity_InitNative(JNIEnv *env,
                                                          jobject obj,
                                                          jobject activity) {
    setActivity(env->NewGlobalRef(activity));
}

void Java_com_example_rootsofevil_MainActivity_OnDestroyNative(JNIEnv *env,
                                                               jobject obj) {
    clearActivity(env);
}

void Java_com_example_rootsofevil_MainActivity_passAccelerometerData(
    JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z) {
    std::cout << "Accelerometer Data: X = " << x << ", Y = " << y
              << ", Z = " << z << std::endl;

    // You can use this data in your game logic
    // Example: Detect a shake gesture
    const float shakeThreshold = 12.5f;
    if (fabs(x) > shakeThreshold || fabs(y) > shakeThreshold ||
        fabs(z) > shakeThreshold) {
        std::cout << "Shake detected!" << std::endl;
        shakeDetected = true;
    }
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    // Save the JavaVM pointer globally
    setJavaVM(vm);
    return JNI_VERSION_1_6; // Specify JNI version
}
#endif

void SetPlatform() {
#if defined(PLATFORM_DESKTOP)
    setPlatform("Desktop");
#elif defined(PLATFORM_ANDROID)
    setPlatform("Android");
    SetGesturesEnabled(GESTURE_DRAG | GESTURE_PINCH_IN | GESTURE_PINCH_OUT |
                       GESTURE_TAP | GESTURE_HOLD | GESTURE_SWIPE_RIGHT |
                       GESTURE_SWIPE_LEFT);
#elif defined(PLATFORM_RPI)
    setPlatform("Raspberry Pi");
#elif defined(PLATFORM_WEB)
    setPlatform("HTML5");
#else
    setPlatform("Unknown");
#endif
}

bool spaceActivated = true;
const float spaceRT = 0.5f;
float spaceAT = 0.0f;
bool keyReleased = true;
int screenHeight = 440;
int screenWidth = 960;

void basicDraw() {

    ClearBackground(GRAY);
    mStateStack->draw();
    if (paused) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                      Fade(BLACK, 0.5f));
        DrawText("Paused", GetScreenWidth() / 2 - 50,
                 GetScreenHeight() / 2 - 10, 20, WHITE);
    }
}

void updateDrawFrame() {

    if (isPlatformWeb()) {

        float sh = GetScreenHeight();
        float sw = GetScreenWidth();
        if (sh != screenHeight || sw != screenWidth) {
            screenWidth = sw;
            screenHeight = sh;
            // SetWindowSize(screenWidth, screenHeight);
            CloseWindow();
            InitWindow(sw, sh, "Roots Of Evil");
            // std::cout << "Resized" << std::endl;
            // UnloadRenderTexture(renderTexture);
            renderTexture =
                LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        }
    }
    if (IsKeyUp(KEY_P)) {
        keyReleased = true;
    }
    if (IsKeyDown(KEY_P) && spaceActivated && keyReleased) {
        std::cout << "pausing/unpausing" << std::endl;
        paused = !paused;
        spaceActivated = false;
        keyReleased = false;
        spaceAT = 0.0f;
    }
    if (!spaceActivated) {
        // std::cout << "!space activated" << std::endl;
        spaceAT += GetFrameTime();
        if (spaceAT >= spaceRT) {
            spaceActivated = true;
        }
    }
    // std::cout << "paused: " << paused << ", spaceActivated: " <<
    // spaceActivated
    //           << ", spaceAT: " << spaceAT << ", keyReleased" << keyReleased
    //           << std::endl;
    float dt = GetFrameTime();
    if (dt > 3.0f) {
        dt = 1 / 60.0f;
    }
    if (!paused) {
        viewCam->update(dt);

        mStateStack->update(dt);
    }
    getAudioManager()->update(dt);
    if (isPlatformDesktop() || isPlatformWeb()) {

        BeginTextureMode(renderTexture);
        basicDraw();
        EndTextureMode();
    }

    BeginDrawing();

    // container->draw();
    // DrawCircleV(viewCam->getMousePosition(), 5, RED);
    // DrawFPS(10, 10);
    if (isPlatformWeb() || isPlatformDesktop()) {
        // ClearBackground(GRAY);
        DrawTextureRec(renderTexture.texture,
                       (Rectangle){0, 0, renderTexture.texture.width * 1.0f,
                                   -renderTexture.texture.height * 1.0f},
                       (Vector2){0, 0}, WHITE); // worldState->draw();
    } else {
        basicDraw();
    }
    // DrawFPS(screenWidth / 2, screenHeight / 2);
    EndDrawing();
}

void startGameLoop() {
    SetPlatform();
    timer = Timer();
    if (isPlatformWeb()) {
        // const int screenWidth = 960;
        // const int screenHeight = 440;

        SetConfigFlags(FLAG_WINDOW_HIGHDPI);
        SetConfigFlags(FLAG_VSYNC_HINT);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(screenWidth, screenHeight, "Roots Of Evil");
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        renderTexture = LoadRenderTexture(screenWidth, screenHeight);
    } else {
        SetConfigFlags(FLAG_VSYNC_HINT);
        // SetConfigFlags(FLAG_WINDOW_UNDECORATED);
        // SetConfigFlags(FLAG_FULLSCREEN_MODE);
        InitWindow(0, 0, "Roots Of Evil");
        // SetWindowMonitor(monitorIndex);
        SetWindowSize(GetScreenWidth(), GetScreenHeight());
        SetWindowPosition(0, 0);
        // SetWindowState(FLAG_WINDOW_MAXIMIZED);
        // SetWindowState(FLAG_WINDOW_RESIZABLE);
        if (isPlatformDesktop()) {
            SetWindowPosition(0, 0);
        }
        renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }
    SetTargetFPS(60);
    sendFirebaseEvent("GameLaunched", {});
    if (isPlatformDesktop() || isPlatformWeb())
        SearchAndSetResourceDir("resources");

    initLevels();
    initLevelUpgradeData();
    initWarriorSizes();
    initEnemySizes();
    initPropertyTributeRates();
    initHordeConfigs();
    spriteHolder = std::make_shared<SpriteHolder>();
    setBuildingData();
    setUpgradeInfo();
    float sh = GetScreenHeight();
    float sw = GetScreenWidth();
    float scaleX = (float)sw / VIRTUAL_WIDTH;
    float scaleY = (float)sh / VIRTUAL_HEIGHT;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    scale *= 1.5;
    viewCam = std::make_shared<CameraEnhanced>(VIRTUAL_WIDTH / 2.0f,
                                               VIRTUAL_HEIGHT / 2.0f, sw, sh,
                                               scale, 0.0f, nullptr);
    if (isPlatformAndroid()) {
        if (GetScreenWidth() > GetScreenHeight()) {
            joystick = std::make_shared<Joystick>(Vector2{
                5.0f / 6 * GetScreenWidth(), 6.0f / 8 * GetScreenHeight()});
            joystickRect = {
                4.0f / 6 * GetScreenWidth(), 1.0f / 2 * GetScreenHeight(),
                2.0f / 6 * GetScreenWidth(), 1.0f / 2 * GetScreenHeight()};
        } else {
            // portrait
            joystick = std::make_shared<Joystick>(Vector2{
                1.0f / 2 * GetScreenWidth(), 5.0f / 6 * GetScreenHeight()});
            joystickRect = {0, 4.0f / 6 * GetScreenHeight(),
                            GetScreenWidth() * 1.0f,
                            1.0f / 3 * GetScreenHeight()};
        }
    }
    container = std::make_shared<Container>();
    // container->init();
    audioManager = std::make_shared<AudioManager>();
    mStateStack = std::make_shared<StateStack>();
    loading = std::make_shared<Loading>();
    gameOver = std::make_shared<GameOver>();
    mainMenu = std::make_shared<MainMenu>();
    victory = std::make_shared<Victory>();
    mStateStack->push(loading);
    // overtime we will check for world state in saved game implementation
    worldState = std::make_shared<WorldState>();
    Texture2D texturePP = LoadTexture(getAssetPath("textures/soft_sphere.png"));
    bloodSplatter = getParticleSystem(&texturePP, bloodColors);
    enemyBloodSplatter = getParticleSystem(&texturePP, enemyBloodColors);
    DrawRectangle(0, 0, 128, 128, WHITE);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(updateDrawFrame, 0, 1);
#else

    while (!WindowShouldClose()) {
        updateDrawFrame();
    }
#endif
    CloseWindow();
    return;
}
