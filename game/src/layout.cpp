#include "layout.hpp"
#include "constants.h"
#include <memory>
#include "panel.hpp"

Layout::Layout() {
    std::shared_ptr<PanelDef> screen = std::make_shared<PanelDef>();
    screen->x = 0;
    screen->y = 0;
    screen->width = VIRTUAL_WIDTH;
    screen->height = VIRTUAL_HEIGHT;
    mPanels["screen"] = screen;
}

void Layout::draw(Color color) {
    for (auto &panel : mPanels) {
        Panel p = Panel(panel.second->x, panel.second->y, panel.second->width,
                        panel.second->height, color);
        p.draw();
    }
}

void Layout::update(float dt) {
}

void Layout::contract(std::string name, float horz, float vert) {
    mPanels[name]->width -= horz;
    mPanels[name]->height -= vert;
}

void Layout::splitHorz(std::string name, std::string tname, std::string bname,
                       float ratio, float splitSize) {
    float p1Height = mPanels[name]->height * ratio;
    float p2Height = mPanels[name]->height - p1Height;
    std::shared_ptr<PanelDef> tPanel = std::make_shared<PanelDef>();
    tPanel->x = mPanels[name]->x;
    tPanel->y = mPanels[name]->y - mPanels[name]->height / 2 + p1Height / 2 -
                splitSize / 2;
    tPanel->width = mPanels[name]->width;
    tPanel->height = p1Height - splitSize;
    mPanels[tname] = tPanel;
    std::shared_ptr<PanelDef> bPanel = std::make_shared<PanelDef>();
    bPanel->x = mPanels[name]->x;
    bPanel->y = mPanels[name]->y + mPanels[name]->height / 2 - p2Height / 2 +
                splitSize / 2;
    bPanel->width = mPanels[name]->width;
    bPanel->height = p2Height - splitSize;
    mPanels[bname] = bPanel;

    // remove the original panel
    mPanels.erase(name);
}

void Layout::splitVert(std::string name, std::string lname, std::string rname,
                       float ratio, float splitSize) {
    float p1Width = mPanels[name]->width * ratio;
    float p2Width = mPanels[name]->width - p1Width;
    std::shared_ptr<PanelDef> lPanel = std::make_shared<PanelDef>();
    lPanel->x = mPanels[name]->x - mPanels[name]->width / 2 + p1Width / 2 -
                splitSize / 2;
    lPanel->y = mPanels[name]->y;
    lPanel->width = p1Width - splitSize;
    lPanel->height = mPanels[name]->height;
    mPanels[lname] = lPanel;
    std::shared_ptr<PanelDef> rPanel = std::make_shared<PanelDef>();
    rPanel->x = mPanels[name]->x + mPanels[name]->width / 2 - p2Width / 2 +
                splitSize / 2;
    rPanel->y = mPanels[name]->y;
    rPanel->width = p2Width - splitSize;
    rPanel->height = mPanels[name]->height;
    mPanels[rname] = rPanel;

    // remove the original panel
    mPanels.erase(name);
}

float Layout::top(std::string name) {
    return mPanels[name]->y - mPanels[name]->height / 2;
}

float Layout::left(std::string name) {
    return mPanels[name]->x - mPanels[name]->width / 2;
}

float Layout::bottom(std::string name) {
    return mPanels[name]->y + mPanels[name]->height / 2;
}

float Layout::right(std::string name) {
    return mPanels[name]->x + mPanels[name]->width / 2;
}

float Layout::midx(std::string name) {
    return mPanels[name]->x;
}

float Layout::midy(std::string name) {
    return mPanels[name]->y;
}

void Layout::setClickAction(std::string name, std::function<void()> action) {
    mPanels[name]->clickAction = action;
}
