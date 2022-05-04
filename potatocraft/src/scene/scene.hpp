#pragma once

#include <entt/entt.hpp>

class Entity;

class Scene
{
public:
  Scene();
  ~Scene();

  Entity createEntity(const std::string &name = std::string());
  void destroyEntity(Entity entity);

private:
  template <typename T>
  void onComponentAdded(Entity entity, T &component);

  entt::registry m_registry;

  friend class Entity;
};