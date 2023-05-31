#include "potatoengine/scene/entity.h"

#include "potatoengine/scene/components.h"

namespace potatoengine {

Entity::Entity(entt::entity entity, Scene* scene) : m_entity(entity), m_scene(scene) {}

Entity::operator bool() const {
    return m_entity != entt::null;
}
bool Entity::operator==(const Entity& other) const {
    return m_entity == other.m_entity && m_scene == other.m_scene;
}
bool Entity::operator!=(const Entity& other) const {
    return !(*this == other);
}

Entity::operator entt::entity() const {
    return m_entity;
}
Entity::operator uint32_t() const {
    return (uint32_t)m_entity;
}

}