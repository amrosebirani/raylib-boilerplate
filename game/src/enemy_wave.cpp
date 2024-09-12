#include "enemy_wave.hpp"
#include "globals.h"
#include "utils.h"
#include "enemy_shield_bearer.hpp"

EnemyWave::EnemyWave(float time, int count, std::shared_ptr<GameObject> castle)
    : time(time), count(count), castle(castle) {
    timer = Timer();
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

void EnemyWave::spawnEnemy() {

    float xd = randomFloatInRange(VIRTUAL_WIDTH / 3.0f, VIRTUAL_WIDTH / 2.0f);
    float yd = randomFloatInRange(VIRTUAL_HEIGHT / 3.0f, VIRTUAL_HEIGHT / 2.0f);
    int xm = getRandomValue(-1, 1);
    int ym = getRandomValue(-1, 1);
    xd *= xm;
    yd *= ym;
    std::shared_ptr<EnemyShieldBearer> esb =
        std::make_shared<EnemyShieldBearer>(this->castle->x + xd,
                                            this->castle->y + yd);
    esb->init();
    getContainer()->addGameObject(esb);
}

bool EnemyWave::isFinishedWave() {
    return isFinished;
}
