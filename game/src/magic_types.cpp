#include "magic_types.hpp"
#include "constants.h"
#include "globals.h"
#include "raylib.h"

int get_spell_range(MagicType type, int level) {
    switch (type) {
    case MagicType::LIGHTNING_SPELL:
        return 120 + (level - 1) * 20;
    case MagicType::FIRE_SPELL:
        return 100 + (level - 1) * 20;
    case MagicType::ICE_SPELL:
        return 130 + (level - 1) * 20;
    case MagicType::METEOR_SPELL:
        return 150 + (level - 1) * 20;
    }
    return 0;
}

int get_max_enemy_count(MagicType type, int level) {
    switch (type) {
    case MagicType::LIGHTNING_SPELL:
        return 15 + (level - 1) * 5;
    case MagicType::FIRE_SPELL:
        return 20 + (level - 1) * 5;
    case MagicType::ICE_SPELL:
        return 25 + (level - 1) * 5;
    case MagicType::METEOR_SPELL:
        return 25 + (level - 1) * 5;
    }
    return 0;
}

int get_spell_cost(MagicType type, int level) {
    switch (type) {
    case MagicType::LIGHTNING_SPELL:
        return 20;
    case MagicType::FIRE_SPELL:
        return 25;
    case MagicType::ICE_SPELL:
        return 25;
    case MagicType::METEOR_SPELL:
        return 30;
    }
    return 0;
}

int get_spell_damage(MagicType type, int level) {
    switch (type) {
    case MagicType::LIGHTNING_SPELL:
        return 20 + (level - 1) * 5;
    case MagicType::FIRE_SPELL:
        return 25 + (level - 1) * 5;
    case MagicType::ICE_SPELL:
        return 30 + (level - 1) * 5;
    case MagicType::METEOR_SPELL:
        return 35 + (level - 1) * 5;
    }
    return 0;
}

std::string getMagicText(MagicType type) {
    switch (type) {
    case MagicType::LIGHTNING_SPELL:
        return "Lightning Spell";
    case MagicType::FIRE_SPELL:
        return "Fire Spell";
    case MagicType::ICE_SPELL:
        return "Ice Spell";
    case MagicType::METEOR_SPELL:
        return "Meteor Spell";
    };
}

void draw_magic_icon(MagicType type, Rectangle rect) {
    getSpriteHolder()->drawSprite(LIGHTNING_ICON, rect);
}
