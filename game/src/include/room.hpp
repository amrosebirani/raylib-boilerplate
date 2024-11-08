#pragma once

class Room {
    public:
        virtual bool update(float dt) = 0;
        virtual void draw() = 0;
        virtual bool isFinished() = 0;
};
