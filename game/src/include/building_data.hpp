#pragma once

#include <string>
#include <vector>
#include "raylib.h"

struct BuildingData {
        std::string building_id;
        float w;
        float h;
        float o_x;
        float o_y;
        float h_y;
        // counterclockwise vertices, keeping top left corner as the origin
        std::vector<Vector2> vertices;
        float aligned_l;

        BuildingData(std::string building_id, float w, float h, float o_x,
                     float o_y, std::vector<Vector2> vertices,
                     float aligned_l = 0, float h_y = 0)
            : building_id(building_id), w(w), h(h), o_x(o_x), o_y(o_y),
              vertices(vertices), aligned_l(aligned_l), h_y(h_y){};
};
