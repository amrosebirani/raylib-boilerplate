#pragma once
#include "panel.hpp"
#include "raylib.h"
#include "room.hpp"
#include "timer.h"
#include <string>

class TextBox : public Room {
    public:
        TextBox(std::string text, std::string bust_sprite, bool blocking,
                float fontSize, float spacing, bool shifted = false,
                float shifted_by = 0, float timev = 4);
        ~TextBox();

        void draw() override;
        bool update(float dt) override;
        bool isFinished() override;
        void DrawTextWrapped(const char *, Color tint, Font font);
        std::pair<float, float> MeasureTextWrapped(const char *text,
                                                   Rectangle rec, Font font);

    private:
        Timer timer;
        std::string text;
        std::string bust_sprite;
        bool blocking;
        bool finished = false;
        Panel *panel = nullptr;
        float width;
        float height;
        float iHeight;
        float iWidth;
        float tHeight;
        float tWidth;
        RenderTexture2D renderTexture;
        float fontSize;
        float spacing;
        bool shifted;
        float shifted_by;
        void finish();
        Rectangle exitRect;
        bool exitPressed = false;
};
