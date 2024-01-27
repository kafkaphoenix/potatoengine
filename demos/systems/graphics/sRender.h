#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class RenderSystem : public engine::systems::System {
  public:
    RenderSystem(int priority) : engine::systems::System(priority) {}

    void update(entt::registry& registry,
                const engine::Time& ts) override final;
};

}
