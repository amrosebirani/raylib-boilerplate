#pragma once
#include "building_data.hpp"
#include "constants.h"
#include "raylib.h"
#include <unordered_map>
#include <string>
#include <vector>

struct SpriteConfig {
        std::string sprite_id;
        std::string path;
        int width;
        int height;
        Texture2D texture;

        // add a constructor here

        SpriteConfig(std::string sprite_id, std::string path, int width,
                     int height) {
            this->sprite_id = sprite_id;
            this->path = path;
            this->width = width;
            this->height = height;
        }
};

class SpriteHolder {
    public:
        SpriteHolder();
        ~SpriteHolder();
        void initSprites(std::vector<SpriteConfig *> spriteConfigs);
        void drawSpriteWithColor(std::string sprite_id, int sprite_no,
                                 Rectangle dest_rec, Color sprite_color,
                                 Vector2 origin = {0, 0}, float rotation = 0);
        void drawSpriteWithColor(std::string sprite_id, Rectangle dest_rec,
                                 Color sprite_color, Vector2 origin = {0, 0},
                                 float rotation = 0);
        void drawSprite(std::string sprite_id, int sprite_no,
                        Rectangle dest_rec, Vector2 origin = {0, 0},
                        float rotation = 0, bool flipped = false);
        void drawSprite(std::string sprite_id, Rectangle dest_rec,
                        Vector2 origin = {0, 0}, float rotation = 0);
        Vector2 getSpriteSize(std::string sprite_id);
        void drawSprite(BuildingData *building, float x, float y,
                        float perc_cover, unsigned char alpha = 255,
                        float scale_length = CASTLE_WIDTH);

    private:
        std::unordered_map<std::string, SpriteConfig *> sprites;
};
