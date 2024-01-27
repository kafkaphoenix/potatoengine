#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class CoinsSystem : public engine::systems::System {
  public:
    CoinsSystem(int priority) : engine::systems::System(priority) {}
    ~CoinsSystem() override final;

    void init(entt::registry& registry) override final;
    void update(entt::registry& registry,
                const engine::Time& ts) override final;
};

}
