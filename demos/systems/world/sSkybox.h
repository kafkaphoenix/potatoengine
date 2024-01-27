#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class SkyboxSystem : public engine::systems::System {
  public:
    SkyboxSystem(int priority) : engine::systems::System(priority) {}
    ~SkyboxSystem() override final;

    void init(entt::registry& registry) override final;
    void update(entt::registry& registry,
                const engine::Time& ts) override final;
};

}
