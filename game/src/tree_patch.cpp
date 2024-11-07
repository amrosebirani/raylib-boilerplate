#include "tree_patch.hpp"
#include "collider_factory.hpp"
#include "constants.h"
#include "globals.h"
#include "raylib.h"
#include "utils.h"
#include "RandomGenerator.h"

RandomGenerator rng1;

void drawTreeType(TreeType type, float x, float y) {
    std::string sprite_id;
    switch (type) {
    case TreeType::TREE1:
        sprite_id = TREE1_SPRITE_ID;
        break;
    case TreeType::TREE2:
        sprite_id = TREE2_SPRITE_ID;
        break;
    case TreeType::TREE3:
        sprite_id = TREE3_SPRITE_ID;
        break;
    case TreeType::TREE4:
        sprite_id = TREE4_SPRITE_ID;
        break;
    case TreeType::TREE5:
        sprite_id = TREE5_SPRITE_ID;
        break;
    case TreeType::TREE6:
        sprite_id = TREE6_SPRITE_ID;
        break;
    case TreeType::TREE7:
        sprite_id = TREE7_SPRITE_ID;
        break;
    }
    Vector2 ss = getSpriteHolder()->getSpriteSize(sprite_id);
    float width = TREE_HEIGHT * ss.x / ss.y;
    getSpriteHolder()->drawSprite(
        sprite_id, {x - width / 2, y - TREE_HEIGHT / 2, width, TREE_HEIGHT});
    // DrawRectangleLines(x - width / 2, y - TREE_HEIGHT / 2, width,
    // TREE_HEIGHT, YELLOW);
}

TreePatch::~TreePatch() {
    UnloadRenderTexture(tree_texture);
    delete collider_data;
    collider = nullptr;
    trees.clear();
}

TreePatch::TreePatch(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {
    tree_texture = LoadRenderTexture(width, height);
    collider_data = new ColliderUserData();
    collider_data->type = ColliderUserData::Type::TreePatch;
    collider = ColliderFactory::newCircleCollider(
        collider_data, x, y, 20, b2_staticBody, CATEGORY_ENVIRONMENT,
        CATEGORY_FORMATION, getContainer()->getWorld());
}

void TreePatch::draw() {
    DrawTextureRec(tree_texture.texture,
                   {0, 0, (float)tree_texture.texture.width,
                    (float)-tree_texture.texture.height},
                   {x - width / 2, y - height / 2}, WHITE);
}

void TreePatch::update() {
    // update the tree patch
    // collider->SetAwake(true);
}

Vector2 getRandomLocation(float width, float height) {
    float x = (float)rng1.random(0, 100);
    float xx, yy;
    if (x < 25) {
        xx = (float)rng1.randomNormal((width / 2 - TREE_HEIGHT) / 2.6f);
        yy = (float)rng1.randomNormal((height / 2 - TREE_HEIGHT) / 2.4f);
    } else if (x < 56) {
        xx = (float)rng1.random(-(width / 2 - TREE_HEIGHT),
                                width / 2 - TREE_HEIGHT);
        yy = (float)rng1.randomNormal((height / 2 - TREE_HEIGHT) / 2.4f);
    } else if (x < 75) {
        xx = (float)rng1.randomNormal((width / 2 - TREE_HEIGHT) / 2.6f);
        yy = (float)rng1.random(-(height / 2 - TREE_HEIGHT),
                                height / 2 - TREE_HEIGHT);
    } else {

        xx = (float)rng1.random(-(width / 2 - TREE_HEIGHT),
                                width / 2 - TREE_HEIGHT);
        yy = (float)rng1.random(-(height / 2 - TREE_HEIGHT),
                                height / 2 - TREE_HEIGHT);
    }
    return {xx, yy};
}

void TreePatch::initialize(int n) {
    // initialize the tree patch
    // create a collider
    // create a tree
    for (int i = 0; i < n; i++) {
        // get random tree type
        // get random tree type
        TreeType type = (TreeType)getRandomIntInRange(0, 6);
        // get random xx using a normal distribution
        Vector2 loc = getRandomLocation(width, height);
        float xx = loc.x;
        float yy = loc.y;
        // create a tree
        std::shared_ptr<Tree> tree =
            std::make_shared<Tree>(width / 2 + xx, height / 2 + yy, type);
        trees.push_back(tree);
    }
    // sort the trees on y position increasing
    std::sort(trees.begin(), trees.end(),
              [](std::shared_ptr<Tree> a, std::shared_ptr<Tree> b) {
                  return a->y < b->y;
              });
    BeginTextureMode(tree_texture);
    ClearBackground(BLANK);
    for (int i = 0; i < n; i++) {
        trees[i]->draw();
    }
    // DrawRectangle(10, 10, width - 20, height - 20,
    // {PINK.r, PINK.g, PINK.b, 200});
    EndTextureMode();
    // so the texture is ready now,
    // just need to add the collider fixtures now for all the trees
}
