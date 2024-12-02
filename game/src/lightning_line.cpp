#include "lightning_line.hpp"
#include "box2d/b2_math.h"
#include "game_object.h"
#include "utils.h"

void LightningLine::update(float dt) {
    timer->update(dt);
}

void LightningLine::draw() {
    Color cc1 = c1;
    Color cc2 = c2;
    cc1.a = (*params)["alpha"];
    cc2.a = (*params)["alpha"];
    for (auto &segment : segments) {
        DrawLineEx(segment.first, segment.second, 2.5, cc1);
        DrawLineEx(segment.first, segment.second, 1.5, cc2);
    }
}

bool LightningLine::isAlive() {
    return alive;
}

void LightningLine::die() {
    alive = false;
}

void LightningLine::generateLightning() {
    segments.push_back({Vector2{x1, y1}, Vector2{x2, y2}});
    float offsetAmount = maximumOffset;
    for (int i = 0; i < generationCount; i++) {
        std::vector<std::pair<Vector2, Vector2>> newSegments;
        for (int j = 0; j < segments.size(); j++) {
            Vector2 start = segments[j].first;
            Vector2 end = segments[j].second;
            Vector2 mid = Vector2{(start.x + end.x) / 2, (start.y + end.y) / 2};
            float offset = randomFloatInRange(-offsetAmount, offsetAmount);
            b2Vec2 unit = {(end.x - start.x), (end.y - start.y)};
            unit.Normalize();
            mid.x += unit.y * offset;
            mid.y -= unit.x * offset;
            newSegments.push_back({start, mid});
            newSegments.push_back({mid, end});
        }
        segments = newSegments;
        offsetAmount /= 2;
    }
}

LightningLine::LightningLine(float x1, float y1, float x2, float y2, Color c1,
                             Color c2)
    : x1(x1), y1(y1), x2(x2), y2(y2), GameObject(0, 0), c1(c1), c2(c2) {
    generateLightning();
    params = std::make_shared<std::unordered_map<std::string, float>>();
    (*params)["alpha"] = 205;
    timer->tween(1.05, params, {{"alpha", 0}}, "in-out-cubic",
                 [this]() { this->die(); }, "", {});
}

void LightningLine::init() {
}

void LightningLine::cleanupData() {
}
