#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class LightSystem : public engine::systems::System {
  public:
    LightSystem(int priority) : engine::systems::System(priority) {}
    ~LightSystem() override = default;

    void init(entt::registry& registry, const engine::Time& ts) override{};
    void update(entt::registry& registry, const engine::Time& ts) override;
    void shutdown(entt::registry& registry, const engine::Time& ts) override{};
};

}
