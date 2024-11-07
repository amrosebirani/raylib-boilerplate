#include "horde_cine_exp.hpp"
#include "cine_experience.hpp"
#include "game_object.h"
#include "globals.h"
#include <unordered_map>

HordeCineExperience::HordeCineExperience(std::shared_ptr<GameObject> hordeLead)
    : hordeLead(hordeLead), CineExperience() {
    timer = Timer();
    params = std::make_shared<std::unordered_map<std::string, float>>();
}

void HordeCineExperience::update(float dt) {
    timer.update(dt);

    if (active) {
        if (getViewCamera()->getTarget() == nullptr) {
            getViewCamera()->moveTo((*params)["x"], (*params)["y"]);
        }
        getViewCamera()->setScale(original_scale + (*params)["sc"]);
    }
}

void HordeCineExperience::startExp() {
    active = true;
    original_scale = getViewCamera()->getScale();

    getViewCamera()->follow(nullptr);

    Vector2 formPos = getContainer()->getFormPos();
    (*params)["x"] = formPos.x;
    (*params)["y"] = formPos.y;
    (*params)["sc"] = 0;

    timer.tween(1.5, params,
                {{"x", hordeLead->x}, {"y", hordeLead->y}, {"sc", 2}},
                "in-out-cubic", []() {}, "", {});

    timer.after(
        1.52, [this](float dt) { getViewCamera()->follow(this->hordeLead); },
        "");

    timer.after(
        4.5,
        [this](float dt) {
            getViewCamera()->follow(nullptr);
            Vector2 fp = getContainer()->getFormPos();
            (*this->params)["x"] = this->hordeLead->x;
            (*this->params)["y"] = this->hordeLead->y;
            (*this->params)["sc"] = 2;

            this->timer.tween(1.5, this->params,
                              {{"x", fp.x}, {"y", fp.y}, {"sc", 0}},
                              "in-out-cubic", []() {}, "", {});
        },
        "");

    timer.after(
        6.6,
        [this](float dt) {
            this->active = false;
            getViewCamera()->follow(getContainer()->getFollowObject());
            getViewCamera()->setScale(this->original_scale);
        },
        "");
}
