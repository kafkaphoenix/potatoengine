#pragma once

#include <entt/entt.hpp>

#include "core/application.h"
#include "scene/components/common/cDeleted.h"

namespace potatoengine {

void deleteSystem(entt::registry& registry) {
  auto to_destroy = registry.view<CDeleted>();
  registry.destroy(to_destroy.begin(), to_destroy.end());
}
}