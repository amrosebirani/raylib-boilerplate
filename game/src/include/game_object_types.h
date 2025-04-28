#pragma once

enum class GameObjectType {
    ARCHER,          // saved in game objects
    FIREBALL,        // not saved
    ARROW,           // not saved
    BARRACK,         // saved through region
    CASTLE,          // saved through region
    ENEMY,           // saved in game objects
    GEM,             // saved in game objects
    HOUSE_BLOCK,     // saved through region
    WARRIOR,         // saved in game objects
    WARRIOR_DUMMY,   // not saved
    WIZARDRY,        // saved through region
    ARCHERY,         // saved through region
    ATTACK_TOWER,    // saved through region
    LIGHTNING_TOWER, // saved through region
    DEAD_BODY,       // not saved
    NON_TYPE,        // not saved
    SPELL            // not saved
};
