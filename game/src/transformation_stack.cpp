#include "transformation_stack.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <memory>
#include <vector>

std::vector<std::shared_ptr<Camera2D>> transformations;

void pushTransformation(std::shared_ptr<Camera2D> transformation) {
    transformations.push_back(transformation);
    beginTransformationStack();
}

void popTransformation() {
    transformations.pop_back();
    if (transformations.size() == 0)
        endTransformation();
    else
        beginTransformationStack();
}

void beginTransformationStack() {
    rlDrawRenderBatchActive();
    rlLoadIdentity();

    for (auto it = transformations.begin(); it != transformations.end(); ++it) {
        std::shared_ptr<Camera2D> cam = *it;

        rlMultMatrixf(MatrixToFloat(GetCameraMatrix2D(*cam)));
    }
}

void endTransformation() {
    EndMode2D();

}

Vector2 toWorldCoords(Camera2D cam, float xx, float yy) {
    float c = std::cos(-cam.rotation * DEG2RAD);
    float s = std::sin(-cam.rotation * DEG2RAD);
    xx = (xx-cam.offset.x)/cam.zoom;
    yy = (yy-cam.offset.y)/cam.zoom;
    
    float xi = c*xx - s*yy;
    float yi = s*xx + c * yy;

    return (Vector2) {xi + cam.target.x, yi + cam.target.y};
}
Vector2 toCameraCoords(Camera2D cam, float xx, float yy)
{
    float c = std::cos(cam.rotation * DEG2RAD);
    float s = std::sin(cam.rotation * DEG2RAD);
    xx = xx - cam.target.x;
    yy = yy - cam.target.y;
    float xi = c*xx - s*yy;
    float yi = s*xx + c*yy;

    return (Vector2) {xi*cam.zoom + cam.offset.x, yi*cam.zoom + cam.offset.y};

}

Vector2 toWorldCoords(float xx, float yy) {
    Camera2D cam = *transformations[0];
    float c = std::cos(-cam.rotation * DEG2RAD);
    float s = std::sin(-cam.rotation * DEG2RAD);
    xx = (xx-cam.offset.x)/cam.zoom;
    yy = (yy-cam.offset.y)/cam.zoom;
    
    float xi = c*xx - s*yy;
    float yi = s*xx + c * yy;

    return (Vector2) {xi + cam.target.x, yi + cam.target.y};
}

Vector2 toCameraCoords(float xx, float yy)
{
    Camera2D cam = *transformations[0];
    float c = std::cos(cam.rotation * DEG2RAD);
    float s = std::sin(cam.rotation * DEG2RAD);
    xx = xx - cam.target.x;
    yy = yy - cam.target.y;
    float xi = c*xx - s*yy;
    float yi = s*xx + c*yy;

    return (Vector2) {xi*cam.zoom + cam.offset.x, yi*cam.zoom + cam.offset.y};

}
