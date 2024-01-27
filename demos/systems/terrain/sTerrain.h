#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class TerrainSystem : public engine::systems::System {
  public:
    TerrainSystem(int priority) : engine::systems::System(priority) {}

    void init(entt::registry& registry) override final;
    void update(entt::registry& registry,
                const engine::Time& ts) override final;
};

}
