#pragma once

#include <entt/entt.hpp>

#include "potatoengine/scene/entity.h"

namespace potatoengine {

template <typename Component, typename... Args>
// cppcheck-suppress unusedFunction
Component& assign(Entity e, Args... args) {
    return e.add<Component>(std::forward<Args>(args)...);
}

template <typename Component>
// cppcheck-suppress unusedFunction
Component& onComponentAdded(Entity e, Component& c) {
    e.getScene().onComponentAdded<Component>(e, c);
    return c;
}

template <typename Component>
// cppcheck-suppress unusedFunction
Component& onComponentCloned(Entity e, Component& c) {
    e.getScene().onComponentCloned<Component>(e, c);
    return c;
}
}