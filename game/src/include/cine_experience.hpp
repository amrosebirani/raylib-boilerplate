#pragma once

class CineExperience {
    public:
        CineExperience();
        virtual void startExp() = 0;
        virtual void update(float dt) = 0;
        bool active;

    private:
};
