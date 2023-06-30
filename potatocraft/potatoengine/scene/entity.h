#pragma once

#include <entt/entt.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/scene/components.h"
#include "potatoengine/scene/scene.h"

namespace potatoengine {

class Entity {
   public:
    Entity(entt::entity entity, Scene* scene);

    template <typename Component, typename... Args>
    void add(Args&&... args) {
        if (has_all<Component>()) {
            throw std::runtime_error("Entity already has component " + std::string(typeid(Component).name()));
        }
        m_scene->m_registry.emplace<Component>(m_entity, std::forward<Args>(args)...);
    }

    template <typename... Components>
    void addCTags(const std::vector<std::string>& ctags) {
        std::apply([&ctags, this](auto&&... components) { (addIfCTagMatches<std::decay_t<decltype(components)>>(ctags), ...); }, allCTags{});
    }

    template <typename Component>
    void addIfCTagMatches(const std::vector<std::string>& ctags) {
        if (std::ranges::find(ctags, getCTag<Component>()) != ctags.end()) {
            add<Component>();
        }
    }

    template <typename Component>
    std::string getCTag() {
        try {
            return allCTypes.at(std::type_index(typeid(Component)));
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Component " + std::string(typeid(Component).name()) + " not found! Is it defined?");
        }
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
    bool has_all() {
        return (m_scene->m_registry.all_of<Components>(m_entity) and ...);
    }

    template <typename... Components>
    bool has_any() {
        return (m_scene->m_registry.any_of<Components>(m_entity) or ...);
    }

    template <typename... Components>
    void remove() {
        (m_scene->m_registry.remove<Components>(m_entity), ...);
    }

    operator bool() const;
    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;
    operator entt::entity() const;
    operator uint32_t() const;
    operator uint64_t() const;
    operator std::string() const;

   private:
    entt::entity m_entity{entt::null};
    Scene* m_scene;  // shared ptr would add a counter we dont need here as we don't own the object
};
}