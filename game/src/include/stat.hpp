#pragma once

#include <string>
#include <map>
// define enum with two types count and percentage
enum class StatType { COUNT, PERCENTAGE };

class Stat {
    public:
        Stat(float base, int baseCount, StatType type)
            : base(base), baseCount(baseCount), type(type) {
            if (type == StatType::COUNT) {
                valueCount = baseCount;
            } else {
                additive = 0;
                value = base * (1 + additive);
            }
        };

        void update();
        void increase(float percentage, std::string tag = "");
        void decrease(float percentage, std::string tag = "");
        void increaseCount(int count, std::string tag = "");
        void decreaseCount(int count, std::string tag = "");
        void clearStat(std::string tag);
        void setBase(float base);
        void setBaseCount(int baseCount);
        float value;

        float base;

        int baseCount;
        float valueCount;

    private:
        float additive;
        int flat_additive;
        std::map<std::string, float> additive_map;
        std::map<std::string, int> flat_additive_map;
        StatType type;
};
