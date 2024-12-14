
#include "stat.hpp"
#include "chance_list.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <utility>
#include <cmath>
// make an enum for type which is eithe a STAT or CHANCE

enum class UpgradeType { STAT, CHANCE };

class UpgradeEntity {
    public:
        UpgradeEntity(int chance_val, float lm,
                      std::vector<std::string> display_strs) {
            type = UpgradeType::CHANCE;
            std::vector<std::pair<std::string, int>> chance_def = {
                {"true", ceil(chance_val * lm)},
                {"false", 100 - ceil(lm * chance_val)}};
            chance_list = std::make_shared<ChanceList>(chance_def);
            this->display_strs = display_strs;
        }
        UpgradeEntity(float stat_val, int countVal, StatType stype,
                      std::vector<std::string> display_strs) {
            this->type = UpgradeType::STAT;
            this->display_strs = display_strs;
            stat = std::make_shared<Stat>(stat_val, countVal, stype);
        }
        UpgradeType type;
        std::shared_ptr<Stat> stat;
        std::shared_ptr<ChanceList> chance_list;
        std::vector<std::string> display_strs;
};

class UpgradeContent {
    public:
        void initStats();
        void initChances(float lm);
        UpgradeContent();
        std::unordered_map<std::string, std::shared_ptr<UpgradeEntity>>
            upgrade_entities;
        float get_stat(std::string key);
        int get_stat_count(std::string key);
        void update_stat(std::string key, float value, std::string tag = "");
        void update_stat_count(std::string key, int value,
                               std::string tag = "");
        void clear_stat(std::string key, std::string tag = "");
        bool get_chance(std::string key);
        void update_chance(std::string key, int value);
        std::vector<std::string> get_display_strs(std::string key);
};
