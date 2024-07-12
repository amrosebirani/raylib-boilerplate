#pragma once
#include <functional>
#include <unordered_map>
#include <string>
#include <utility>
#include <vector>
#include <memory>

float linear(float x, std::unordered_map<std::string, float> args);
float quad(float x, std::unordered_map<std::string, float> args);
float cubic(float x, std::unordered_map<std::string, float> args);
float quart(float x, std::unordered_map<std::string, float> args);
float quint(float x, std::unordered_map<std::string, float> args);
float sine(float x, std::unordered_map<std::string, float> args);
float expo(float x, std::unordered_map<std::string, float> args);
float circ(float x, std::unordered_map<std::string, float> args);
float back(float x, std::unordered_map<std::string, float> args);
float bounce(float x, std::unordered_map<std::string, float> args);
float elastic(float x, std::unordered_map<std::string, float> args);

enum class TimerTaskType { AFTER, EVERY, DURING, TWEEN };

class Timer {
    public:
        Timer();
        ~Timer();
        void check_print();

        std::string after(float delay, std::function<void(float dt)> action,
                          std::string tag);
        std::string every(float delay, std::function<void(float dt)> action,
                          int count, std::function<void()> afterAction,
                          std::string tag);
        std::string during(float delay, std::function<void(float dt)> action,
                           std::function<void()> afterAction, std::string tag);
        std::string
        tween(float delay,
              std::shared_ptr<std::unordered_map<std::string, float>> subject,
              std::unordered_map<std::string, float> target, std::string method,
              std::function<void()> afterFunction, std::string tag,
              std::unordered_map<std::string, float> args);
        void cancel(const std::string &tag);
        void update(float dt);

    private:
        struct TimerTask {
                float time = 0;
                float delay = 0;
                std::function<void(float dt)> action;
                TimerTaskType type;
                int count;
                int counter = 0;
                std::function<void()> afterAction;
                std::shared_ptr<std::unordered_map<std::string, float>>
                    subject = nullptr;
                std::unordered_map<std::string, float> target;
                std::string method;
                float last_s = 0;
                std::unordered_map<std::string, float> args;
                std::vector<std::pair<std::string, float>> payload;

                TimerTask();
        };

        std::unordered_map<std::string, std::shared_ptr<TimerTask>> timers;

        std::string generateUUID();
        float tweenValue(std::string method, float s,
                         std::unordered_map<std::string, float> args);

        std::vector<std::pair<std::string, float>> collectPayload(
            std::shared_ptr<std::unordered_map<std::string, float>> subject,
            std::unordered_map<std::string, float> target);
};
