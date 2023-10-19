#pragma once

#include <entt/entt.hpp>
#include "potatoengine/scene/components/utils/cDeleted.h"

namespace potatoengine {

void deleteSystem(entt::registry& r) {
    auto to_destroy = r.view<CDeleted>();
    r.destroy(std::begin(to_destroy), std::end(to_destroy));
}
}