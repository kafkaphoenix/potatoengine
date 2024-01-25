#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class DeleteSystem : public engine::systems::System {
  public:
    DeleteSystem(int priority) : engine::systems::System(priority) {}
    ~DeleteSystem() override = default;

    void init(entt::registry& registry, const engine::Time& ts) override{};
    void update(entt::registry& registry, const engine::Time& ts) override;
    void shutdown(entt::registry& registry, const engine::Time& ts) override{};
};

}
