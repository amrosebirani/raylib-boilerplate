#pragma once
#include <vector>

struct Shake {
    float amplitude = 0;
    float duration = 0;
    float frequency = 60;
    std::vector<float> samples;
    float t = 0;
    bool shaking = true;

    Shake(float amp, float dur, int freq);

    void update(float dt);
    float noise(float s);
    float decay(float s);
    float getAmplitude(float t);


};
