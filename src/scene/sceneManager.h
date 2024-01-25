#pragma once

#include <entt/entt.hpp>

#include "assets/assetsManager.h"
#include "core/time.h"
#include "events/event.h"
#include "scene/sceneFactory.h"
#include "scene/system.h"
#include "utils/uuid.h"

namespace potatoengine {
class SceneManager {
  public:
    SceneManager();
    void registerSystem(std::unique_ptr<systems::System>&& system);
    void onUpdate(const Time& ts);
    entt::registry& getRegistry() noexcept;
    entt::entity getEntity(std::string_view name);
    entt::entity getEntity(UUID& uuid);
    const std::vector<std::string>& getNamedSystems();

    template <typename Component>
    void onComponentAdded(entt::entity e, Component& c);
    template <typename Component>
    void onComponentCloned(entt::entity e, Component& c);

    static std::unique_ptr<SceneManager> Create();

    // scene factory methods
    entt::entity
    createEntity(std::string_view prefabID, std::string&& prototypeID,
                 std::string&& name,
                 const std::optional<uint32_t>& uuid = std::nullopt);
    entt::entity cloneEntity(const entt::entity& e);
    void removeEntity(entt::entity& e);

    void createScene(std::string scene_name, std::string scene_path);
    void reloadScene(bool reload_prototypes);
    void clearScene();
    std::string getActiveScene() const;
    const std::map<std::string, entt::entity, NumericComparator>&
    getNamedEntities();
    const std::map<std::string, std::string, NumericComparator>& getMetrics();

    // entity factory methods
    void createPrototypes(std::string_view prefab_name,
                          const std::unordered_set<std::string>& prototypeIDs);
    void updatePrototypes(std::string_view prefab_name,
                          const std::unordered_set<std::string>& prototypeIDs);
    void destroyPrototypes(std::string_view prefab_name,
                           const std::unordered_set<std::string>& prototypeIDs);
    EntityFactory::Prototypes
    getPrototypes(std::string_view prefab_name,
                  const std::unordered_set<std::string>& prototypeIDs);
    bool
    containsPrototypes(std::string_view prefab_name,
                       const std::unordered_set<std::string>& prototypeIDs);
    const std::map<std::string, EntityFactory::Prototypes, NumericComparator>&
    getAllPrototypes();
    const std::map<std::string, std::string, NumericComparator>&
    getPrototypesCountByPrefab();
    // does not destroy entt entities, just clears the map
    void clearPrototypes();

  private:
    entt::registry m_registry;
    SceneFactory m_sceneFactory;
    std::set<std::unique_ptr<systems::System>, systems::SystemComparator>
      m_systems;
    std::vector<std::string> m_namedSystems;
    bool dirtySystems{};
};
}