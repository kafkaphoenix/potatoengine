#pragma once

#include "events/event.h"
#include "scene/systems/common/sApp.h"
#include "scene/systems/common/sInput.h"
#include <entt/entt.hpp>

namespace potatoengine {

void eventSystem(entt::registry& reg, Event& e) {
  inputSystem(reg, e);
  appSystem(e);
}
}