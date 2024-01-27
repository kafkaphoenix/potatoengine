#pragma once

#include <entt/entt.hpp>

namespace potatoengine {

template <typename Component, typename... Args>
inline Component& assign(entt::entity e, Args... args) {
  auto& registry = Application::Get().getSceneManager()->getRegistry();
  ENGINE_ASSERT(not registry.all_of<Component>(e), "Entity already has component {}",
                typeid(Component).name());
  return registry.emplace<Component>(e, std::forward<Args>(args)...);
}

template <typename Component>
inline Component& onComponentAdded(entt::entity e, Component& c) {
  Application::Get().getSceneManager()->onComponentAdded<Component>(e, c);
  return c;
}

template <typename Component>
inline Component& onComponentCloned(entt::entity e, Component& c) {
  Application::Get().getSceneManager()->onComponentCloned<Component>(e, c);
  return c;
}
}