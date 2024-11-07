#pragma once

#include "box2d/b2_body.h"
#include "collider_user_data.h"
#include "raylib.h"
#include <memory>
#include <vector>

enum class TreeType {
    TREE1,
    TREE2,
    TREE3,
    TREE4,
    TREE5,
    TREE6,
    TREE7,
};

void drawTreeType(TreeType type, float x, float y);

struct Tree {
        float x, y;
        TreeType type;
        Tree(float x, float y, TreeType type) : x(x), y(y), type(type) {
        }

        void draw() {
            drawTreeType(type, x, y);
        }
};

class TreePatch {
    public:
        TreePatch(float x, float y, float width, float height);
        ~TreePatch();

        void draw();
        //  ideally I should draw and render the trees in a texture, and then
        //  just draw the texture on all the draw calls
        void update();
        void initialize(int n);

    private:
        float x, y, width, height;
        std::shared_ptr<b2Body> collider;
        std::vector<std::shared_ptr<Tree>> trees;
        RenderTexture2D tree_texture;
        ColliderUserData *collider_data;
};

// what should be the distribution of the trees around the center
// using normal distribution with different variances across x and y
