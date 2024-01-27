#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class ScoreSystem : public engine::systems::System {
  public:
    ScoreSystem(int priority) : engine::systems::System(priority) {}
    ~ScoreSystem() override final;

    void init(entt::registry& registry) override final;
};

}
