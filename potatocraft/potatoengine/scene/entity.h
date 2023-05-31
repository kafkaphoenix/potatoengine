#pragma once

#include <entt/entt.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/scene/scene.h"

namespace potatoengine {

class Entity {
   public:
    Entity(entt::entity entity, Scene* scene);

    template <typename Component, typename... Args>
    Component& add(Args&&... args) {
        return m_scene->m_registry.emplace<Component>(m_entity, std::forward<Args>(args)...);
    }

    template <typename Component>
    Component& get() {
        return m_scene->m_registry.get<Component>(m_entity);
    }

    template <typename... Components>
    bool has() {
        return (m_scene->m_registry.all_of<Components>(m_entity) && ...);
    }

    template <typename Component>
    void remove() {
        m_scene->m_registry.remove<Component>(m_entity);
    }

    operator bool() const;
    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;
    operator entt::entity() const;
    operator uint32_t() const;

   private:
    entt::entity m_entity{entt::null};
    Scene* m_scene = nullptr;  // shared ptr would add a counter we dont need here as we don't own the object
};
}