#pragma once
#include <vector>
#include <utility>

template <typename T> class TempChanceList {
    public:
        TempChanceList(std::vector<std::pair<T, int>> chance_def);
        T next();
        std::vector<std::pair<T, int>> chance_def;

    private:
        std::vector<T> chance_list;
};

#include "temp_chance_list.tpp"
