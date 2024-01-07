#pragma once

#include <entt/entt.hpp>

#include "assets/assetsManager.h"
#include "core/time.h"
#include "events/event.h"
#include "renderer/renderer.h"
#include "scene/entityFactory.h"
#include "scene/sceneLoader.h"
#include "utils/numericComparator.h"
#include "utils/uuid.h"

namespace potatoengine {
class Entity;

class SceneManager {
  public:
    SceneManager();
    void onEvent(Event& e);
    void onUpdate(const Time& ts);
    void print();
    const std::map<std::string, std::string, NumericComparator>& getMetrics();
    entt::registry& getRegistry() noexcept { return m_registry; }

    // Entity is a wrapper around entt::entity without persistency
    Entity createEntity(std::string_view prefabID, std::string_view prototypeID,
                        const std::optional<uint32_t>& uuid = std::nullopt);
    Entity cloneEntity(Entity&& e, uint32_t uuid);
    void removeEntity(Entity&& e);
    Entity getEntity(std::string_view name);
    Entity getEntity(UUID& uuid);
    const std::map<std::string, entt::entity, NumericComparator>&
    getAllNamedEntities();

    void createPrototypes(std::string_view prefabID);
    void updatePrototypes(std::string_view prefabID);
    void destroyPrototypes(std::string_view prefabID);
    const EntityFactory::Prototypes& getPrototypes(std::string_view prefabID);
    const std::map<std::string, EntityFactory::Prototypes, NumericComparator>&
    getAllPrototypes();
    bool containsPrototypes(std::string_view prefabID) const;
    void createPrototype(std::string_view prefabID,
                         std::string_view prototypeID);
    void updatePrototype(std::string_view prefabID,
                         std::string_view prototypeID);
    void destroyPrototype(std::string_view prefabID,
                          std::string_view prototypeID);
    Entity getPrototype(std::string_view prefabID,
                        std::string_view prototypeID);
    bool containsPrototype(std::string_view prefabID,
                           std::string_view prototypeID) const;

    // TODO: move to scene creator if I fix the circular dependency
    void loadScene(std::string_view sceneID, const std::unique_ptr<AssetsManager>& asset_manager);
    void createScene(std::string sceneID, const std::unique_ptr<AssetsManager>& asset_manager,
                     const std::unique_ptr<Renderer>& renderer, bool reload);
    void createEntities(std::string_view prefabID,
                        const SceneLoader& loadedScene,
                        const std::unique_ptr<AssetsManager>& asset_manager,
                        const std::unique_ptr<Renderer>& renderer);
    void reloadScene(const std::unique_ptr<AssetsManager>& asset_manager,
                     const std::unique_ptr<Renderer>& renderer);
    void clearScene(const std::unique_ptr<Renderer>& renderer);
    std::string_view getActiveScene() const noexcept { return m_activeScene; }

    template <typename T> void onComponentAdded(Entity& e, T& c);

    template <typename T> void onComponentCloned(Entity& e, T& c);

    static std::unique_ptr<SceneManager> Create();

  private:
    EntityFactory m_entityFactory{};
    entt::registry m_registry;
    std::string m_activeScene{};
    std::unordered_map<std::string, SceneLoader> m_loadedScenes{};

    std::map<std::string, std::string, NumericComparator> m_metrics{};
    std::map<std::string, entt::entity, NumericComparator> m_namedEntities{};
    bool m_dirty{};

    friend class Entity;
};
}