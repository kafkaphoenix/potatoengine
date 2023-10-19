#pragma once

#include "potatoengine/renderer/camera/Camera.h"
#include "potatoengine/scene/entity.h"

namespace potatoengine {

struct CCamera {
    Camera camera;  // TODO maybe remove default constructor

    CCamera() = default;
    explicit CCamera(Camera c) : camera(std::move(c)) {}

    void print() const {
        CORE_INFO("\t\tcamera: No implemented"); // TODO finish 
    }
};
}

template <>
void engine::SceneManager::onComponentAdded(Entity e, CCamera& c) { // TODO finish
}