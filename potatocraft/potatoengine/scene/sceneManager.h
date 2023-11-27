#pragma once

#include <entt/entt.hpp>

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/core/time.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/scene/entityFactory.h"
#include "potatoengine/scene/sceneLoader.h"
#include "potatoengine/utils/uuid.h"
#include "potatoengine/events/event.h"

namespace potatoengine {
class Entity;

class SceneManager {
   public:
    SceneManager(std::weak_ptr<AssetsManager> am, std::weak_ptr<Renderer> r);
    Entity createEntity(std::string_view id, const std::optional<uint32_t>& uuid = std::nullopt);
    Entity cloneEntity(Entity e, uint32_t uuid);
    void clearEntity(Entity& e);
    void removeEntity(Entity& e);
    void print();
    void onEvent(Event& e);
    void onUpdate(Time ts, std::weak_ptr<Renderer> r);
    Entity getEntity(std::string_view name);
    Entity getEntity(UUID& uuid);
    void createPrefab(std::string_view id);
    void updatePrefab(std::string_view id);
    void loadScene(std::string_view id);
    void createScene(std::string id);
    void updateScene(std::string_view id);
    void clearScene(std::string_view id);
    std::string_view getActiveScene() const noexcept { return m_activeScene; }
    entt::registry& getRegistry() noexcept { return m_registry; }

    template <typename T>
    void onComponentAdded(Entity e, T& c);

    template <typename T>
    void onComponentCloned(Entity e, T& c);

   private:
    std::weak_ptr<AssetsManager> m_assetsManager;
    std::weak_ptr<Renderer> m_renderer;
    EntityFactory m_entityFactory;
    entt::registry m_registry;
    std::string m_activeScene{};
    std::unordered_map<std::string, SceneLoader> m_loadedScenes{};

    friend class Entity;
};
}