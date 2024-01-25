#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class GravitySystem : public engine::systems::System {
  public:
    GravitySystem(int priority) : engine::systems::System(priority) {}
    ~GravitySystem() override = default;

    void init(entt::registry& registry, const engine::Time& ts) override{};
    void update(entt::registry& registry, const engine::Time& ts) override;
    void shutdown(entt::registry& registry, const engine::Time& ts) override{};
};

}
