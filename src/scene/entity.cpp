#include "scene/entity.h"

#include "scene/components/common/cName.h"
#include "scene/components/common/cUUID.h"

namespace potatoengine {

Entity::Entity(entt::entity e, SceneManager* sceneManager) : m_entity(e), m_sceneManager(sceneManager) {}

Entity::operator bool() const noexcept { return m_entity not_eq entt::null; }
bool Entity::operator==(const Entity other) const noexcept {
  return m_entity == other.m_entity and m_sceneManager->getActiveScene() == other.m_sceneManager->getActiveScene();
}
bool Entity::operator!=(const Entity other) const noexcept { return *this not_eq other; }

Entity::operator entt::entity() const noexcept { return m_entity; }
Entity::operator uint32_t() const { return get<CUUID>().uuid; }
Entity::operator std::string() const {
  return m_entity == entt::null ? "null" : get<CName>().name + ": " + std::to_string(get<CUUID>().uuid);
}

}