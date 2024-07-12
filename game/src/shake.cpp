#include "shake.h"
#include <cstdlib>

Shake::Shake(float amp, float dur, int freq) {
    amplitude = amp;
    duration = dur;
    frequency = freq;
    int sample_count = (duration) * frequency;
    for (int i=0; i<sample_count; ++i) {
        samples.push_back(2 * static_cast<float>(std::rand())/RAND_MAX - 1);
    }
}

void Shake::update(float dt) {
    t += dt;
    if (t > duration) {
        shaking = false;
    }
}

float Shake::noise(float s) {
    int idx = static_cast<int>(s);
    if (idx >= samples.size())
        return 0;
    return samples[idx];
}

float Shake::decay(float t) {
    if (t > duration) {
        return 0;
    }
    return (duration - t) / duration;
}

float Shake::getAmplitude(float t) {
    float s = t * frequency;
    int s0 = static_cast<int>(s);
    int s1 = s0 + 1;
    float k = decay(t);
    return amplitude  * (noise(s0) + (s-s0)*(noise(s1) - noise(s0))) * k;
}
