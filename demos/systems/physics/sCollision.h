#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class CollisionSystem : public engine::systems::System {
  public:
    CollisionSystem(int priority) : engine::systems::System(priority) {}
    ~CollisionSystem() override = default;

    void init(entt::registry& registry, const engine::Time& ts) override{};
    void update(entt::registry& registry, const engine::Time& ts) override;
    void shutdown(entt::registry& registry, const engine::Time& ts) override{};
};

}
