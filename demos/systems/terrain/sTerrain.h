#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class TerrainSystem : public engine::systems::System {
  public:
    TerrainSystem(int priority) : engine::systems::System(priority) {}
    ~TerrainSystem() override = default;

    void init(entt::registry& registry, const engine::Time& ts) override{};
    void update(entt::registry& registry, const engine::Time& ts) override;
    void shutdown(entt::registry& registry, const engine::Time& ts) override{};
};

}
