#pragma once
#include <string>

enum class CameraTransitionType {
    MOVE_TO_POINT_X,
    MOVE_TO_POINT_Y,
    ZOOM_IN,
    ZOOM_OUT,
    RECOVER_TO_FORMATION,
};

class CameraTransition {
    public:
        CameraTransition(CameraTransitionType type, float timeAfter,
                         float duration, float startVal, float endVal,
                         std::string label)
            : type(type), timeAfter(timeAfter), duration(duration),
              startVal(startVal), endVal(endVal), label(label) {};

        float timeAfter;
        float duration;
        CameraTransitionType type;

        float startVal;
        float endVal;
        std::string label;

    private:
};
