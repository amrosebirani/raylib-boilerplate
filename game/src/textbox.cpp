#include "textbox.hpp"
#include "globals.h"
#include "raylib.h"
#include <cstring>
#include <iostream>

TextBox::TextBox(std::string text, std::string bust_sprite, bool blocking,
                 float fontSize, float spacing, bool shifted, float shifted_by,
                 float timev) {
    this->text = text;
    this->bust_sprite = bust_sprite;
    this->blocking = blocking;
    this->fontSize = fontSize;
    this->spacing = spacing;
    this->shifted = shifted;
    this->shifted_by = shifted_by;
    // determine the height needed for the text box
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();

    if (sw > sh) {
        width = sw / 3;
    } else {
        width = 2 * sw / 3;
    }
    float avWidth = width - 45;
    iWidth = avWidth * .25f;
    tWidth = avWidth * .75f;
    std::pair<float, float> mm =
        MeasureTextWrapped(text.c_str(), {0, 0, tWidth, 0}, GetFontDefault());
    if (mm.first < tWidth) {
        tWidth = mm.first;
        width = iWidth + tWidth + 45;
    }
    while (mm.second > sh - 90) {
        std::cout << "Reducing font size" << std::endl;
        std::cout << fontSize << std::endl;
        std::cout << mm.second << std::endl;
        std::cout << sh << std::endl;

        this->fontSize = this->fontSize * .8f;
        mm = MeasureTextWrapped(text.c_str(), {0, 0, tWidth, 0},
                                GetFontDefault());
    }
    iHeight = iWidth;
    tHeight = mm.second;
    float fHeight = iHeight > tHeight ? iHeight : tHeight;

    height = fHeight + 30 + 60;
    float panel_x = sw / 2;
    float panel_y = sh / 2;
    if (shifted) {
        if (sw > sh) {
            panel_x += shifted_by;
        } else {
            panel_y += shifted_by;
        }
    }
    panel = new Panel(panel_x, panel_y, width, height, {0, 0, 0, 200});
    if (!blocking) {
        timer.after(timev, [this](float dt) { this->finish(); }, "");
    }
    renderTexture = LoadRenderTexture(tWidth, tHeight);
    DrawTextWrapped(text.c_str(), WHITE, GetFontDefault());
    getWorldState()->setPopupActive(true);
    float mme = MeasureText("Exit", 20);
    exitRect = {panel->left + panel->width / 2 - mme / 2 - 20,
                panel->top + panel->height - 70, mme + 40, 40};
}

TextBox::~TextBox() {
    if (panel != nullptr) {
        delete panel;
    }
}

void TextBox::finish() {
    finished = true;
    getWorldState()->setPopupActive(false);
    getViewCamera()->follow(getContainer()->getFollowObject());
}

std::pair<float, float> TextBox::MeasureTextWrapped(const char *text,
                                                    Rectangle rec, Font font) {
    float ww = rec.width;
    float totalWidth = MeasureTextEx(font, text, fontSize, spacing).x;
    if (totalWidth <= ww) {
        return std::make_pair(totalWidth, fontSize);
    }
    float hh = 0;
    float lineSpacing = font.baseSize * fontSize / font.baseSize * 1.5f;
    char *textCopy = (char *)malloc(strlen(text) + 1);
    strcpy(textCopy, text);

    char *line = (char *)malloc(strlen(text) + 1);
    line[0] = '\0';

    char *word = strtok(textCopy, " ");

    while (word != NULL) {
        char *testLine = (char *)malloc(strlen(line) + strlen(word) + 2);
        strcpy(testLine, line);

        if (strlen(line) > 0) {
            strcat(testLine, " ");
        }

        strcat(testLine, word);

        Vector2 size = MeasureTextEx(font, testLine, fontSize, spacing);
        if (size.x > rec.width) {
            strcpy(line, word);
            hh += lineSpacing;
        } else {
            strcpy(line, testLine);
        }

        free(testLine);
        word = strtok(NULL, " ");
    }

    if (strlen(line) > 0) {
        hh += lineSpacing;
    }

    return std::make_pair(ww, hh);
}

bool TextBox::update(float dt) {
    timer.update(dt);
    panel->update(dt);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, exitRect)) {
            exitPressed = true;
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, exitRect) && exitPressed) {
            finish();
        }
        exitPressed = false;
    }
    return !blocking;
}

bool TextBox::isFinished() {
    return finished;
}

void TextBox::draw() {
    if (blocking)
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                      {0, 0, 0, 100});
    panel->draw();
    float mme = MeasureText("Exit", 20);
    getSpriteHolder()->drawSprite(BUTTON_ICON, exitRect);
    DrawText("Exit", panel->left + panel->width / 2 - mme / 2,
             panel->top + panel->height - 60, 20, BLACK);
    Rectangle iRect = {panel->left + 15, panel->top + 15, iWidth, iHeight};
    getSpriteHolder()->drawSprite(bust_sprite, iRect);
    getSpriteHolder()->drawSprite(CHARACTER_FRAME_SPRITE_ID, iRect);
    Rectangle tRect = {iRect.x + iRect.width + 15, iRect.y, tWidth, tHeight};
    DrawTexturePro(renderTexture.texture, {0, 0, tWidth, -tHeight}, tRect,
                   {0, 0}, 0, WHITE);
}

void TextBox::DrawTextWrapped(const char *text, Color tint, Font font) {
    BeginTextureMode(renderTexture);
    ClearBackground(BLANK);
    float hh = 0;
    float lineSpacing = font.baseSize * fontSize / font.baseSize * 1.5f;
    char *textCopy = (char *)malloc(strlen(text) + 1);
    strcpy(textCopy, text);

    char *line = (char *)malloc(strlen(text) + 1);
    line[0] = '\0';

    char *word = strtok(textCopy, " ");

    while (word != NULL) {
        char *testLine = (char *)malloc(strlen(line) + strlen(word) + 2);
        strcpy(testLine, line);

        if (strlen(line) > 0) {
            strcat(testLine, " ");
        }

        strcat(testLine, word);

        Vector2 size = MeasureTextEx(font, testLine, fontSize, spacing);
        if (size.x > tWidth) {
            Vector2 position = {0, hh};
            DrawTextEx(font, line, position, fontSize, spacing, tint);
            strcpy(line, word);
            hh += lineSpacing;
        } else {
            strcpy(line, testLine);
        }

        free(testLine);
        word = strtok(NULL, " ");
    }

    if (strlen(line) > 0) {
        Vector2 position = {0, hh};
        DrawTextEx(font, line, position, fontSize, spacing, tint);
        hh += lineSpacing;
    }
    EndTextureMode();
}
