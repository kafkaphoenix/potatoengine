#pragma once

#include <entt/entt.hpp>

#include "core/application.h"
#include "scene/components/utils/cDeleted.h"

namespace potatoengine {

void deleteSystem(entt::registry& registry) {
  auto to_destroy = registry.view<CDeleted>();
  registry.destroy(std::begin(to_destroy), std::end(to_destroy));
}
}