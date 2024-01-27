#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class DeleteSystem : public engine::systems::System {
  public:
    DeleteSystem(int priority) : engine::systems::System(priority) {}

    void update(entt::registry& registry,
                const engine::Time& ts) override final;
};

}
