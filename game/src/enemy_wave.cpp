#include "enemy_wave.hpp"
#include "globals.h"
#include "lightning_line.hpp"
#include "utils.h"
#include "parameterized_enemy.hpp"

EnemyWave::EnemyWave(float time, int count, std::shared_ptr<GameObject> castle)
    : time(time), count(count), castle(castle) {
    timer = Timer();
    enemy_chance_list =
        new TempChanceList<EnemyType>({{EnemyType::ENEMY_TYPE_SHIELD_BEARER, 4},
                                       {EnemyType::ZOMBIE1, 4},
                                       {EnemyType::ZOMBIE2, 4},
                                       {EnemyType::ZOMBIE3, 4},
                                       {EnemyType::ZOMBIE4, 4},
                                       {EnemyType::ZOMBIE5, 4},
                                       {EnemyType::ZOMBIE6, 4},
                                       {EnemyType::SKELETON, 4},
                                       {EnemyType::GHOUL, 4},
                                       {EnemyType::RAT, 4},
                                       {EnemyType::BUCK, 4},
                                       {EnemyType::SPIDER, 4},
                                       {EnemyType::NECROMANCER, 4},
                                       {EnemyType::SKELETON_PRINCE, 4},
                                       {EnemyType::ZOMBIE_GIANT, 4},
                                       {EnemyType::WOLF, 4}});
    int first_s = count / 5;
    int second_s = count / 5;
    int third_s = 3 * count / 5;
    // spawn the first wave right away
    for (int i = 0; i < first_s; i++) {
        spawnEnemy();
    }
    timer.after(
        time / 4,
        [this, time, second_s](float dt) {
            this->timer.every(
                time / 8,
                [this, second_s](float dt) {
                    for (int i = 0; i < second_s / 4; i++) {
                        this->spawnEnemy();
                    }
                },
                0, []() {}, "");
        },
        "");
    timer.after(
        3 * time / 4,
        [this, time, third_s](float dt) {
            this->timer.every(
                time / 8,
                [this, third_s](float dt) {
                    for (int i = 0; i < third_s / 2; i++) {
                        this->spawnEnemy();
                    }
                },
                0, []() {}, "");
        },
        "");
    timer.after(1.1 * time, [this](float dt) { this->isFinished = true; }, "");
}

void EnemyWave::update(float dt) {
    timer.update(dt);
}

EnemyWave::~EnemyWave() {
    delete enemy_chance_list;
}

void EnemyWave::createEnemy(float xd, float yd) {
    EnemyType et = enemy_chance_list->next();
    std::shared_ptr<ParameterizedEnemy> pe =
        std::make_shared<ParameterizedEnemy>(this->castle->x + xd,
                                             this->castle->y + yd, et);
    pe->init();
    getContainer()->addGameObject(pe);
}

void EnemyWave::spawnEnemy() {
    // return;

    float xd = randomFloatInRange(VIRTUAL_WIDTH / 3.0f, VIRTUAL_WIDTH / 2.0f);
    float yd = randomFloatInRange(VIRTUAL_HEIGHT / 3.0f, VIRTUAL_HEIGHT / 2.0f);
    int xm = getRandomValue(-1, 1);
    int ym = getRandomValue(-1, 1);
    xd *= xm;
    yd *= ym;
    // this is where we need to add a lightning and then spawn the
    // enemy_shield_bearer
    std::shared_ptr<LightningLine> ll = std::make_shared<LightningLine>(
        this->castle->x + xd, this->castle->y + yd - 60, this->castle->x + xd,
        this->castle->y + yd);
    getContainer()->addGameObject(ll);
    timer.after(
        0.20, [this, xd, yd](float dt) { this->createEnemy(xd, yd); }, "");
}

bool EnemyWave::isFinishedWave() {
    return isFinished;
}
