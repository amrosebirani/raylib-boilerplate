#include "utils.h"
#include "box2d/b2_circle_shape.h"
#include "building_data.hpp"
#include "collider_user_data.h"
#include "collider_factory.hpp"
#include "direction.hpp"
#include "enemy_types.h"
#include "globals.h"
#include "constants.h"
#include "box2d/b2_body.h"
#include "property_type.hpp"
#include "raylib.h"
#include "warrior_types.h"
#include "ParticleSystem.h"
#include <unordered_map>

std::unordered_map<std::string, BuildingData *> buildingData;
std::unordered_map<PropertyType, int> maxLevel;
std::unordered_map<PropertyType, std::vector<int>> upgradeCoins;
std::unordered_map<PropertyType, std::vector<float>> maxHealthByLevel;
std::unordered_map<PropertyType, std::vector<float>> percentCover;
std::unordered_map<PropertyType, std::vector<float>> buildingProduceTime;
std::unordered_map<PropertyType, std::vector<Vector2>> buildingSummonDim;
std::unordered_map<PropertyType, std::vector<std::vector<WarriorType>>>
    buildingSummonTypes;

int getRandomValue(int value1, int value2) {
    // Initialize random seed the first time this function is called
    static int seedInitialized = 0;
    if (!seedInitialized) {
        srand(time(NULL)); // Use current time as seed for random generator
        seedInitialized = 1;
    }

    // Generate a random number (0 or 1) and return value1 or value2 accordingly
    if (rand() % 2 == 0) {
        return value1;
    } else {
        return value2;
    }
}

int getRandomIntInRange(int value1, int value2) {
    // Initialize random seed the first time this function is called
    static int seedInitialized = 0;
    if (!seedInitialized) {
        srand(time(NULL)); // Use current time as seed for random generator
        seedInitialized = 1;
    }

    // Generate a random number between value1 and value2 (inclusive)
    return rand() % (value2 - value1 + 1) + value1;
}

// Function to generate a random floating point number between min and max
float randomFloatInRange(float min, float max) {
    float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min);       /* [min, max] */
}

std::shared_ptr<b2Body> getWarriorCollider(WarriorType wtype, float x, float y,
                                           ColliderUserData *data,
                                           bool inFormation) {

    data->type = ColliderUserData::Warrior;
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    std::shared_ptr<b2Body> collider = ColliderFactory::newCircleCollider(
        data, x, y, get_warrior_size(wtype), b2_dynamicBody,
        inFormation ? CATEGORY_WARRIOR : CATEGORY_WARRIOR_OF,
        CATEGORY_COLLECTIBLE | CATEGORY_ENEMY | CATEGORY_ENEMY_PROJECTILE |
            CATEGORY_BUILDING |
            (inFormation ? CATEGORY_WARRIOR : CATEGORY_WARRIOR_OF),
        world);
    return collider;
}

std::shared_ptr<b2Body> getWarriorSensor(WarriorType wtype, float x, float y,
                                         ColliderUserData *data) {

    data->type = ColliderUserData::WarriorSensor;
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    std::shared_ptr<b2Body> sensor = ColliderFactory::newCircleSensor(
        data, x, y, get_warrior_size(wtype) * 5, b2_dynamicBody,
        CATEGORY_WARRIOR_SENSOR, CATEGORY_ENEMY, world);
    return sensor;
}

std::shared_ptr<b2Body> getEnemyCollider(EnemyType wtype, float x, float y,
                                         ColliderUserData *data) {

    data->type = ColliderUserData::Enemy;
    std::shared_ptr<Container> cont = getContainer();
    std::shared_ptr<b2World> world = cont->getWorld();
    std::shared_ptr<b2Body> collider = ColliderFactory::newCircleCollider(
        data, x, y, get_enemy_size(wtype), b2_dynamicBody, CATEGORY_ENEMY,
        CATEGORY_WARRIOR | CATEGORY_WARRIOR_OF | CATEGORY_PROJECTILE |
            CATEGORY_ENEMY | CATEGORY_BUILDING | CATEGORY_DEFENSE_TOWER |
            CATEGORY_DEFENSE_TOWER_SENSOR | CATEGORY_BUILDING_SENSOR,
        world);
    return collider;
}

float getMaxCastleHealthByLevel(int level) {
    return 10000 + (level - 1) * 500;
}

float getCastleAttackTimer(int level) {
    switch (level) {
    case 1: {
        return 6;
    }
    case 2: {
        return 5.5;
    }
    case 3: {
        return 5.1;
    }
    default: {
        return 2;
    }
    }
}

b2FixtureDef *getFormationFixtureDef(float radius, float offset_x,
                                     float offset_y) {
    b2CircleShape *circle = new b2CircleShape();
    circle->m_radius = radius / PIXEL_TO_METER_SCALE;
    circle->m_p.Set(offset_x / PIXEL_TO_METER_SCALE,
                    offset_y / PIXEL_TO_METER_SCALE);

    b2FixtureDef *fixtureDef = new b2FixtureDef();

    fixtureDef->shape = circle;
    fixtureDef->density = 1.0f;
    fixtureDef->friction = 1.0f;
    fixtureDef->restitution = 1.0f;
    fixtureDef->filter.maskBits =
        CATEGORY_BUILDING | CATEGORY_DEFENSE_TOWER | CATEGORY_BUILDING_SENSOR;
    fixtureDef->filter.categoryBits = CATEGORY_FORMATION;
    return fixtureDef;
}

std::unordered_map<Direction, int> get_direction_rows() {
    std::unordered_map<Direction, int> direction_rows;
    direction_rows[Direction::NORTH] = 3;
    direction_rows[Direction::NORTH_EAST] = 7;
    direction_rows[Direction::EAST] = 2;
    direction_rows[Direction::SOUTH_EAST] = 6;
    direction_rows[Direction::SOUTH] = 0;
    direction_rows[Direction::SOUTH_WEST] = 4;
    direction_rows[Direction::WEST] = 1;
    direction_rows[Direction::NORTH_WEST] = 5;
    return direction_rows;
}

Direction get_direction(Vector2 dir) {
    float angle = atan2(-dir.y, dir.x);
    Direction d = Direction::NORTH;
    if (angle >= -PI / 8 && angle < PI / 8) {
        d = Direction::EAST;
    } else if (angle >= PI / 8 && angle < 3 * PI / 8) {
        d = Direction::NORTH_EAST;
    } else if (angle >= 3 * PI / 8 && angle < 5 * PI / 8) {
        d = Direction::NORTH;
    } else if (angle >= 5 * PI / 8 && angle < 7 * PI / 8) {
        d = Direction::NORTH_WEST;
    } else if (angle >= 7 * PI / 8 || angle < -7 * PI / 8) {
        d = Direction::WEST;
    } else if (angle >= -7 * PI / 8 && angle < -5 * PI / 8) {
        d = Direction::SOUTH_WEST;
    } else if (angle >= -5 * PI / 8 && angle < -3 * PI / 8) {
        d = Direction::SOUTH;
    } else if (angle >= -3 * PI / 8 && angle < -PI / 8) {
        d = Direction::SOUTH_EAST;
    }
    return d;
}

std::shared_ptr<ParticleSystem> getParticleSystem(Texture2D *texture,
                                                  std::vector<Color> colors) {
    std::shared_ptr<ParticleSystem> ps =
        std::make_shared<ParticleSystem>(texture, 1000);
    ps->setEmissionRate(22);
    ps->setParticleLifetime(0.85, 1.5);
    ps->setEmissionArea(
        ParticleSystem::AreaSpreadDistribution::DISTRIBUTION_NORMAL, 4.67, 4.67,
        0, false);
    ps->setSpeed(10, 30);
    ps->setRadialAcceleration(0);
    ps->setLinearDamping(.9, 1.8);
    ps->setDirection(0);
    ps->setSpread(0);
    ps->setSizes({.003f, .014f, .005f});
    ps->setSizeVariation(1.0f);
    ps->setColor(colors);
    ps->setEmitterLifetime(1.1f);
    return ps;
}

void setBuildingData() {
    buildingData[ARCHERY1_SPRITE_ID] =
        new BuildingData(ARCHERY1_SPRITE_ID, 1131, 678, 539, 381,
                         {Vector2{539, 632}, Vector2{1032, 380},
                          Vector2{539, 131}, Vector2{45, 381}},
                         554, -1);
    buildingData[ARCHERY2_SPRITE_ID] =
        new BuildingData(ARCHERY2_SPRITE_ID, 1204, 877, 533, 603,
                         {Vector2{518, 848}, Vector2{1028, 594},
                          Vector2{548, 358}, Vector2{39, 612}},
                         570, 252);
    buildingData[ARCHERY3_SPRITE_ID] =
        new BuildingData(ARCHERY3_SPRITE_ID, 1314, 1020, 637, 649,
                         {Vector2{610, 958}, Vector2{1247, 637},
                          Vector2{665, 340}, Vector2{24, 661}},
                         713, 141);

    buildingData[BARRACKS1_SPRITE_ID] =
        new BuildingData(BARRACKS1_SPRITE_ID, 1629, 918, 798, 464,
                         {Vector2{912, 841}, Vector2{1536, 521},
                          Vector2{682, 91}, Vector2{56, 409}},
                         957, -1);
    buildingData[BARRACKS2_SPRITE_ID] =
        new BuildingData(BARRACKS2_SPRITE_ID, 768, 703, 318, 560,
                         {Vector2{263, 700}, Vector2{607, 530},
                          Vector2{372, 417}, Vector2{29, 588}},
                         384, 138);
    buildingData[BARRACKS3_SPRITE_ID] =
        new BuildingData(BARRACKS3_SPRITE_ID, 977, 878, 435, 680,
                         {Vector2{426, 876}, Vector2{838, 681},
                          Vector2{443, 486}, Vector2{30, 682}},
                         456, 118);
    buildingData[BARRACKS4_SPRITE_ID] =
        new BuildingData(BARRACKS4_SPRITE_ID, 1021, 823, 505, 531,
                         {Vector2{529, 780}, Vector2{981, 541},
                          Vector2{481, 285}, Vector2{29, 523}},
                         562, 103);

    buildingData[CASTLE1_SPRITE_ID] =
        new BuildingData(CASTLE1_SPRITE_ID, 2178, 1516, 1002, 995,
                         {Vector2{1001, 1479}, Vector2{1969, 995},
                          Vector2{998, 507}, Vector2{25, 993}},
                         1082, -1);
    buildingData[CASTLE2_SPRITE_ID] =
        new BuildingData(CASTLE2_SPRITE_ID, 1150, 924, 468, 664,
                         {Vector2{473, 893}, Vector2{935, 670},
                          Vector2{463, 434}, Vector2{2, 661}},
                         513, 146);
    buildingData[CASTLE3_SPRITE_ID] =
        new BuildingData(CASTLE3_SPRITE_ID, 1197, 1053, 507, 773,
                         {Vector2{510, 1038}, Vector2{1014, 752},
                          Vector2{504, 509}, Vector2{2, 795}},
                         580, 259);
    buildingData[CASTLE4_SPRITE_ID] =
        new BuildingData(CASTLE4_SPRITE_ID, 1150, 1087, 422, 867,
                         {Vector2{396, 1058}, Vector2{800, 853},
                          Vector2{447, 674}, Vector2{44, 880}},
                         453, 125);

    buildingData[HOUSE1_SPRITE_ID] =
        new BuildingData(HOUSE1_SPRITE_ID, 410, 348, 182, 246,
                         {Vector2{150, 322}, Vector2{319, 230},
                          Vector2{209, 174}, Vector2{40, 264}},
                         192, 75);
    buildingData[HOUSE2_SPRITE_ID] =
        new BuildingData(HOUSE2_SPRITE_ID, 410, 474, 163, 381,
                         {Vector2{134, 451}, Vector2{302, 366},
                          Vector2{192, 312}, Vector2{24, 396}},
                         188, 128);
    buildingData[HOUSE3_SPRITE_ID] =
        new BuildingData(HOUSE3_SPRITE_ID, 410, 474, 201, 367,
                         {Vector2{213, 454}, Vector2{376, 373},
                          Vector2{189, 279}, Vector2{25, 362}},
                         182, 149);
    buildingData[HOUSE4_SPRITE_ID] =
        new BuildingData(HOUSE4_SPRITE_ID, 578, 585, 243, 442,
                         {Vector2{246, 559}, Vector2{467, 439},
                          Vector2{241, 324}, Vector2{19, 446}},
                         252, 202);

    buildingData[WIZARDRY1_SPRITE_ID] =
        new BuildingData(WIZARDRY1_SPRITE_ID, 859, 844, 315, 696,
                         {Vector2{351, 832}, Vector2{580, 716},
                          Vector2{282, 560}, Vector2{54, 677}},
                         335, 96);
    buildingData[WIZARDRY2_SPRITE_ID] =
        new BuildingData(WIZARDRY2_SPRITE_ID, 869, 857, 359, 660,
                         {Vector2{467, 821}, Vector2{672, 715},
                          Vector2{251, 499}, Vector2{44, 604}},
                         475, 164);
    buildingData[WIZARDRY3_SPRITE_ID] =
        new BuildingData(WIZARDRY3_SPRITE_ID, 756, 758, 382, 582,
                         {Vector2{377, 752}, Vector2{725, 577},
                          Vector2{387, 413}, Vector2{39, 587}},
                         390, 87);

    buildingData[BLOCK_TOWER1_SPRITE_ID] =
        new BuildingData(BLOCK_TOWER1_SPRITE_ID, 450, 352, 203, 242,
                         {Vector2{200, 328}, Vector2{376, 242},
                          Vector2{206, 156}, Vector2{28, 242}},
                         196, 80);
    buildingData[BLOCK_TOWER2_SPRITE_ID] =
        new BuildingData(BLOCK_TOWER2_SPRITE_ID, 450, 352, 202, 243,
                         {Vector2{202, 330}, Vector2{377, 242},
                          Vector2{202, 156}, Vector2{27, 244}},
                         196, 80);
    buildingData[LIGHTNING_TOWER_SPRITE_ID] =
        new BuildingData(LIGHTNING_TOWER_SPRITE_ID, 504, 734, 135, 657,
                         {Vector2{135, 723}, Vector2{267, 654},
                          Vector2{135, 593}, Vector2{2, 661}},
                         149, 159);
}

BuildingData *getBuildingData(std::string building_id) {
    return buildingData[building_id];
}

Vector2 getWorldIsometricCoordinated(Vector2 position) {
    return Vector2{(position.x - position.y), (position.x + position.y) / 2};
}

Vector2 getReverseWorldIsometricCoordinated(Vector2 position) {
    return Vector2{(2 * position.y + position.x) / 2,
                   (2 * position.y - position.x) / 2};
}

Rectangle getBuildingTransparencyRect(BuildingData *buildingData, float x,
                                      float y, float perc_cover) {

    float rl = std::sqrt(1.25) * CASTLE_WIDTH * perc_cover;
    float scale = rl / buildingData->aligned_l;
    float x1 = buildingData->vertices[3].x * scale;
    float y1 = buildingData->h_y * scale;
    float x2 = buildingData->vertices[1].x * scale;
    float y2 =
        (buildingData->vertices[1].y + buildingData->vertices[3].y) * scale / 2;
    float w = x2 - x1;
    float h = y2 - y1;
    Vector2 o = {buildingData->o_x * scale, buildingData->o_y * scale};
    return {x1 - o.x + x, y1 - o.y + y, w, h};
}

std::vector<Vector2> getBuildingColliderPolygon(BuildingData *buildingData,
                                                float x, float y,
                                                float perc_cover,
                                                bool isSensor) {
    float rl = std::sqrt(1.25) * CASTLE_WIDTH * perc_cover;
    float scale = rl / buildingData->aligned_l;
    if (isSensor) {
        scale *= 1.2;
    }
    std::vector<Vector2> vertices;
    Vector2 o = {buildingData->o_x * scale, buildingData->o_y * scale};
    for (auto &v : buildingData->vertices) {
        Vector2 vf;
        vf.x = (v.x * scale - o.x);
        vf.y = (v.y * scale - o.y);
        vertices.push_back(vf);
    }
    return vertices;
}

Vector2 getBuildingUpgradePoint(BuildingData *buildingData, float x, float y,
                                float perc_cover) {

    float rl = std::sqrt(1.25) * CASTLE_WIDTH * perc_cover;
    float scale = rl / buildingData->aligned_l;
    Vector2 o = {buildingData->o_x * scale, buildingData->o_y * scale};
    Vector2 vfo = buildingData->vertices[2];
    return {(vfo.x * scale - o.x) + x, (vfo.y * scale - o.y) + y - 40};
}

Vector2 translateBuildingPoint(BuildingData *buildingData, float x, float y,
                               float perc_cover, Vector2 pp) {

    float rl = std::sqrt(1.25) * CASTLE_WIDTH * perc_cover;
    float scale = rl / buildingData->aligned_l;
    Vector2 o = {buildingData->o_x * scale, buildingData->o_y * scale};
    return {(pp.x * scale - o.x) + x, (pp.y * scale - o.y) + y};
}

float getMaxHealthByLevel(int level, PropertyType type) {
    if (level == 0) {
        return 0;
    }
    return maxHealthByLevel[type][level - 1];
}

int getUpgradeCoins(PropertyType type, int level) {
    return upgradeCoins[type][level - 1];
}

int getMaxLevel(PropertyType type) {
    return maxLevel[type];
}

void initLevelUpgradeData() {
    maxLevel[PropertyType::ARCHERY] = 3;
    maxLevel[PropertyType::BARRACKS] = 4;
    maxLevel[PropertyType::CASTLE] = 3;
    maxLevel[PropertyType::HOUSE] = 4;
    maxLevel[PropertyType::WIZARDRY] = 3;
    maxLevel[PropertyType::DEFENSE_TOWER] = 2;
    maxLevel[PropertyType::LIGHTNING_TOWER] = 1;

    upgradeCoins[PropertyType::ARCHERY] = {200, 500, 800};
    upgradeCoins[PropertyType::BARRACKS] = {100, 300, 500, 700};
    upgradeCoins[PropertyType::CASTLE] = {500, 1000, 1500, 4000};
    upgradeCoins[PropertyType::HOUSE] = {50, 70, 90, 150};
    upgradeCoins[PropertyType::WIZARDRY] = {300, 500, 900};
    upgradeCoins[PropertyType::DEFENSE_TOWER] = {100, 200};
    upgradeCoins[PropertyType::LIGHTNING_TOWER] = {200};

    maxHealthByLevel[PropertyType::ARCHERY] = {1000, 3000, 7000};
    maxHealthByLevel[PropertyType::BARRACKS] = {1000, 3000, 5000, 7000};
    maxHealthByLevel[PropertyType::CASTLE] = {10000, 20000, 40000, 70000};
    maxHealthByLevel[PropertyType::HOUSE] = {500, 700, 1000, 1500};
    maxHealthByLevel[PropertyType::WIZARDRY] = {2000, 7000, 10000};
    maxHealthByLevel[PropertyType::DEFENSE_TOWER] = {1000, 3000};
    maxHealthByLevel[PropertyType::LIGHTNING_TOWER] = {2000};

    percentCover[PropertyType::ARCHERY] = {0.5, 0.5, 0.5};
    percentCover[PropertyType::BARRACKS] = {0.5, 0.5, 0.6, 0.6};
    percentCover[PropertyType::CASTLE] = {0.5, 0.5, 0.6, 0.6};
    percentCover[PropertyType::HOUSE] = {0.4, 0.4, 0.5, 0.6};
    percentCover[PropertyType::WIZARDRY] = {0.3, 0.5, 0.5};
    percentCover[PropertyType::DEFENSE_TOWER] = {0.2, 0.3};
    percentCover[PropertyType::LIGHTNING_TOWER] = {0.10};

    buildingProduceTime[PropertyType::BARRACKS] = {60, 50, 40, 30};
    buildingProduceTime[PropertyType::ARCHERY] = {60, 50, 40};

    buildingSummonDim[PropertyType::BARRACKS] = {Vector2{4, 6}, Vector2{5, 7},
                                                 Vector2{6, 8}, Vector2{7, 9}};
    buildingSummonDim[PropertyType::ARCHERY] = {Vector2{10, 3}, Vector2{12, 5},
                                                Vector2{14, 7}};

    buildingSummonTypes[PropertyType::BARRACKS] = {
        {WarriorType::WARRIOR_TYPE_SPEARMAN,
         WarriorType::WARRIOR_TYPE_SHIELD_BEARER},
        {WarriorType::WARRIOR_TYPE_SPEARMAN,
         WarriorType::WARRIOR_TYPE_SHIELD_BEARER},
        {WarriorType::WARRIOR_TYPE_SPEARMAN,
         WarriorType::WARRIOR_TYPE_SHIELD_BEARER},
        {WarriorType::WARRIOR_TYPE_SPEARMAN,
         WarriorType::WARRIOR_TYPE_SHIELD_BEARER,
         WarriorType::WARRIOR_TYPE_SWORDSMAN},
    };
    buildingSummonTypes[PropertyType::ARCHERY] = {
        {WarriorType::WARRIOR_TYPE_ARCHER},
        {WarriorType::WARRIOR_TYPE_ARCHER},
        {WarriorType::WARRIOR_TYPE_ARCHER},
    };
}

std::string getBuildingId(PropertyType type, int level) {
    if (level == 0) {
        level = 1;
    }
    switch (type) {
    case PropertyType::ARCHERY: {
        return TextFormat("archery%d", level);
    }
    case PropertyType::BARRACKS: {
        return TextFormat("barracks%d", level);
    }
    case PropertyType::CASTLE: {
        return TextFormat("castle%d", level);
    }
    case PropertyType::HOUSE: {
        return TextFormat("house%d", level);
    }
    case PropertyType::WIZARDRY: {
        return TextFormat("wizardry%d", level);
    }
    case PropertyType::DEFENSE_TOWER: {
        return TextFormat("block_tower%d", level);
    }
    case PropertyType::LIGHTNING_TOWER: {
        return "lightning_tower";
    }
    default: {
        return "";
    }
    }
}

float getPercentCover(PropertyType type, int level) {
    if (level == 0) {
        level = 1;
    }
    return percentCover[type][level - 1];
}

Vector2 getBuildingSummonDim(PropertyType type, int level) {
    if (level == 0) {
        level = 1;
    }
    return buildingSummonDim[type][level - 1];
}

float getBuildingProduceTime(PropertyType type, int level) {
    if (level == 0) {
        level = 1;
    }
    return buildingProduceTime[type][level - 1];
}

std::vector<WarriorType> getSummonChoices(PropertyType type, int level) {
    if (level == 0) {
        level = 1;
    }
    return buildingSummonTypes[type][level - 1];
}

std::vector<Vector2> getAttackTowerArcherPos(int level) {
    switch (level) {
    case 1: {
        return {Vector2{0, -16}, Vector2{25, -33}, Vector2{0, -46},
                Vector2{-25, -33}};
    }
    case 2: {
        return {Vector2{0, -22},   Vector2{0, -34},  Vector2{0, -48},
                Vector2{0, -62},   Vector2{0, -76},  Vector2{-50, -48},
                Vector2{-25, -48}, Vector2{25, -48}, Vector2{50, -48},
                Vector2{-25, -62}, Vector2{25, -62}, Vector2{-25, -34},
                Vector2{25, -34}};
    }
    default: {
        return {Vector2{0, 0}};
    }
    }
}
