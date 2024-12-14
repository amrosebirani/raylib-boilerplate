#include "stat.hpp"
#include <random>

std::string generateTempUUID() {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    std::string res;
    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            res += "-";
        } else {
            res += v[dist(rng)];
        }
    }
    return res;
}

void Stat::update() {
    for (auto it = additive_map.begin(); it != additive_map.end(); it++) {
        additive += it->second;
    }

    for (auto it = flat_additive_map.begin(); it != flat_additive_map.end();
         it++) {
        flat_additive += it->second;
    }

    if (additive > 0) {
        value = base * (1 + additive);
    } else {
        value = base / (1 - additive);
    }

    valueCount = baseCount + flat_additive;

    additive = 0;
    flat_additive = 0;

    // additives.clear();
}

void Stat::increase(float percentage, std::string tag) {
    if (tag == "") {
        tag = generateTempUUID();
    }
    additive_map[tag] = percentage * 0.01;
    update();
}

void Stat::decrease(float percentage, std::string tag) {
    if (tag == "") {
        tag = generateTempUUID();
    }
    additive_map[tag] = -percentage * 0.01;
    update();
}

void Stat::increaseCount(int count, std::string tag) {
    if (tag == "") {
        tag = generateTempUUID();
    }
    flat_additive_map[tag] = count;
    update();
}

void Stat::decreaseCount(int count, std::string tag) {
    if (tag == "") {
        tag = generateTempUUID();
    }
    flat_additive_map[tag] = -count;
    update();
}

void Stat::clearStat(std::string tag) {
    // erase if exists

    additive_map.erase(tag);
    flat_additive_map.erase(tag);
    update();
}

void Stat::setBase(float base) {
    this->base = base;
    update();
}

void Stat::setBaseCount(int baseCount) {
    this->baseCount = baseCount;
    update();
}
