#include "sprite_holder.hpp"

SpriteHolder::SpriteHolder() {
}

SpriteHolder::~SpriteHolder() {
    for (auto &sprite : sprites) {
        UnloadTexture(sprite.second->texture);
        delete sprite.second;
    }
}

void SpriteHolder::initSprites(std::vector<SpriteConfig *> spriteConfigs) {
    for (auto &spriteConfig : spriteConfigs) {
        std::string spath = spriteConfig->path;
        Texture2D texture = LoadTexture(spath.c_str());
        spriteConfig->texture = texture;
        sprites.emplace(spriteConfig->sprite_id, spriteConfig);
        // sprites[spriteConfig->sprite_id] = spriteConfig;
    }
}

void SpriteHolder::drawSprite(std::string sprite_id, int sprite_no,
                              Rectangle dest_rec, Vector2 origin,
                              float rotation, bool flipped) {
    SpriteConfig *spriteConfig = sprites[sprite_id];
    int swidth = spriteConfig->texture.width;
    int sheight = spriteConfig->texture.height;
    int c = swidth / spriteConfig->width;
    int r = sheight / spriteConfig->height;
    if (sprite_no < 0 || sprite_no >= c * r) {
        return;
    }
    int y = sprite_no / c;
    int x = sprite_no % c;
    float width = spriteConfig->width;
    float height = spriteConfig->height;
    Rectangle source_rec = {x * width, y * height, width, height};
    if (flipped) {
        source_rec.width *= -1;
    }
    DrawTexturePro(spriteConfig->texture, source_rec, dest_rec, origin,
                   rotation, WHITE);
}

void SpriteHolder::drawSpriteWithColor(std::string sprite_id, int sprite_no,
                                       Rectangle dest_rec, Color sprite_color,
                                       Vector2 origin, float rotation) {
    SpriteConfig *spriteConfig = sprites[sprite_id];
    int swidth = spriteConfig->texture.width;
    int sheight = spriteConfig->texture.height;
    int c = swidth / spriteConfig->width;
    int r = sheight / spriteConfig->height;
    if (sprite_no < 0 || sprite_no >= c * r) {
        return;
    }
    int y = sprite_no / c;
    int x = sprite_no % c;
    float width = spriteConfig->width;
    float height = spriteConfig->height;
    Rectangle source_rec = {x * width, y * height, width, height};
    DrawTexturePro(spriteConfig->texture, source_rec, dest_rec, origin,
                   rotation, sprite_color);
}

void SpriteHolder::drawSprite(std::string sprite_id, Rectangle dest_rec,
                              Vector2 origin, float rotation) {
    SpriteConfig *spriteConfig = sprites[sprite_id];
    DrawTexturePro(spriteConfig->texture,
                   {0, 0, spriteConfig->texture.width * 1.0f,
                    spriteConfig->texture.height * 1.0f},
                   dest_rec, origin, rotation, WHITE);
}

Vector2 SpriteHolder::getSpriteSize(std::string sprite_id) {
    SpriteConfig *spriteConfig = sprites[sprite_id];
    return {spriteConfig->texture.width * 1.0f,
            spriteConfig->texture.height * 1.0f};
}
