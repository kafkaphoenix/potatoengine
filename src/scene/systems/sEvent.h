#pragma once

#include <entt/entt.hpp>

#include "events/event.h"
#include "scene/systems/common/sApp.h"
#include "scene/systems/common/sInput.h"

namespace potatoengine {

void eventSystem(entt::registry& registry, Event& e) {
  inputSystem(registry, e);
  appSystem(registry, e);
}
}