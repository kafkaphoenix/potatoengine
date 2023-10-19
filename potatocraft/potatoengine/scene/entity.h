#pragma once

#include <entt/entt.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/scene/sceneManager.h"

namespace potatoengine {

class Entity {
   public:
    Entity(entt::entity e, SceneManager* sceneManager);

    template <typename Component, typename... Args>
    Component& add(Args&&... args) {
        if (has_all<Component>()) {
            throw std::runtime_error("Entity already has component " + std::string(typeid(Component).name()));
        }
        return m_sceneManager->m_registry.emplace<Component>(m_entity, std::forward<Args>(args)...);
    }

    template <typename Component, typename... Args>
    Component& update(Args&&... args) {
        if (not has_all<Component>()) {
            throw std::runtime_error("Entity does not have component " + std::string(typeid(Component).name()));
        }
        return m_sceneManager->m_registry.replace<Component>(m_entity, std::forward<Args>(args)...);
    }

    template <typename Component>
    Component& get() {
        return m_sceneManager->m_registry.get<Component>(m_entity);
    }

    template <typename Component>
    Component const& get() const {
        return m_sceneManager->m_registry.get<Component>(m_entity);
    }

    template <typename... Components>
    bool has_all() const {
        return (m_sceneManager->m_registry.all_of<Components>(m_entity) and ...);
    }

    template <typename... Components>
    bool has_any() const {
        return (m_sceneManager->m_registry.any_of<Components>(m_entity) or ...);
    }

    template <typename... Components>
    void remove() {
        (m_sceneManager->m_registry.remove<Components>(m_entity), ...);
    }

    SceneManager& getSceneManager() const { return *m_sceneManager; }

    operator bool() const noexcept;
    bool operator==(const Entity other) const noexcept;
    bool operator!=(const Entity other) const noexcept;
    operator entt::entity() const noexcept;
    operator uint32_t() const;
    operator std::string() const;

   private:
    entt::entity m_entity{entt::null};
    SceneManager* m_sceneManager{};  // shared ptr would add a counter we dont need here as we don't own the object
};
}