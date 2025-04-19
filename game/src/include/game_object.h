#pragma once
#include "timer.h"
#include <fstream>
#include <time.h>
#include <unordered_map>
#include "raylib.h"
#include "game_object_types.h"
#include <array>
#include <random>

using UUID = std::array<uint8_t, 16>;

inline UUID generateUUID() {
    static std::random_device rd;
    static std::mt19937_64 eng(rd());
    UUID id;
    for (int i = 0; i < 16; ++i) {
        id[i] = static_cast<uint8_t>(eng() & 0xFF);
    }
    return id;
}

class GameObject : public std::enable_shared_from_this<GameObject> {
    public:
        virtual void draw() = 0;
        virtual void update(float dt) = 0;
        virtual bool isAlive() = 0;
        virtual void die() = 0;
        virtual void init() = 0;
        virtual void cleanupData() = 0;
        virtual void Save(std::ofstream &out) const = 0;
        virtual void Load(std::ifstream &in) = 0;
        virtual GameObjectType getObjectType() = 0;
        virtual Color getColor() {
            return {0, 0, 0, 0};
        }
        void gameObjectSave(std::ofstream &out) const;
        UUID getId();

        float x;
        float y;

        GameObject(float x, float y);
        GameObject(float x, float y, UUID id);
        GameObject(std::ifstream &in);

        std::shared_ptr<GameObject> get_shared_ptr();

        float depth = 50;
        time_t creation_time;
        std::vector<std::string> graphics_types;
        bool raised = false;

    protected:
        std::shared_ptr<Timer> timer;
        std::shared_ptr<std::unordered_map<std::string, float>> params;
        UUID id;
};
