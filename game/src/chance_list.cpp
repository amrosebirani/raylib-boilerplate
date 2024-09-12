#include "chance_list.hpp"
#include "raylib.h"
#include <string>

ChanceList::ChanceList(std::vector<std::pair<std::string, int>> chance_def): chance_def(chance_def) {};

std::string ChanceList::next() {

    if (chance_list.size() == 0) {
        for (auto& pp : chance_def) {
            for (int i=0; i<pp.second; i++) {
                chance_list.push_back(pp.first);
            }
        }
    }

    int random_index = GetRandomValue(0, chance_list.size()-1);
    std::string ret_val = chance_list[random_index];
    //chance_list.erase(chance_list.begin() + random_index);
    std::vector<std::string> newChanceList;
    for (int i=0; i<chance_list.size(); i++) {
        if (i == random_index)
            continue;
        newChanceList.push_back(chance_list[i]);
    }
    chance_list = newChanceList;
    return ret_val;
}
