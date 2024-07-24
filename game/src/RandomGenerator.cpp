#include "RandomGenerator.h"
#include "raylib.h"

#include <exception>
#include <limits>
#include <sstream>
#include <iomanip>

#include <cmath>
#include <cstdlib>

static uint64 wangHash64(uint64 key) {
    key = (~key) + (key << 21);
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8);
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;
}

RandomGenerator::RandomGenerator()
    : last_randomnormal(std::numeric_limits<double>::infinity()) {
    Seed newseed;
    newseed.b32.low = 0xCBBF7A44;
    newseed.b32.high = 0x0139408D;
    setSeed(newseed);
}

uint64 RandomGenerator::rand() {
    rng_state.b64 ^= (rng_state.b64 >> 12);
    rng_state.b64 ^= (rng_state.b64 << 25);
    rng_state.b64 ^= (rng_state.b64 >> 27);
    return rng_state.b64 * 2685821657736338717ULL;
}

void RandomGenerator::setSeed(RandomGenerator::Seed newseed) {
    seed = newseed;

    do {
        newseed.b64 = wangHash64(newseed.b64);
    } while (newseed.b64 == 0);

    rng_state = newseed;

    last_randomnormal = std::numeric_limits<double>::infinity();
}

double RandomGenerator::randomNormal(double stddev) {

    if (last_randomnormal != std::numeric_limits<double>::infinity()) {
        double r = last_randomnormal;
        last_randomnormal = std::numeric_limits<double>::infinity();
        return r * stddev;
    }

    double r = sqrt(-2.0 * log(1. - random()));
    double phi = 2.0 * M_PI * (1. - random());

    last_randomnormal = r * cos(phi);
    return r * sin(phi) * stddev;
}

RandomGenerator::Seed RandomGenerator::getSeed() const {
    return seed;
}

void RandomGenerator::setState(const std::string &statestr) {
    if (statestr.find("0x") != 0 || statestr.size() < 3) {
    }

    Seed state = {};

    char *end = nullptr;
    state.b64 = strtoull(statestr.c_str(), &end, 16);

    if (end != nullptr && *end != 0) {
    }

    rng_state = state;

    last_randomnormal = std::numeric_limits<double>::infinity();
}

std::string RandomGenerator::getState() const {
    std::stringstream ss;

    ss << "0x" << std::setfill('0') << std::setw(16) << std::hex
       << rng_state.b64;

    return ss.str();
}
