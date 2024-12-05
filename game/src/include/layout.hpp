#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include "constants.h"

struct PanelDef {
        float x;
        float y;
        float width;
        float height;
        std::function<void()> clickAction;
};

class Layout {
    public:
        void draw(Color color = default_color);
        void update(float dt);
        Layout();
        void contract(std::string name, float horz, float vert);
        void splitHorz(std::string name, std::string tname, std::string bname,
                       float ratio, float splitSize);
        void splitVert(std::string name, std::string lname, std::string rname,
                       float ratio, float splitSize);
        float top(std::string name);
        float left(std::string name);
        float bottom(std::string name);
        float right(std::string name);
        float midx(std::string name);
        float midy(std::string name);
        void setClickAction(std::string name, std::function<void()> action);

    private:
        std::unordered_map<std::string, std::shared_ptr<PanelDef>> mPanels;
};
