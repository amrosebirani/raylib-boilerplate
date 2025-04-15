#include "game_object.h"
#include <fstream>

GameObject::GameObject(std::ifstream &in) {
    in.read(reinterpret_cast<char *>(id.data()), 16);
    in.read(reinterpret_cast<char *>(&x), sizeof(x));
    in.read(reinterpret_cast<char *>(&y), sizeof(y));
}

GameObject::GameObject(float x, float y) : x(x), y(y) {
    timer = std::make_shared<Timer>();
    creation_time = time(NULL);
    id = generateUUID();
}

GameObject::GameObject(float x, float y, UUID id) : x(x), y(y), id(id) {
    timer = std::make_shared<Timer>();
    creation_time = time(NULL);
}

std::shared_ptr<GameObject> GameObject::get_shared_ptr() {
    return shared_from_this();
}

void GameObject::gameObjectSave(std::ofstream &out) const {
    out.write(reinterpret_cast<const char *>(id.data()), 16);
    out.write(reinterpret_cast<const char *>(&x), sizeof(x));
    out.write(reinterpret_cast<const char *>(&y), sizeof(y));
}

UUID GameObject::getId() {
    return id;
}
