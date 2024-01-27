#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class CollisionSystem : public engine::systems::System {
  public:
    CollisionSystem(int priority) : engine::systems::System(priority) {}

    void update(entt::registry& registry,
                const engine::Time& ts) override final;
};

}
