#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class PipesSystem : public engine::systems::System {
  public:
    PipesSystem(int priority) : engine::systems::System(priority) {}
    ~PipesSystem() override final;

    void init(entt::registry& registry) override final;
    void update(entt::registry& registry,
                const engine::Time& ts) override final;
};

}
