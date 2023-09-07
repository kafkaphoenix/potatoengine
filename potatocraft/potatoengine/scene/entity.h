#pragma once

#include <entt/entt.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/scene/scene.h"

namespace potatoengine {

class Entity {
   public:
    Entity(entt::entity e, Scene* scene);

    template <typename Component, typename... Args>
    Component& add(Args&&... args) {
        if (has_all<Component>()) {
            throw std::runtime_error("Entity already has component " + std::string(typeid(Component).name()));
        }
        auto& c = m_scene->m_registry.emplace<Component>(m_entity, std::forward<Args>(args)...);
        return c;
    }

    template <typename Component>
    void onComponentAdded(Component& c) {
        m_scene->onComponentAdded(*this, c);
    }

    template <typename Component>
    Component& get() {
        return m_scene->m_registry.get<Component>(m_entity);
    }

    template <typename Component>
    Component const& get() const {
        return m_scene->m_registry.get<Component>(m_entity);
    }

    template <typename... Components>
    bool has_all() const {
        return (m_scene->m_registry.all_of<Components>(m_entity) and ...);
    }

    template <typename... Components>
    bool has_any() const {
        return (m_scene->m_registry.any_of<Components>(m_entity) or ...);
    }

    template <typename... Components>
    void remove() {
        (m_scene->m_registry.remove<Components>(m_entity), ...);
    }

    operator bool() const noexcept;
    bool operator==(const Entity other) const noexcept;
    bool operator!=(const Entity other) const noexcept;
    operator entt::entity() const noexcept;
    operator uint32_t() const;
    operator uint64_t() const;
    operator std::string() const;

   private:
    entt::entity m_entity{entt::null};
    Scene* m_scene{};  // shared ptr would add a counter we dont need here as we don't own the object
};
}