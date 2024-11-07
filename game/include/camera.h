#pragma once
#include "shake.h"
#include "raylib.h"
#include "game_object.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include "timer.h"

enum class FollowStyle {
    LOCKON,
    PLATFORMER,
    TOPDOWN,
    TOPDOWN_TIGHT,
    SCREEN_BY_SCREEN,
    NO_DEADZONE,
    CUSTOM_DEADZONE,
};

float lerp(float a, float b, float c);
float csnap(float v, float x);

class CameraEnhanced {
    public:
        CameraEnhanced(float x, float y, float w, float h, float scale,
                       float rotation, std::shared_ptr<GameObject> target)
            : x(x), y(y), mx(x), my(y), screen_x(x), screen_y(y), w(w), h(h),
              scale(scale), rotation(rotation), target(target) {
            camera = std::make_shared<Camera2D>();
            params = std::make_shared<std::unordered_map<std::string, float>>();
        };

        void move(float dx, float dy);
        void moveTo(float x, float y);
        Vector2 toWorldCoords(float x, float y);
        Vector2 toCameraCoords(float x, float y);
        Vector2 getMousePosition();
        bool isRectangleTouched(Rectangle rect);
        void shake(float intensity, float duration, int frequency);
        void update(float dt);
        void draw();
        void setScale(float scale);
        void editScale(float change);
        float getScale();
        void tweenZoom(float change);
        void follow(std::shared_ptr<GameObject> target);
        void setDeadzone(float x, float y, float w, float h);
        void setBounds(float x, float y, float w, float h);
        void setFollowStyle(FollowStyle followStyle);
        void setFollowLerp(float x, float y);
        void setFollowLead(float x, float y);
        void flash(float duration, Color color);
        void fade(float duration, Color color, std::function<void()> action);
        std::shared_ptr<GameObject> getTarget();
        void attach(float vw, float vh);
        void detach();
        void drawWorldLine(float x1, float y1, float x2, float y2, Color color);
        bool adjustScreenScale = false;
        float x, y; // camera target or camera position

    private:
        std::shared_ptr<Camera2D> camera;

        float mx, my;             // mouse position in world coordinates
        float screen_x, screen_y; // screen position
        float w, h;               // camera width and height
        float scale;              // camera scale or zoom
        float rotation;           // camera rotation
        float ox, oy;             // offset x and offset y

        std::vector<std::shared_ptr<Shake>> horizontal_shakes, vertical_shakes;
        std::shared_ptr<GameObject> target = nullptr;
        std::shared_ptr<Vector2> last_target;
        float scroll_x = 0, scroll_y = 0;
        float follow_lerp_x = 1, follow_lerp_y = 1;
        float follow_lead_x = 0, follow_lead_y = 0;
        bool deadzone = false;
        bool bound = false;
        bool draw_deadzone = true;
        float flash_duration = 1;
        float flash_timer = 0;
        Color flash_color = {0, 0, 0, 255};
        float last_horizontal_shake_amount = 0;
        float last_vertical_shake_amount = 0;
        float fade_duration = 1;
        float fade_timer = 0;
        Color fade_color = {0, 0, 0, 0};
        Color base_fade_color;
        Color target_fade_color;
        std::function<void()> fade_action;
        FollowStyle followStyle = FollowStyle::NO_DEADZONE;
        bool flashing = false;
        bool fading = false;
        float bounds_min_x = 0;
        float bounds_min_y = 0;
        float bounds_max_x = 0;
        float bounds_max_y = 0;
        float deadzone_x = 0;
        float deadzone_y = 0;
        float deadzone_w = 0;
        float deadzone_h = 0;
        std::shared_ptr<std::unordered_map<std::string, float>> params;
        Timer timer;
};
