#pragma once

#include <entt/entt.hpp>
#include "potatoengine/events/event.h"
#include "potatoengine/scene/systems/common/sInput.h"

namespace potatoengine {

void eventSystem(entt::registry& reg, Event& e) {
    inputSystem(reg, e);
}
}