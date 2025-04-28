#include "timer.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <random>
#include <unordered_map>
#include <string_view>
#include <utility>

Timer::Timer() {
}

Timer::~Timer() {
    timers.clear();
}

void Timer::clearAll() {
    timers.clear();
}

Timer::TimerTask::TimerTask() {};

void Timer::check_print() {
    // std::cout << "Total Timers: " << timers.size() << std::endl;
}

std::string Timer::generateUUID() {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    std::string res;
    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            res += "-";
        } else {
            res += v[dist(rng)];
        }
    }
    return res;
}

std::string Timer::after(float delay, std::function<void(float dt)> action,
                         std::string tag) {
    if (tag == "") {
        tag = generateUUID();
    }

    cancel(tag);
    std::shared_ptr<TimerTask> thisTask = std::make_shared<TimerTask>();
    thisTask->delay = delay;
    thisTask->action = action;
    thisTask->type = TimerTaskType::AFTER;

    timers.emplace(tag, thisTask);

    return tag;
}

std::string Timer::during(float delay, std::function<void(float dt)> action,
                          std::function<void()> afterAction, std::string tag) {
    if (tag == "") {
        tag = generateUUID();
    }

    cancel(tag);

    std::shared_ptr<TimerTask> thisTask = std::make_shared<TimerTask>();
    thisTask->delay = delay;
    thisTask->action = action;
    thisTask->afterAction = afterAction;
    thisTask->type = TimerTaskType::DURING;

    timers.emplace(tag, thisTask);

    return tag;
}

std::string Timer::every(float delay, std::function<void(float dt)> action,
                         int count, std::function<void()> afterAction,
                         std::string tag) {
    if (tag == "") {
        tag = generateUUID();
    }

    cancel(tag);

    std::shared_ptr<TimerTask> thisTask = std::make_shared<TimerTask>();
    thisTask->time = 0;
    thisTask->delay = delay;
    thisTask->action = action;
    thisTask->count = count;
    thisTask->counter = 0;
    thisTask->afterAction = afterAction;
    thisTask->type = TimerTaskType::EVERY;

    timers.emplace(tag, thisTask);

    return tag;
}

std::vector<std::pair<std::string, float>> Timer::collectPayload(
    std::shared_ptr<std::unordered_map<std::string, float>> subject,
    std::unordered_map<std::string, float> target) {

    std::vector<std::pair<std::string, float>> retVal;
    for (auto &p : target) {
        float ref = (*subject)[p.first];
        float delta = p.second - ref;
        retVal.push_back({p.first, delta});
    }

    return retVal;
}

std::string
Timer::tween(float delay,
             std::shared_ptr<std::unordered_map<std::string, float>> subject,
             std::unordered_map<std::string, float> target, std::string method,
             std::function<void()> afterFunction, std::string tag,
             std::unordered_map<std::string, float> args) {
    if (tag == "") {
        tag = generateUUID();
    }

    cancel(tag);

    std::shared_ptr<TimerTask> thisTask = std::make_shared<TimerTask>();
    thisTask->type = TimerTaskType::TWEEN;
    thisTask->time = 0;
    thisTask->delay = delay;
    thisTask->subject = subject;
    thisTask->target = target;
    thisTask->method = method;
    thisTask->afterAction = afterFunction;
    thisTask->args = args;
    thisTask->payload = collectPayload(subject, target);

    timers.emplace(tag, thisTask);

    return tag;
}

void Timer::cancel(const std::string &tag) {
    timers.erase(tag);
}

float Timer::getAfterTaskRemainingTime(const std::string &tag) const {
    auto it = timers.find(tag);
    if (it != timers.end()) {
        std::shared_ptr<TimerTask> task = it->second;
        if (task->type == TimerTaskType::AFTER) {
            return task->delay - task->time;
        }
    }
    return -1;
}

void Timer::update(float dt) {
    for (auto it = timers.begin(); it != timers.end();) {
        auto &pair = *it;
        std::shared_ptr<TimerTask> task = pair.second;
        task->time = task->time + dt;

        if (task->type == TimerTaskType::AFTER) {
            if (task->time >= task->delay) {
                task->action(dt);
                it = timers.erase(it);
                continue;
            }
        }

        if (task->type == TimerTaskType::EVERY) {
            if (task->time >= task->delay) {
                task->action(dt);
                task->time = task->time - task->delay;
                if (task->count > 0) {
                    task->counter += 1;
                    if (task->counter >= task->count) {
                        task->afterAction();
                        it = timers.erase(it);
                        continue;
                    }
                }
            }
        }

        if (task->type == TimerTaskType::TWEEN) {
            float s = tweenValue(task->method,
                                 std::min(1.0f, task->time / task->delay),
                                 task->args);
            float ds = s - task->last_s;
            task->last_s = s;
            for (auto &pp : task->payload) {
                std::shared_ptr<std::unordered_map<std::string, float>>
                    &subject = task->subject;
                (*subject)[pp.first] += pp.second * ds;
            }

            if (task->time >= task->delay) {
                task->afterAction();
                it = timers.erase(it);
                continue;
            }
        }

        if (task->type == TimerTaskType::DURING) {
            task->action(dt);
            if (task->time >= task->delay) {
                task->afterAction();
                it = timers.erase(it);
                continue;
            }
        }

        ++it;
    }
}

float linear(float x, std::unordered_map<std::string, float> args) {
    return x;
}

float quad(float x, std::unordered_map<std::string, float> args) {
    return x * x;
}

float cubic(float x, std::unordered_map<std::string, float> args) {
    return x * x * x;
}

float quart(float x, std::unordered_map<std::string, float> args) {
    return x * x * x * x;
}

float quint(float x, std::unordered_map<std::string, float> args) {
    return x * x * x * x * x;
}

float sine(float x, std::unordered_map<std::string, float> args) {
    return 1 - std::cos(x * M_PI / 2);
}

float expo(float x, std::unordered_map<std::string, float> args) {
    return std::pow(2, 10 * (x - 1));
}

float circ(float x, std::unordered_map<std::string, float> args) {
    return 1 - std::sqrt(1 - x * x);
}

float back(float s, std::unordered_map<std::string, float> args) {
    float bounciness = args["bounciness"];
    return s * s * ((bounciness + 1) * s - bounciness);
}

float bounce(float s, std::unordered_map<std::string, float> args) {
    float a = 7.5625;
    float b = 1 / 2.75;
    return std::min(std::min(a * pow(s, 2), a * pow((s - 1.5 * b), 2) + 0.75),
                    std::min(a * pow(s - 2.25 * b, 2) + 0.9375,
                             a * pow(s - 2.625 * b, 2) + 0.984375));
}

float elastic(float s, std::unordered_map<std::string, float> args) {
    float amp = args["amp"];
    float period = args["period"];
    amp = amp > 1 ? amp : 1;
    period = period != 0 ? period : 0.3;

    double result =
        (-amp * std::sin(2 * M_PI / period * (s - 1) - std::asin(1 / amp))) *
        std::pow(2, 10 * (s - 1));

    return result;
}

std::function<float(float, std::unordered_map<std::string, float>)>
chain(std::function<float(float, std::unordered_map<std::string, float>)> f1,
      std::function<float(float, std::unordered_map<std::string, float>)> f2) {
    return [=](float s, std::unordered_map<std::string, float> args) {
        return ((s < 0.5) ? f1(2 * s, args) : 1 + f2(2 * s - 1, args)) * 0.5;
    };
}

std::function<float(float, std::unordered_map<std::string, float>)>
out(std::function<float(float, std::unordered_map<std::string, float>)> f) {
    return [=](float s, std::unordered_map<std::string, float> args) {
        return 1 - f(1 - s, args);
    };
}

float Timer::tweenValue(std::string method, float s,
                        std::unordered_map<std::string, float> args) {
    std::string_view strView = method;

    std::unordered_map<
        std::string,
        std::function<float(float, std::unordered_map<std::string, float>)>>
        functionMap;
    functionMap["elastic"] = [](float s,
                                std::unordered_map<std::string, float> args) {
        return elastic(s, args);
    };

    functionMap["bounce"] = [](float s,
                               std::unordered_map<std::string, float> args) {
        return bounce(s, args);
    };

    functionMap["back"] = [](float s,
                             std::unordered_map<std::string, float> args) {
        return back(s, args);
    };

    functionMap["expo"] = [](float s,
                             std::unordered_map<std::string, float> args) {
        return expo(s, args);
    };

    functionMap["quad"] = [](float s,
                             std::unordered_map<std::string, float> args) {
        return quad(s, args);
    };

    functionMap["cubic"] = [](float s,
                              std::unordered_map<std::string, float> args) {
        return cubic(s, args);
    };

    functionMap["quart"] = [](float s,
                              std::unordered_map<std::string, float> args) {
        return quart(s, args);
    };

    functionMap["quint"] = [](float s,
                              std::unordered_map<std::string, float> args) {
        return quint(s, args);
    };

    functionMap["sine"] = [](float s,
                             std::unordered_map<std::string, float> args) {
        return sine(s, args);
    };

    functionMap["circ"] = [](float s,
                             std::unordered_map<std::string, float> args) {
        return circ(s, args);
    };

    if (method == "linear") {
        return linear(s, args);
    } else if (strView.substr(0, 6) == "in-out") {
        std::string funcName = std::string(strView.substr(7));
        return chain(functionMap[funcName], out(functionMap[funcName]))(s,
                                                                        args);
    } else if (strView.substr(0, 6) == "out-in") {
        std::string funcName = std::string(strView.substr(7));
        return chain(out(functionMap[funcName]), functionMap[funcName])(s,
                                                                        args);
    } else if (strView.substr(0, 4) == "out-") {
        std::string funcName = std::string(strView.substr(4));
        return out(functionMap[funcName])(s, args);
    } else if (strView.substr(0, 3) == "in-") {
        std::string funcName = std::string(strView.substr(3));
        return functionMap[funcName](s, args);
    }

    return 0.0f;
}
