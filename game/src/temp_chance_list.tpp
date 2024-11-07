#pragma once

template <typename T>
TempChanceList<T>::TempChanceList(std::vector<std::pair<T, int>> chance_def) {
    this->chance_def = chance_def;
}

template <typename T> T TempChanceList<T>::next() {
    if (chance_list.size() == 0) {
        for (auto &pair : chance_def) {
            for (int i = 0; i < pair.second; i++) {
                chance_list.push_back(pair.first);
            }
        }
    }

    int random_index = GetRandomValue(0, chance_list.size() - 1);
    T ret_val = chance_list[random_index];
    std::vector<T> new_chance_list;
    for (int i = 0; i < chance_list.size(); i++) {
        if (i != random_index) {
            new_chance_list.push_back(chance_list[i]);
        }
    }
    chance_list = new_chance_list;
    return ret_val;
}
