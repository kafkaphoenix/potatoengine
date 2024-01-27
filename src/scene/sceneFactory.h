#pragma once

#include <entt/entt.hpp>

#include "assets/assetsManager.h"
#include "assets/scene.h"
#include "pch.h"
#include "renderer/renderer.h"
#include "scene/entityFactory.h"
#include "utils/numericComparator.h"

namespace potatoengine {

class SceneFactory {
  public:
    SceneFactory();
    entt::entity
    createEntity(std::string_view prefabID, std::string&& prototypeID,
                 entt::registry& registry, std::string&& name, std::optional<std::string> tag = std::nullopt,
                 std::optional<uint32_t> uuid = std::nullopt);
    entt::entity
    cloneEntity(const entt::entity& e, uint32_t uuid, entt::registry& registry,
                std::optional<std::string> name = std::nullopt,
                std::optional<std::string> tag = std::nullopt);
    void removeEntity(entt::entity& e, entt::registry& registry);

    void
    createScene(std::string scene_name, std::string scene_path,
                const std::unique_ptr<assets::AssetsManager>& assets_manager,
                const std::unique_ptr<Renderer>& renderer,
                entt::registry& registry);
    void
    reloadScene(const std::unique_ptr<assets::AssetsManager>& assets_manager,
                const std::unique_ptr<Renderer>& renderer,
                entt::registry& registry, bool reload_prototypes);

    void clearScene(const std::unique_ptr<Renderer>& renderer,
                    entt::registry& registry);

    std::string getActiveScene() const { return m_active_scene; }
    const std::map<std::string, std::string, NumericComparator>&
    getMetrics(entt::registry& registry);
    const std::map<std::string, entt::entity, NumericComparator>&
    getNamedEntities(entt::registry& registry);

    EntityFactory& getEntityFactory() { return m_entityFactory; }

  private:
    std::string m_active_scene{};
    EntityFactory m_entityFactory;

    std::map<std::string, std::string, NumericComparator> m_metrics{};
    std::map<std::string, entt::entity, NumericComparator> m_namedEntities{};
    bool m_dirtyMetrics{};
    bool m_dirtyNamedEntities{};

    void createShaderPrograms(
      const assets::Scene& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager,
      const std::unique_ptr<Renderer>& renderer);
    void createTextures(
      const assets::Scene& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void
    createModels(const assets::Scene& scene,
                 const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void createEntitiesFromPrefabs(
      const assets::Scene& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager,
      const std::unique_ptr<Renderer>& renderer, entt::registry& registry);
    void createChildrenScenes(
      const assets::Scene& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void createSceneEntities(
      const assets::Scene& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager,
      const std::unique_ptr<Renderer>& renderer, entt::registry& registry);
    void createNormalEntities(
      const assets::Scene& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager,
      entt::registry& registry);
    void createLightEntities(
      const assets::Scene& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager,
      entt::registry& registry);
    void createCameraEntities(
      const assets::Scene& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager,
      entt::registry& registry);
    void createSystemEntities(const assets::Scene& scene,
                              entt::registry& registry);
    void createFBOEntities(const assets::Scene& scene, entt::registry& registry,
                           const std::unique_ptr<Renderer>& renderer);
};
}