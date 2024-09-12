#pragma once
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
constexpr uint16 CATEGORY_CASTLE = 0x0020;
constexpr uint16 CATEGORY_FORMATION = 0x0040;
constexpr uint16 CATEGORY_DEFENSE_TOWER_SENSOR = 0x0080;
constexpr uint16 CATEGORY_DEFENSE_TOWER = 0x0100;

const int CASTLE_WIDTH = 310;
const int CASTLE_HEIGHT = 310;
const int ROAD_WIDTH = 10;
const int PROPERTY_WIDTH = 50;
const int PROPERTY_HEIGHT = 50;
const int DEFAULT_PROPERTY_RINGS = 2;
const int TOWER_UNIT_DISTANCE = 120;
const int TOWER_BASE_DISTANCE = 150;

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
const std::string TOWER_SPRITE_ID = "tower";
const std::string ARCHER_ATTACKS_SPRITE_ID = "archer_attacks";
const std::string ARCHER_BUST_SPRITE_ID = "archer_bust";
const std::string ARCHER_IDLE_BOW_SPRITE_ID = "archer_idle_bow";
const std::string ARCHER_IDLE_SPRITE_ID = "archer_idle";
const std::string ARROW_SPRITE_ID = "arrow";
const std::string CASTLE_SPRITE_ID = "castle";
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

const float DEFENSE_TOWER_RADIUS = 15.0f;
const float DEFENSE_TOWER_SENSOR_RADIUS = 70.0f;
const std::vector<Color> bloodColors = {
    {178, 34, 34, 255}, {90, 0, 0, 255}, {75, 0, 0, 255}};
const std::vector<Color> enemyBloodColors = {
    {0, 128, 128, 255}, {0, 0, 139, 255}, {0, 31, 63, 255}};
