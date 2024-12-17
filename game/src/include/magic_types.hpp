#pragma once

#include "raylib.h"
#include <string>
enum class MagicType {
    LIGHTNING_SPELL,
    FIRE_SPELL,
    ICE_SPELL,
    METEOR_SPELL,
};

int get_spell_range(MagicType type, int level);
int get_max_enemy_count(MagicType type, int level);
int get_spell_cost(MagicType type, int level);
int get_spell_damage(MagicType type, int level);
void draw_magic_icon(MagicType type, Rectangle rect);
std::string getMagicText(MagicType type);
