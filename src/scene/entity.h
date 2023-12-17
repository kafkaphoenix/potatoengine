#pragma once

#include <entt/entt.hpp>

#include "pch.h"
#include "scene/sceneManager.h"

namespace potatoengine {

class Entity {
  public:
    Entity(entt::entity e, SceneManager* sceneManager);

    template <typename Component, typename... Args> Component& add(Args&&... args) {
      ENGINE_ASSERT(not has_all<Component>(), "Entity already has component {}", typeid(Component).name());
      return m_sceneManager->m_registry.emplace<Component>(m_entity, std::forward<Args>(args)...);
    }

    template <typename Component, typename... Args> Component& update(Args&&... args) {
      ENGINE_ASSERT(has_all<Component>(), "Entity does not have component {}", typeid(Component).name());
      return m_sceneManager->m_registry.replace<Component>(m_entity, std::forward<Args>(args)...);
    }

    template <typename Component> Component& get() { return m_sceneManager->m_registry.get<Component>(m_entity); }

    template <typename Component> Component* try_get() {
      return m_sceneManager->m_registry.try_get<Component>(m_entity);
    }

    template <typename Component> Component const& get() const {
      return m_sceneManager->m_registry.get<Component>(m_entity);
    }

    template <typename... Components> bool has_all() const {
      return (m_sceneManager->m_registry.all_of<Components>(m_entity) and ...);
    }

    template <typename... Components> bool has_any() const {
      return (m_sceneManager->m_registry.any_of<Components>(m_entity) or ...);
    }

    template <typename... Components> void remove() { (m_sceneManager->m_registry.remove<Components>(m_entity), ...); }

    SceneManager& getSceneManager() const { return *m_sceneManager; }

    operator bool() const;
    bool operator==(const Entity other) const;
    bool operator!=(const Entity other) const;
    operator entt::entity() const noexcept;
    operator uint32_t() const;
    operator std::string() const;

  private:
    entt::entity m_entity{entt::null};
    SceneManager* m_sceneManager; // shared ptr would add a counter we do not need here as we do not own the object
};
}