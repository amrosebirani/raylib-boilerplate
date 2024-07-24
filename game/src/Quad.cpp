#include "Quad.h"

#include <cstring>

Quad::Quad(const Quad::Viewport &v, double sw, double sh) : sw(sw), sh(sh) {
    arrayLayer = 0;
    refresh(v, sw, sh);
}

Quad::~Quad() {
}

void Quad::refresh(const Quad::Viewport &v, double sw, double sh) {
    viewport = v;
    this->sw = sw;
    this->sh = sh;

    vertexPositions[0] = {0.0f, 0.0f};
    vertexPositions[1] = {0.0f, (float)v.h};
    vertexPositions[2] = {(float)v.w, 0.0f};
    vertexPositions[3] = {(float)v.w, (float)v.h};

    vertexTexCoords[0] = {(float)(v.x / sw), (float)(v.y / sh)};
    vertexTexCoords[1] = {(float)(v.x / sw), (float)((v.y + v.h) / sh)};
    vertexTexCoords[2] = {(float)((v.x + v.w) / sw), (float)(v.y / sh)};
    vertexTexCoords[3] = {(float)((v.x + v.w) / sw), (float)((v.y + v.h) / sh)};
}

void Quad::setViewport(const Quad::Viewport &v) {
    refresh(v, sw, sh);
}

Quad::Viewport Quad::getViewport() const {
    return viewport;
}

double Quad::getTextureWidth() const {
    return sw;
}

double Quad::getTextureHeight() const {
    return sh;
}

void Quad::setLayer(int layer) {
    arrayLayer = layer;
}

int Quad::getLayer() const {
    return arrayLayer;
}
