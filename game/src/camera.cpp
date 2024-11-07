#include "camera.h"
#include "raylib.h"
#include "game_object.h"
#include <cmath>
#include <memory>
#include <transformation_stack.h>

// Helper functions
float lerp(float a, float b, float x) {
    return a + (b - a) * x;
}

float csnap(float v, float x) {
    return std::ceil(v / x) * x - x / 2;
}

void CameraEnhanced::setScale(float scale) {
    this->scale = scale;
    if (scale < 0.1f) {
        this->scale = 0.1f;
    }
}

float CameraEnhanced::getScale() {
    return scale;
}

void CameraEnhanced::editScale(float change) {
    scale += change;
    if (scale < 0.1f) {
        scale = 0.1f;
    }
    if (scale > 8.0f) {
        scale = 8.0f;
    }
}

void CameraEnhanced::attach(float vw, float vh) {
    if (adjustScreenScale) {
        w = GetScreenWidth();
        h = GetScreenHeight();
        float scaleX = (float)GetScreenWidth() / vw;
        float scaleY = (float)GetScreenHeight() / vh;
        float newScale = (scaleX < scaleY) ? scaleX : scaleY;
        scale = newScale;
    }
    camera->rotation = rotation;
    camera->zoom = scale;
    camera->target = (Vector2){x, y};
    camera->offset = (Vector2){w / 2, h / 2};
    // BeginMode2D(camera);
    pushTransformation(camera);
}

void CameraEnhanced::detach() {
    // EndMode2D();
    popTransformation();
}

void CameraEnhanced::move(float dx, float dy) {
    x += dx;
    y += dy;
}

void CameraEnhanced::moveTo(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2 CameraEnhanced::toWorldCoords(float xx, float yy) {
    float c = std::cos(-rotation * DEG2RAD);
    float s = std::sin(-rotation * DEG2RAD);
    xx = (xx - w / 2.0f) / scale;
    yy = (yy - h / 2.0f) / scale;

    float xi = c * xx - s * yy;
    float yi = s * xx + c * yy;

    return (Vector2){xi + x, yi + y};
}

bool CameraEnhanced::isRectangleTouched(Rectangle rect) {
    int touchCount = GetTouchPointCount();
    for (int i = 0; i < touchCount; i++) {
        Vector2 touch = GetTouchPosition(i);
        Vector2 wc = toWorldCoords(touch.x, touch.y);
        if (CheckCollisionPointRec(wc, rect)) {
            return true;
        }
    }
    return false;
}

Vector2 CameraEnhanced::toCameraCoords(float xx, float yy) {
    float c = std::cos(rotation * DEG2RAD);
    float s = std::sin(rotation * DEG2RAD);
    xx = xx - x;
    yy = yy - y;
    float xi = c * xx - s * yy;
    float yi = s * xx + c * yy;

    return (Vector2){xi * scale + w / 2.0f, yi * scale + h / 2.0f};
}

Vector2 CameraEnhanced::getMousePosition() {
    Vector2 mp = GetMousePosition();

    return toWorldCoords(mp.x, mp.y);
}

void CameraEnhanced::shake(float intensity, float duration, int frequency) {
    horizontal_shakes.push_back(
        std::make_shared<Shake>(intensity, duration, frequency));
    vertical_shakes.push_back(
        std::make_shared<Shake>(intensity, duration, frequency));
}

void CameraEnhanced::tweenZoom(float change) {
    float target_scale = scale + change;
    if (target_scale < 0.1f) target_scale = 0.1f;
    if (target_scale > 4.0f) target_scale = 4.0f;

    (*params)["scale"] = scale;
    timer.tween(0.2, params, {{"scale", target_scale}}, "in-out-cubic",
                [this]() {
                    // remove scale from params if there
                    if (this->params->find("scale") != this->params->end())
                        this->params->erase("scale");
                },
                "zoom", {});
}

void CameraEnhanced::update(float dt) {
    timer.update(dt);
    // check if there is scale in params, if there then update scale
    if (params->find("scale") != params->end()) {
        scale = (*params)["scale"];
    }
    Vector2 mm = getMousePosition();
    mx = mm.x;
    my = mm.y;

    if (flashing) {
        flash_timer += dt;
        if (flash_timer > flash_duration) {
            flash_timer = 0;
            flashing = false;
        }
    }

    if (fading) {
        fade_timer += dt;
        fade_color = {
            static_cast<unsigned char>(lerp(base_fade_color.r,
                                            target_fade_color.r,
                                            fade_timer / fade_duration)),
            static_cast<unsigned char>(lerp(base_fade_color.g,
                                            target_fade_color.g,
                                            fade_timer / fade_duration)),
            static_cast<unsigned char>(lerp(base_fade_color.b,
                                            target_fade_color.b,
                                            fade_timer / fade_duration)),
            static_cast<unsigned char>(lerp(base_fade_color.a,
                                            target_fade_color.a,
                                            fade_timer / fade_duration)),
        };
        if (fade_timer > fade_duration) {
            fade_timer = 0;
            fading = false;
            fade_action();
        }
    }

    float horizontal_shake_amount = 0;
    float vertical_shake_amount = 0;

    for (auto it = horizontal_shakes.begin(); it != horizontal_shakes.end();) {
        std::shared_ptr<Shake> shake = *it;
        shake->update(dt);
        horizontal_shake_amount += shake->getAmplitude(shake->t);
        if (!shake->shaking) {
            it = horizontal_shakes.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = vertical_shakes.begin(); it != vertical_shakes.end();) {
        std::shared_ptr<Shake> shake = *it;
        shake->update(dt);
        vertical_shake_amount += shake->getAmplitude(shake->t);
        if (!shake->shaking) {
            it = vertical_shakes.erase(it);
        } else {
            ++it;
        }
    }

    x -= last_horizontal_shake_amount;
    y -= last_vertical_shake_amount;

    move(horizontal_shake_amount, vertical_shake_amount);
    last_vertical_shake_amount = vertical_shake_amount;
    last_horizontal_shake_amount = horizontal_shake_amount;

    // Follow //

    if (target == nullptr) {
        return;
    }

    if (!target->isAlive()) {
        target = nullptr;
        return;
    }

    if (followStyle == FollowStyle::LOCKON) {
        float ww = w / 16;
        float hh = h / 16;
        setDeadzone((w - ww) / 2, (h - hh) / 2, ww, hh);
    } else if (followStyle == FollowStyle::PLATFORMER) {
        float ww = w / 8;
        float hh = h / 3;
        setDeadzone((w - ww) / 2, (h - hh) / 2 - hh * 0.25, ww, hh);
    } else if (followStyle == FollowStyle::TOPDOWN) {
        float s = std::max(w, h) / 4;
        setDeadzone((w - s) / 2, (h - s) / 2, s, s);
    } else if (followStyle == FollowStyle::TOPDOWN_TIGHT) {
        float s = std::max(w, h) / 8;
        setDeadzone((w - s) / 2, (h - s) / 2, s, s);
    } else if (followStyle == FollowStyle::SCREEN_BY_SCREEN) {
        setDeadzone(0, 0, 0, 0);
    } else if (followStyle == FollowStyle::NO_DEADZONE) {
        deadzone = false;
    }

    if (!deadzone) {
        x = target->x;
        y = target->y;

        if (bound) {
            x = std::min(std::max(x, bounds_min_x), bounds_max_x);
            y = std::min(std::max(y, bounds_min_y), bounds_max_y);
        }
        return;
    }

    float dx1 = deadzone_x;
    float dy1 = deadzone_y;
    float dx2 = deadzone_x + deadzone_w;
    float dy2 = deadzone_y + deadzone_h;

    float scroll_x = 0;
    float scroll_y = 0;

    Vector2 localTarget = toCameraCoords(target->x, target->y);

    Vector2 cc = toCameraCoords(x, y);

    if (followStyle == FollowStyle::SCREEN_BY_SCREEN) {
        if (bound) {
            if (x > bounds_min_x && localTarget.x < 0)
                screen_x = csnap(screen_x - w / scale, w / scale);
            if (x < bounds_max_x && localTarget.x >= w)
                screen_x = csnap(screen_x + w / scale, w / scale);

            if (y > bounds_min_y && localTarget.y < 0)
                screen_y = csnap(screen_y - h / scale, h / scale);
            if (y < bounds_max_y && localTarget.y >= h)
                screen_y = csnap(screen_y + h / scale, h / scale);
        } else {
            if (localTarget.x < 0)
                screen_x = csnap(screen_x - w / scale, w / scale);
            if (localTarget.x >= w)
                screen_x = csnap(screen_x + w / scale, w / scale);
            if (localTarget.y < 0)
                screen_y = csnap(screen_y - h / scale, h / scale);
            if (localTarget.y >= h)
                screen_y = csnap(screen_y + h / scale, h / scale);
        }

        x = lerp(x, screen_x, follow_lerp_x);
        y = lerp(y, screen_y, follow_lerp_y);

        if (bound) {
            x = std::min(std::max(x, bounds_min_x), bounds_max_x);
            y = std::min(std::max(y, bounds_min_y), bounds_max_y);
        }
    } else {
        if (localTarget.x < cc.x + (dx1 + dx2 - cc.x)) {
            float d = localTarget.x - dx1;
            if (d < 0) scroll_x = d;
        }
        if (localTarget.x > cc.x - (dx1 + dx2 - cc.x)) {
            float d = localTarget.x - dx2;
            if (d > 0) scroll_x = d;
        }

        if (localTarget.y < cc.y + (dy1 + dy2 - cc.y)) {
            float d = localTarget.y - dy1;
            if (d < 0) scroll_y = d;
        }
        if (localTarget.y > cc.y - (dy1 + dy2 - cc.y)) {
            float d = localTarget.y - dy2;
            if (d > 0) scroll_y = d;
        }

        if (last_target == nullptr) {
            last_target = std::make_shared<Vector2>();
            last_target->x = target->x;
            last_target->y = target->y;
        }
        scroll_x =
            scroll_x + (target->x - last_target->x) * scale * follow_lead_x;
        scroll_y =
            scroll_y + (target->y - last_target->y) * scale * follow_lead_y;
        last_target->x = target->x;
        last_target->y = target->y;

        x = lerp(x, x + scroll_x / scale, follow_lerp_x);
        y = lerp(y, y + scroll_y / scale, follow_lerp_y);

        if (bound) {
            x = std::min(std::max(x, bounds_min_x), bounds_max_x);
            y = std::min(std::max(y, bounds_min_y), bounds_max_y);
        }
    }
}

std::shared_ptr<GameObject> CameraEnhanced::getTarget() {
    return target;
}

void CameraEnhanced::drawWorldLine(float x1, float y1, float x2, float y2,
                                   Color color) {
    Vector2 wc1 = toWorldCoords(x1, y1);
    Vector2 wc2 = toWorldCoords(x2, y2);

    DrawLine(wc1.x, wc1.y, wc2.x, wc2.y, color);
}

void CameraEnhanced::draw() {
    if (draw_deadzone && deadzone) {
        drawWorldLine(deadzone_x - 1, deadzone_y, deadzone_x + 6, deadzone_y,
                      WHITE);
        drawWorldLine(deadzone_x, deadzone_y, deadzone_x, deadzone_y + 6,
                      WHITE);
        drawWorldLine(deadzone_x - 1, deadzone_y + deadzone_h, deadzone_x + 6,
                      deadzone_y + deadzone_h, WHITE);
        drawWorldLine(deadzone_x, deadzone_y + deadzone_h, deadzone_x,
                      deadzone_y + deadzone_h - 6, WHITE);
        drawWorldLine(deadzone_x + deadzone_w + 1, deadzone_y + deadzone_h,
                      deadzone_x + deadzone_w - 6, deadzone_y + deadzone_h,
                      WHITE);
        drawWorldLine(deadzone_x + deadzone_w, deadzone_y + deadzone_h,
                      deadzone_x + deadzone_w, deadzone_y + deadzone_h - 6,
                      WHITE);
        drawWorldLine(deadzone_x + deadzone_w + 1, deadzone_y,
                      deadzone_x + deadzone_w - 6, deadzone_y, WHITE);
        drawWorldLine(deadzone_x + deadzone_w, deadzone_y,
                      deadzone_x + deadzone_w, deadzone_y + 6, WHITE);
    }

    Vector2 origin = toWorldCoords(0, 0);
    Vector2 rightbottom = toWorldCoords(w, h);
    if (flashing) {
        DrawRectangle(origin.x, origin.y, w / scale, h / scale, flash_color);
    }

    DrawRectangle(origin.x, origin.y, w / scale, h / scale, fade_color);
    // DrawRectangle(origin.x, origin.y, w/scale, h/scale, {255, 255, 255,
    // 100}); DrawText(TextFormat("%d", followStyle), mx + 5, my, 8, RED);
    // DrawText("Camera", x, y, 8, BLUE);
}

void CameraEnhanced::follow(std::shared_ptr<GameObject> target) {
    this->target = target;
}

void CameraEnhanced::setDeadzone(float x, float y, float w, float h) {
    deadzone = true;
    deadzone_x = x;
    deadzone_y = y;
    deadzone_w = w;
    deadzone_h = h;
}

void CameraEnhanced::setBounds(float x, float y, float w, float h) {
    bound = true;
    bounds_min_x = x;
    bounds_max_x = x + w;
    bounds_min_y = y;
    bounds_max_y = y + h;
}

void CameraEnhanced::setFollowStyle(FollowStyle followStyle) {
    this->followStyle = followStyle;
}

void CameraEnhanced::setFollowLerp(float x, float y) {
    follow_lerp_x = x;
    follow_lerp_y = y;
}

void CameraEnhanced::setFollowLead(float x, float y) {
    follow_lead_x = x;
    follow_lead_y = y;
}

void CameraEnhanced::flash(float duration, Color color) {
    flash_duration = duration;
    flash_color = color;
    flash_timer = 0;
    flashing = true;
}

void CameraEnhanced::fade(float duration, Color color,
                          std::function<void()> action) {
    fade_duration = duration;
    base_fade_color = this->fade_color;
    target_fade_color = color;
    fade_timer = 0;
    fade_action = action;
    fading = true;
}
