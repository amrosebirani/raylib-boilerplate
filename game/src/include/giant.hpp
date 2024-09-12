#include "warrior.hpp"
#include "warrior_types.h"

class Giant : public Warrior {
    public:
        Giant(float rx, float ry)
            : Warrior(get_warrior_size(WarriorType::WARRIOR_TYPE_GIANT), rx,
                      ry) {
            graphics_types.push_back("warrior");
        };

        void draw();

        void update(float dt);

        bool isAlive();

        void die();

        void attack();

        void tryAttack(GameObject *enemy);
        void takeAttack(float damage);
};
