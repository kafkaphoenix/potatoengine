#include "potatoengine/scene/entity.h"

#include "potatoengine/scene/components.h"

namespace potatoengine {

Entity::Entity(entt::entity e, Scene* scene) : m_entity(e), m_scene(scene) {}

Entity::operator bool() const noexcept {
    return m_entity not_eq entt::null;
}
bool Entity::operator==(const Entity other) const noexcept {
    return m_entity == other.m_entity and m_scene == other.m_scene;
}
bool Entity::operator!=(const Entity other) const noexcept {
    return *this not_eq other;
}

Entity::operator entt::entity() const noexcept {
    return m_entity;
}
Entity::operator uint32_t() const {
    return static_cast<uint32_t>(get<UUIDComponent>().uuid);
}
Entity::operator uint64_t() const {
    return get<UUIDComponent>().uuid;
}
Entity::operator std::string() const {
    return m_entity == entt::null ? "null" : get<Name>().name + ": " + std::to_string(get<UUIDComponent>().uuid);
}

}