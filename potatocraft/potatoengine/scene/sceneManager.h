#pragma once

#include <entt/entt.hpp>

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/core/time.h"
#include "potatoengine/events/event.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/scene/entityFactory.h"
#include "potatoengine/scene/sceneLoader.h"
#include "potatoengine/utils/numericComparator.h"
#include "potatoengine/utils/uuid.h"

namespace potatoengine {
class Entity;

class SceneManager {
   public:
    SceneManager(std::weak_ptr<AssetsManager> am, std::weak_ptr<Renderer> r);
    void onEvent(Event& e);
    void onUpdate(Time ts, std::weak_ptr<Renderer> r);
    void print();
    const std::map<std::string, std::string>& getMetrics();
    entt::registry& getRegistry() noexcept { return m_registry; }

    // Entity is a wrapper around entt::entity without persistency
    Entity createEntity(std::string_view prefabID, std::string_view prototypeID, const std::optional<uint32_t>& uuid = std::nullopt);
    Entity cloneEntity(Entity&& e, uint32_t uuid);
    void removeEntity(Entity&& e);
    Entity getEntity(std::string_view name);
    Entity getEntity(UUID& uuid);
    const std::map<std::string, entt::entity, NumericComparator>& getAllNamedEntities();

    void createPrototypes(std::string_view prefabID);
    void updatePrototypes(std::string_view prefabID);
    void destroyPrototypes(std::string_view prefabID);
    const EntityFactory::Prototypes& getPrototypes(std::string_view prefabID);
    const std::map<std::string, EntityFactory::Prototypes, NumericComparator>& getAllPrototypes();
    bool containsPrototypes(std::string_view prefabID) const;
    void createPrototype(std::string_view prefabID, std::string_view prototypeID);
    void updatePrototype(std::string_view prefabID, std::string_view prototypeID);
    void destroyPrototype(std::string_view prefabID, std::string_view prototypeID);
    Entity getPrototype(std::string_view prefabID, std::string_view prototypeID);
    bool containsPrototype(std::string_view prefabID, std::string_view prototypeID) const;

    // TODO: move to scene creator if I fix the circular dependency
    void loadScene(std::string_view sceneID);
    void createScene(std::string sceneID, bool reload = false);
    void createEntities(std::string_view prefabID, const SceneLoader& loadedScene, const std::shared_ptr<AssetsManager>& am, const std::shared_ptr<Renderer>& r);
    void reloadScene();
    void clearScene();
    std::string_view getActiveScene() const noexcept { return m_activeScene; }

    template <typename T>
    void onComponentAdded(Entity& e, T& c);

    template <typename T>
    void onComponentCloned(Entity& e, T& c);

   private:
    std::weak_ptr<AssetsManager> m_assetsManager;
    std::weak_ptr<Renderer> m_renderer;
    EntityFactory m_entityFactory;
    entt::registry m_registry;
    std::string m_activeScene{};
    std::unordered_map<std::string, SceneLoader> m_loadedScenes{};

    std::map<std::string, std::string> m_metrics{};
    std::map<std::string, entt::entity, NumericComparator> m_namedEntities{};
    bool m_isDirty{false};

    friend class Entity;
};
}