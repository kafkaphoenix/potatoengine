#include "scene/sceneManager.h"

#include "scene/components/core/cName.h"
#include "scene/components/core/cUUID.h"
#include "scene/utils.h"

using namespace entt::literals;

namespace potatoengine {

SceneManager::SceneManager() : m_sceneFactory() {
  ENGINE_TRACE("Initializing scene manager...");
  ENGINE_TRACE("Registering engine components...");
  RegisterComponents();
  ENGINE_TRACE("Scene manager created!");
}

void SceneManager::registerSystem(std::string&& name,
                                  std::unique_ptr<systems::System>&& system) {
  ENGINE_ASSERT(not containsSystem(name), "System {} already registered", name);
  system->init(m_registry);
  m_systems.emplace(std::make_pair(std::move(name), std::move(system)));
  dirtySystems = true;
}

void SceneManager::unregisterSystem(std::string_view name) {
  bool deleted = false;
  for (auto it = m_systems.begin(); it != m_systems.end(); ++it) {
    if (it->first == name) {
      m_systems.erase(it);
      deleted = true;
      break;
    }
  }
  ENGINE_ASSERT(deleted, "System {} not found", name);
  dirtySystems = true;
}

bool SceneManager::containsSystem(std::string_view name) {
  for (const auto& [n, _] : m_systems) {
    if (n == name) {
      return true;
    }
  }
  return false;
}

void SceneManager::clearSystems() {
  m_systems.clear();
  dirtySystems = false;
}

void SceneManager::onUpdate(const Time& ts) {
  for (const auto& [_, system] : m_systems) {
    system->update(m_registry, ts);
  }
}

entt::registry& SceneManager::getRegistry() noexcept { return m_registry; }

entt::entity SceneManager::getEntity(std::string_view name) {
  for (const auto& [e, cName, _] : m_registry.view<CName, CUUID>().each()) {
    if (cName.name == name) {
      return e;
    }
  }
  ENGINE_ASSERT(false, "Entity with name {} not found", name);
}

entt::entity SceneManager::getEntity(UUID& uuid) {
  for (const auto& [e, cUUID] : m_registry.view<CUUID>().each()) {
    if (cUUID.uuid == uuid) {
      return e;
    }
  }
  ENGINE_ASSERT(false, "Entity with UUID {} not found", std::to_string(uuid));
}

const std::vector<std::string>& SceneManager::getNamedSystems() {
  if (not dirtySystems) {
    return m_namedSystems;
  }

  m_namedSystems.clear();
  for (const auto& [name, system] : m_systems) {
    m_namedSystems.emplace_back(name + " - Priority " +
                                std::to_string(system->getPriority()));
  }
  dirtySystems = false;

  return m_namedSystems;
}

template <typename Component>
void SceneManager::onComponentAdded(entt::entity e, Component& c) {
  ENGINE_ASSERT(false,
                "Unsupported onComponentAdded method for component type {}",
                entt::type_id<Component>().name());
}

template <typename Component>
void SceneManager::onComponentCloned(entt::entity e, Component& c) {
  ENGINE_ASSERT(false,
                "Unsupported onComponentCloned method for component type {}",
                entt::type_id<Component>().name());
}

std::unique_ptr<SceneManager> SceneManager::Create() {
  return std::make_unique<SceneManager>();
}

entt::entity SceneManager::createEntity(std::string_view prefabID,
                                        std::string&& prototypeID,
                                        std::string&& name,
                                        std::optional<std::string> tag,
                                        std::optional<uint32_t> uuid) {
  return m_sceneFactory.createEntity(prefabID, std::move(prototypeID),
                                     m_registry, std::move(name), tag, uuid);
}

entt::entity SceneManager::cloneEntity(const entt::entity& e) {
  return m_sceneFactory.cloneEntity(e, UUID(), m_registry);
}

void SceneManager::removeEntity(entt::entity& e) {
  m_sceneFactory.removeEntity(e, m_registry);
}

void SceneManager::createScene(std::string scene_name, std::string scene_path) {
  auto& app = Application::Get();
  m_sceneFactory.createScene(scene_name, scene_path, app.getAssetsManager(),
                             app.getRenderManager(), m_registry);
  PrintScene(m_registry);
}

void SceneManager::reloadScene(bool reload_prototypes) {
  auto& app = Application::Get();
  m_sceneFactory.reloadScene(app.getAssetsManager(), app.getRenderManager(),
                             m_registry, reload_prototypes);
}

void SceneManager::clearScene() {
  m_sceneFactory.clearScene(Application::Get().getRenderManager(), m_registry);
  m_systems.clear();
  m_namedSystems.clear();
  dirtySystems = false;
}

std::string SceneManager::getActiveScene() const {
  return m_sceneFactory.getActiveScene();
}

const std::map<std::string, entt::entity, NumericComparator>&
SceneManager::getNamedEntities() {
  return m_sceneFactory.getNamedEntities(m_registry);
}

const std::map<std::string, std::string, NumericComparator>&
SceneManager::getMetrics() {
  return m_sceneFactory.getMetrics(m_registry);
}

void SceneManager::createPrototypes(
  std::string_view prefab_name, const std::vector<std::string>& prototypeIDs) {
  m_sceneFactory.getEntityFactory().createPrototypes(
    prefab_name, prototypeIDs, m_registry,
    Application::Get().getAssetsManager());
}

void SceneManager::updatePrototypes(
  std::string_view prefab_name, const std::vector<std::string>& prototypeIDs) {
  m_sceneFactory.getEntityFactory().updatePrototypes(
    prefab_name, prototypeIDs, m_registry,
    Application::Get().getAssetsManager());
}

void SceneManager::destroyPrototypes(
  std::string_view prefab_name, const std::vector<std::string>& prototypeIDs) {
  m_sceneFactory.getEntityFactory().destroyPrototypes(prefab_name, prototypeIDs,
                                                      m_registry);
}

EntityFactory::Prototypes
SceneManager::getPrototypes(std::string_view prefab_name,
                            const std::vector<std::string>& prototypeIDs) {
  return m_sceneFactory.getEntityFactory().getPrototypes(prefab_name,
                                                         prototypeIDs);
}

bool SceneManager::containsPrototypes(
  std::string_view prefab_name, const std::vector<std::string>& prototypeIDs) {
  return m_sceneFactory.getEntityFactory().containsPrototypes(prefab_name,
                                                              prototypeIDs);
}

const std::map<std::string, EntityFactory::Prototypes, NumericComparator>&
SceneManager::getAllPrototypes() {
  return m_sceneFactory.getEntityFactory().getAllPrototypes();
}

const std::map<std::string, std::string, NumericComparator>&
SceneManager::getPrototypesCountByPrefab() {
  return m_sceneFactory.getEntityFactory().getPrototypesCountByPrefab();
}

void SceneManager::clearPrototypes() {
  m_sceneFactory.getEntityFactory().clearPrototypes();
}

}