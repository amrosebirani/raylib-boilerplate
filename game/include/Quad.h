#pragma once
#include "raylib.h"

class Quad {
    public:
        struct Viewport {
                double x, y;
                double w, h;
        };

        Quad(const Viewport &v, double sw, double sh);

        virtual ~Quad();

        void refresh(const Viewport &v, double sw, double sh);
        void setViewport(const Viewport &v);
        Viewport getViewport() const;

        double getTextureWidth() const;
        double getTextureHeight() const;

        const Vector2 *getVertexPositions() {
            return vertexPositions;
        }
        const Vector2 *getVertexTexCoords() {
            return vertexTexCoords;
        }

        void setLayer(int layer);
        int getLayer() const;

    private:
        Vector2 vertexPositions[4];
        Vector2 vertexTexCoords[4];

        int arrayLayer;

        Viewport viewport;
        double sw;
        double sh;
};
