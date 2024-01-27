#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class MovementSystem : public engine::systems::System {
  public:
    MovementSystem(int priority) : engine::systems::System(priority) {}

    void update(entt::registry& registry,
                const engine::Time& ts) override final;
};

}
