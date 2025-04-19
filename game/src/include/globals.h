#pragma once
#include "ParticleSystem.h"
#include "audio_manager.hpp"
#include "camera.h"
#include "container.h"
#include "gameover.hpp"
#include "joystick.hpp"
#include "mainmenu.hpp"
#include "scoreboard.hpp"
#include "sprite_holder.hpp"
#include "victory.hpp"
#include "world_state.hpp"
#include "state_stack.hpp"
#include <memory>

std::shared_ptr<Container> getContainer();
std::shared_ptr<AudioManager> getAudioManager();
std::shared_ptr<StateStack> getStateStack();
std::shared_ptr<SpriteHolder> getSpriteHolder();
std::shared_ptr<ParticleSystem> getBloodSplatter();
std::shared_ptr<ParticleSystem> getEnemyBloodSplatter();
std::shared_ptr<CameraEnhanced> getViewCamera();
std::shared_ptr<WorldState> getWorldState();
std::shared_ptr<Joystick> getJoystick();
std::shared_ptr<GameOver> getGameOver();
std::shared_ptr<MainMenu> getMainMenu();
std::shared_ptr<Victory> getVictory();
std::shared_ptr<ScoreBoard> getScoreBoard();
void setScoreBoard(std::shared_ptr<ScoreBoard> sb);
bool isShakeDetected();
void resetShakeDetected();
void reinitializeGame();
void resetGame();
void startGame();
Rectangle getJoystickRect();
const char *getAssetPath(const char *path);
bool isPaused();
void initEnemySprites();
