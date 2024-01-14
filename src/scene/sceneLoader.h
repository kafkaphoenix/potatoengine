#pragma once

#include <nlohmann/json.hpp>

#include "assets/assetsManager.h"
#include "assets/scene.h"
#include "pch.h"

using json = nlohmann::json;

namespace potatoengine {

class SceneLoader {
  public:
    void load(std::string_view sceneID,
              const std::unique_ptr<assets::AssetsManager>& assets_manager);

    const std::unordered_set<std::string>& getLoadedShaders() const noexcept {
      return m_loadedShaders;
    }
    const std::unordered_set<std::string>& getLoadedTextures() const noexcept {
      return m_loadedTextures;
    }
    const std::unordered_set<std::string>& getLoadedModels() const noexcept {
      return m_loadedModels;
    }
    const std::unordered_set<std::string>& getLoadedPrefabs() const noexcept {
      return m_loadedPrefabs;
    }
    const std::unordered_set<std::string>&
    getLoadedChildrenScenes() const noexcept {
      return m_loadedChildrenScenes;
    }

    const std::unordered_map<std::string, json>&
    getLoadedNormalEntities(std::string_view prefabID) const {
      return m_loadedNormalEntities.at(prefabID.data());
    }
    const std::unordered_map<std::string, json>&
    getLoadedLightEntities(std::string_view prefabID) const {
      return m_loadedLightEntities.at(prefabID.data());
    }
    const std::unordered_map<std::string, json>&
    getLoadedCameraEntities(std::string_view prefabID) const {
      return m_loadedCameraEntities.at(prefabID.data());
    }
    const std::unordered_map<std::string, json>&
    getLoadedSystemEntities(std::string_view prefabID) const {
      return m_loadedSystemEntities.at(prefabID.data());
    }
    const std::unordered_map<std::string, json>&
    getLoadedFBOEntities(std::string_view prefabID) const {
      return m_loadedFBOEntities.at(prefabID.data());
    }

  private:
    std::unordered_set<std::string> m_loadedShaders;
    std::unordered_set<std::string> m_loadedTextures;
    std::unordered_set<std::string> m_loadedModels;
    std::unordered_set<std::string> m_loadedPrefabs;
    std::unordered_set<std::string> m_loadedChildrenScenes;

    std::unordered_map<std::string, std::unordered_map<std::string, json>>
      m_loadedNormalEntities;
    std::unordered_map<std::string, std::unordered_map<std::string, json>>
      m_loadedLightEntities;
    std::unordered_map<std::string, std::unordered_map<std::string, json>>
      m_loadedCameraEntities;
    std::unordered_map<std::string, std::unordered_map<std::string, json>>
      m_loadedSystemEntities;
    std::unordered_map<std::string, std::unordered_map<std::string, json>>
      m_loadedFBOEntities;

    void
    loadShaders(const std::shared_ptr<assets::Scene>& scene,
                const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void
    loadTextures(const std::shared_ptr<assets::Scene>& scene,
                 const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void
    loadModels(const std::shared_ptr<assets::Scene>& scene,
               const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void
    loadPrefabs(const std::shared_ptr<assets::Scene>& scene,
                const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void loadChildrenScenes(
      const std::shared_ptr<assets::Scene>& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);

    void loadNormalEntities(
      const std::shared_ptr<assets::Scene>& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void loadLightEntities(
      const std::shared_ptr<assets::Scene>& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void loadCameraEntities(
      const std::shared_ptr<assets::Scene>& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void loadSystemEntities(
      const std::shared_ptr<assets::Scene>& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void loadFBOEntities(
      const std::shared_ptr<assets::Scene>& scene,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
};
}