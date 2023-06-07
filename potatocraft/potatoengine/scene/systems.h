#pragma once

#include "potatoengine/pch.h"
#include "potatoengine/scene/components.h"

namespace potatoengine {

void deleteSystem(entt::registry& r) {
    r.clear<Deleted>();
}

void updateSystems(entt::registry& r, Time ts) {
    deleteSystem(r);
}
}