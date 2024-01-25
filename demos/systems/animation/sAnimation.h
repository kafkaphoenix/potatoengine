#pragma once

#include <entt/entt.hpp>

#include "engineAPI.h"

namespace demos::systems {

class AnimationSystem : public engine::systems::System {
  public:
    AnimationSystem(int priority) : engine::systems::System(priority) {}
    ~AnimationSystem() override = default;

    void init(entt::registry& registry, const engine::Time& ts) override{};
    void update(entt::registry& registry, const engine::Time& ts) override;
    void shutdown(entt::registry& registry, const engine::Time& ts) override{};
};

}
