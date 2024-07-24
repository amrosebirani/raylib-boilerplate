#pragma once
#include <string>
#include "int.h"
#include "config.h"

class RandomGenerator {
    public:
        union Seed {
                uint64 b64;
                struct {
#ifdef LOVE_BIG_ENDIAN
                        uint32 high;
                        uint32 low;
#else
                        uint32 low;
                        uint32 high;
#endif
                } b32;
        };

        RandomGenerator();
        virtual ~RandomGenerator() {};

        uint64 rand();

        inline double random() {
            uint64 r = rand();

            union {
                    uint64 i;
                    double d;
            } u;
            u.i = ((0x3FFULL) << 52) | (r >> 12);

            return u.d - 1.0;
        }

        inline double random(double max) {
            return random() * max;
        }

        inline double random(double min, double max) {
            return random() * (max - min) + min;
        }

        double randomNormal(double stddev);

        void setSeed(Seed seed);

        Seed getSeed() const;

        void setState(const std::string &statestr);

        std::string getState() const;

    private:
        Seed seed;
        Seed rng_state;
        double last_randomnormal;
};
