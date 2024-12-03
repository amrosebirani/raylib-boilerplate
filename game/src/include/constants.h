#pragma once
#include "box2d/b2_math.h"
#include "box2d/b2_types.h"
#include <string>
#include <vector>
#include "raylib.h"

const int VIRTUAL_WIDTH = 800;
const int VIRTUAL_HEIGHT = 450;

constexpr float PIXEL_TO_METER_SCALE = 30.0f;

constexpr uint16 CATEGORY_WARRIOR = 0x0001;
constexpr uint16 CATEGORY_PROJECTILE = 0x0002;
constexpr uint16 CATEGORY_ENEMY = 0x0004;
constexpr uint16 CATEGORY_COLLECTIBLE = 0x0008;
constexpr uint16 CATEGORY_ENEMY_PROJECTILE = 0x0010;
constexpr uint16 CATEGORY_BUILDING = 0x0020;
constexpr uint16 CATEGORY_FORMATION = 0x0040;
constexpr uint16 CATEGORY_DEFENSE_TOWER_SENSOR = 0x0080;
constexpr uint16 CATEGORY_WARRIOR_SENSOR = 0x0080;
constexpr uint16 CATEGORY_DEFENSE_TOWER = 0x0100;
constexpr uint16 CATEGORY_ENVIRONMENT = 0x0200;
constexpr uint16 CATEGORY_BUILDING_SENSOR = 0x0400;
constexpr uint16 CATEGORY_WARRIOR_OF = 0x0800;

const int CASTLE_WIDTH = 250;
const int CASTLE_HEIGHT = 250;
const int ROAD_WIDTH = 10;
const int PROPERTY_WIDTH = 50;
const int PROPERTY_HEIGHT = 50;
const int DEFAULT_PROPERTY_RINGS = 2;
const int TOWER_UNIT_DISTANCE = 100;
const int TOWER_BASE_DISTANCE = 130;
const int LIGHTNING_TOWER_DAMAGE = 20;
const float LIGHTNING_TOWER_ATIME = 1.2;

const float BASE_WOOD_PRODUCTION_CYCLE = 3.0f;
const float BASE_STONE_PRODUCTION_CYCLE = 5.0f;
const float BASE_FOOD_PRODUCTION_CYCLE = 2.0f;
const float BASE_GOLD_PRODUCTION_CYCLE = 10.0f;

const float BASE_WOOD_PRODUCTION = 10.0f;
const float BASE_STONE_PRODUCTION = 10.0f;
const float BASE_FOOD_PRODUCTION = 15.0f;
const float BASE_GOLD_PRODUCTION = 5.0f;

const std::string BOW_SPRITE_ID = "bow";
const int ADD_BOW_SPRITE_NO = 7;
const std::string TERRAIN_OP_SPRITE_ID = "ground_02";
const std::string TOWER_SPRITE_ID = "tower";
const std::string ARCHER_ATTACKS_SPRITE_ID = "archer_attacks";
const std::string ARCHER_BUST_SPRITE_ID = "archer_bust";
const std::string ARCHER_IDLE_BOW_SPRITE_ID = "archer_idle_bow";
const std::string ARCHER_IDLE_SPRITE_ID = "archer_idle";
const std::string ARROW_SPRITE_ID = "arrow";
const std::string CASTLE_SPRITE_ID = "castle";

const std::string INFANTRY_ICON = "infantry_icon";
const std::string ARCHERY_ICON = "archery_icon";
const std::string WIZARDRY_ICON = "wizardry_icon";

const std::string UI_ICONS = "ui_icons";
// for footman
const std::string FOOTMAN_ATTACKS_SPRITE_ID = "footman_attack";
const std::string FOOTMAN_BUST_SPRITE_ID = "footman_bust";
const std::string FOOTMAN_IDLE_SPRITE_ID = "footman_idle";
const std::string FOOTMAN_RUN_SPRITE_ID = "footman_run";
const std::string FOOTMAN_PAPERDOLL_SPRITE_ID = "footman_paperdoll";
const std::string FOOTMAN_DEAD_SPRITE_ID = "footman_dead";
// for spearman
const std::string SPEARMAN_ATTACKS_SPRITE_ID = "spearman_attack";
const std::string SPEARMAN_BUST_SPRITE_ID = "spearman_bust";
const std::string SPEARMAN_IDLE_SPRITE_ID = "spearman_idle";
const std::string SPEARMAN_RUN_SPRITE_ID = "spearman_run";
const std::string SPEARMAN_PAPERDOLL_SPRITE_ID = "spearman_paperdoll";
const std::string SPEARMAN_DEAD_SPRITE_ID = "spearman_dead";
const std::string SPEARMAN_KO_SPRITE_ID = "spearman_ko";
// for knight
const std::string KNIGHT_ATTACKS_SPRITE_ID = "knight_attack";
const std::string KNIGHT_BUST_SPRITE_ID = "knight_bust";
const std::string KNIGHT_IDLE_SPRITE_ID = "knight_idle";
const std::string KNIGHT_RUN_SPRITE_ID = "knight_run";
const std::string KNIGHT_PAPERDOLL_SPRITE_ID = "knight_paperdoll";
const std::string KNIGHT_DEAD_SPRITE_ID = "knight_dead";
// for mage
const std::string MAGE_ATTACKS_SPRITE_ID = "mage_attack";
const std::string MAGE_BUST_SPRITE_ID = "mage_bust";
const std::string MAGE_IDLE_SPRITE_ID = "mage_idle";
const std::string MAGE_RUN_SPRITE_ID = "mage_run";
const std::string MAGE_PAPERDOLL_SPRITE_ID = "mage_paperdoll";
const std::string MAGE_DEAD_SPRITE_ID = "mage_dead";
// for enemy1
const std::string ENEMY1_ATTACKS_SPRITE_ID = "enemy1_attack";
const std::string ENEMY1_BUST_SPRITE_ID = "enemy1_bust";
const std::string ENEMY1_PAPERDOLL_SPRITE_ID = "enemy1_paperdoll";
const std::string ENEMY1_RUN_SPRITE_ID = "enemy1_run";
const std::string ENEMY1_DEAD_SPRITE_ID = "enemy1_dead";
const std::string ENEMY1_KO_SPRITE_ID = "enemy1_ko";
const std::string ENEMY1_IDLE_SPRITE_ID = "enemy1_idle";

// for ghoul
const std::string GHOUL_ATTACKS_SPRITE_ID = "ghoul_attack";
const std::string GHOUL_BUST_SPRITE_ID = "ghoul_bust";
const std::string GHOUL_DEAD_SPRITE_ID = "ghoul_dead";
const std::string GHOUL_RUN_SPRITE_ID = "ghoul_move";
const std::string GHOUL_IDLE_SPRITE_ID = "ghoul_idle";

// for necromancer
const std::string NECROMANCER_ATTACKS_SPRITE_ID = "necromancer_attack";
const std::string NECROMANCER_BUST_SPRITE_ID = "necromancer_bust";
const std::string NECROMANCER_DEAD_SPRITE_ID = "necromancer_dead";
const std::string NECROMANCER_RUN_SPRITE_ID = "necromancer_move";
const std::string NECROMANCER_IDLE_SPRITE_ID = "necromancer_idle";

// for rat
const std::string RAT_ATTACKS_SPRITE_ID = "rat_attack";
const std::string RAT_BUST_SPRITE_ID = "rat_bust";
const std::string RAT_RUN_SPRITE_ID = "rat_move";
const std::string RAT_IDLE_SPRITE_ID = "rat_idle";
const std::string RAT_DEAD_SPRITE_ID = "rat_dead";

// for spider
const std::string SPIDER_ATTACKS_SPRITE_ID = "spider_attack";
const std::string SPIDER_BUST_SPRITE_ID = "spider_bust";
const std::string SPIDER_RUN_SPRITE_ID = "spider_move";
const std::string SPIDER_IDLE_SPRITE_ID = "spider_idle";
const std::string SPIDER_DEAD_SPRITE_ID = "spider_dead";

// for skeleton
const std::string SKELETON_ATTACKS_SPRITE_ID = "skeleton_attack";
const std::string SKELETON_BUST_SPRITE_ID = "skeleton_bust";
const std::string SKELETON_RUN_SPRITE_ID = "skeleton_move";
const std::string SKELETON_IDLE_SPRITE_ID = "skeleton_idle";
const std::string SKELETON_DEAD_SPRITE_ID = "skeleton_dead";

// for skeleton prince
const std::string SKELETON_PRINCE_ATTACKS_SPRITE_ID = "skeleton_prince_attack";
const std::string SKELETON_PRINCE_BUST_SPRITE_ID = "skeleton_prince_bust";
const std::string SKELETON_PRINCE_RUN_SPRITE_ID = "skeleton_prince_move";
const std::string SKELETON_PRINCE_IDLE_SPRITE_ID = "skeleton_prince_idle";
const std::string SKELETON_PRINCE_DEAD_SPRITE_ID = "skeleton_prince_dead";

// for buck
const std::string BUCK_ATTACKS_SPRITE_ID = "buck_attack";
const std::string BUCK_BUST_SPRITE_ID = "buck_bust";
const std::string BUCK_RUN_SPRITE_ID = "buck_move";
const std::string BUCK_IDLE_SPRITE_ID = "buck_idle";
const std::string BUCK_DEAD_SPRITE_ID = "buck_dead";

// for wolf

const std::string WOLF_ATTACKS_SPRITE_ID = "wolf_attack";
const std::string WOLF_BUST_SPRITE_ID = "wolf_bust";
const std::string WOLF_RUN_SPRITE_ID = "wolf_move";
const std::string WOLF_IDLE_SPRITE_ID = "wolf_idle";
const std::string WOLF_DEAD_SPRITE_ID = "wolf_dead";

// for zombie1
const std::string ZOMBIE1_ATTACKS_SPRITE_ID = "zombie1_attack";
const std::string ZOMBIE1_BUST_SPRITE_ID = "zombie1_bust";
const std::string ZOMBIE1_RUN_SPRITE_ID = "zombie1_move";
const std::string ZOMBIE1_IDLE_SPRITE_ID = "zombie1_idle";
const std::string ZOMBIE1_DEAD_SPRITE_ID = "zombie1_dead";

// for zombie2
const std::string ZOMBIE2_ATTACKS_SPRITE_ID = "zombie2_attack";
const std::string ZOMBIE2_BUST_SPRITE_ID = "zombie2_bust";
const std::string ZOMBIE2_RUN_SPRITE_ID = "zombie2_move";
const std::string ZOMBIE2_IDLE_SPRITE_ID = "zombie2_idle";
const std::string ZOMBIE2_DEAD_SPRITE_ID = "zombie2_dead";

// for zombie3
const std::string ZOMBIE3_ATTACKS_SPRITE_ID = "zombie3_attack";
const std::string ZOMBIE3_BUST_SPRITE_ID = "zombie3_bust";
const std::string ZOMBIE3_RUN_SPRITE_ID = "zombie3_move";
const std::string ZOMBIE3_IDLE_SPRITE_ID = "zombie3_idle";
const std::string ZOMBIE3_DEAD_SPRITE_ID = "zombie3_dead";

// for zombie4
const std::string ZOMBIE4_ATTACKS_SPRITE_ID = "zombie4_attack";
const std::string ZOMBIE4_BUST_SPRITE_ID = "zombie4_bust";
const std::string ZOMBIE4_RUN_SPRITE_ID = "zombie4_move";
const std::string ZOMBIE4_IDLE_SPRITE_ID = "zombie4_idle";
const std::string ZOMBIE4_DEAD_SPRITE_ID = "zombie4_dead";

// for zombie5
const std::string ZOMBIE5_ATTACKS_SPRITE_ID = "zombie5_attack";
const std::string ZOMBIE5_BUST_SPRITE_ID = "zombie5_bust";
const std::string ZOMBIE5_RUN_SPRITE_ID = "zombie5_move";
const std::string ZOMBIE5_IDLE_SPRITE_ID = "zombie5_idle";
const std::string ZOMBIE5_DEAD_SPRITE_ID = "zombie5_dead";

// for zombie6
const std::string ZOMBIE6_ATTACKS_SPRITE_ID = "zombie6_attack";
const std::string ZOMBIE6_BUST_SPRITE_ID = "zombie6_bust";
const std::string ZOMBIE6_RUN_SPRITE_ID = "zombie6_move";
const std::string ZOMBIE6_IDLE_SPRITE_ID = "zombie6_idle";
const std::string ZOMBIE6_DEAD_SPRITE_ID = "zombie6_dead";

// for zombie giant
const std::string ZOMBIE_GIANT_ATTACKS_SPRITE_ID = "zombie_giant_attack";
const std::string ZOMBIE_GIANT_BUST_SPRITE_ID = "zombie_giant_bust";
const std::string ZOMBIE_GIANT_RUN_SPRITE_ID = "zombie_giant_move";
const std::string ZOMBIE_GIANT_IDLE_SPRITE_ID = "zombie_giant_idle";
const std::string ZOMBIE_GIANT_DEAD_SPRITE_ID = "zombie_giant_dead";

// for gems and coins
const std::string COINS_AND_GEMS = "coins_and_gems";

const std::string TREE1_SPRITE_ID = "tree1";
const std::string TREE2_SPRITE_ID = "tree2";
const std::string TREE3_SPRITE_ID = "tree3";
const std::string TREE4_SPRITE_ID = "tree4";
const std::string TREE5_SPRITE_ID = "tree5";
const std::string TREE6_SPRITE_ID = "tree6";
const std::string TREE7_SPRITE_ID = "tree7";

// for archery buildings
const std::string ARCHERY1_SPRITE_ID = "archery1";
const std::string ARCHERY2_SPRITE_ID = "archery2";
const std::string ARCHERY3_SPRITE_ID = "archery3";

// for barracks buildings
const std::string BARRACKS1_SPRITE_ID = "barracks1";
const std::string BARRACKS2_SPRITE_ID = "barracks2";
const std::string BARRACKS3_SPRITE_ID = "barracks3";
const std::string BARRACKS4_SPRITE_ID = "barracks4";

// for castle buildings
//
const std::string CASTLE1_SPRITE_ID = "castle1";
const std::string CASTLE2_SPRITE_ID = "castle2";
const std::string CASTLE3_SPRITE_ID = "castle3";
const std::string CASTLE4_SPRITE_ID = "castle4";

// for house buildings
const std::string HOUSE1_SPRITE_ID = "house1";
const std::string HOUSE2_SPRITE_ID = "house2";
const std::string HOUSE3_SPRITE_ID = "house3";
const std::string HOUSE4_SPRITE_ID = "house4";

// for wizardry buildings

const std::string WIZARDRY1_SPRITE_ID = "wizardry1";
const std::string WIZARDRY2_SPRITE_ID = "wizardry2";
const std::string WIZARDRY3_SPRITE_ID = "wizardry3";

const std::string LIGHTNING_TOWER_SPRITE_ID = "lightning_tower";
const std::string BLOCK_TOWER1_SPRITE_ID = "block_tower1";
const std::string BLOCK_TOWER2_SPRITE_ID = "block_tower2";

const float DEFENSE_TOWER_RADIUS = 15.0f;
const float DEFENSE_TOWER_SENSOR_RADIUS = 70.0f;
const float TREE_HEIGHT = 90.0f;
const std::vector<Color> bloodColors = {
    {178, 34, 34, 255}, {90, 0, 0, 255}, {75, 0, 0, 255}};
const std::vector<Color> enemyBloodColors = {
    {0, 128, 128, 255}, {0, 0, 139, 255}, {0, 31, 63, 255}};
constexpr Color default_color = {222, 222, 222, 255};
constexpr Color background_color = {16, 16, 16, 255};
constexpr Color ammo_color = {123, 200, 164, 255};
constexpr Color boost_color = {76, 195, 217, 255};

constexpr float JOYSTICK_RADIUS = 1 / 11.0f;
constexpr float JOYSTICK_BASE_RADIUS = 1 / 8.0f;

constexpr float ENEMY_BASE_MVSPD = 55.0f;
constexpr float FORMATION_BASE_MVSPD = 150.0f;
constexpr float WARRIOR_BASE_MVSPD = 80.0f;
constexpr float ARROW_BASE_MVSPD = 220.0f;

constexpr int WARRIOR_RESPAWN_TIME = 30;
constexpr int FORMATION_RESPAWN_TIME = 20;
