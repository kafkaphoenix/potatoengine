#include "potatoengine/scene/entity.h"

#include "potatoengine/scene/components/common/cName.h"
#include "potatoengine/scene/components/common/cUUID.h"

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
    return get<CUUID>().uuid;
}
Entity::operator std::string() const {
    return m_entity == entt::null ? "null" : get<CName>().name + ": " + std::to_string(get<CUUID>().uuid);
}

}