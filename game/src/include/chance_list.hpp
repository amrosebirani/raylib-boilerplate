#pragma once
#include <vector>
#include <utility>
#include <string>

class ChanceList {
    public:
        ChanceList(std::vector<std::pair<std::string, int>> chance_def);
        std::string next();
        std::vector<std::pair<std::string, int>> chance_def;

    private:
        std::vector<std::string> chance_list;
};
