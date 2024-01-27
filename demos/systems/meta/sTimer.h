#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class TimerSystem : public engine::systems::System {
  public:
    TimerSystem(int priority) : engine::systems::System(priority) {}
    ~TimerSystem() override final;

    void init(entt::registry& registry) override final;
};

}
