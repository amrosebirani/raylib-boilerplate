#pragma once

#include "property_type.hpp"
#include <memory>
#include <string>
#include <vector>
class UpgradeInfo {
    public:
        UpgradeInfo(int start_level, int next_level,
                    std::vector<std::string> key_points)
            : start_level(start_level), next_level(next_level),
              key_points(key_points) {};
        int start_level;
        int next_level;
        std::vector<std::string> key_points;
};

std::shared_ptr<UpgradeInfo> getUpgradeInfo(PropertyType type, int level);
void setUpgradeInfo();
