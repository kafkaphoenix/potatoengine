#pragma once

#include <nlohmann/json.hpp>

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/renderer.h"

namespace potatoengine {

class SceneLoader {
   public:
    SceneLoader(std::weak_ptr<AssetsManager> am);
    void load(std::string_view id);
    const std::unordered_set<std::string>& getLoadedPrefabs() const noexcept { return m_loadedPrefabs; }
    const std::unordered_set<std::string>& getLoadedShaders() const noexcept { return m_loadedShaders; }
    const std::unordered_map<std::string, json>& getLoadedEntities() const noexcept { return m_loadedEntities; }
    const std::unordered_map<std::string, json>& getLoadedLights() const noexcept { return m_loadedLights; }
    const std::unordered_map<std::string, json>& getLoadedCameras() const noexcept { return m_loadedCameras; }

   private:
    std::weak_ptr<AssetsManager> m_assetsManager;
    std::unordered_set<std::string> m_loadedShaders;
    std::unordered_set<std::string> m_loadedTextures;
    std::unordered_set<std::string> m_loadedModels;
    std::unordered_set<std::string> m_loadedPrefabs;
    std::unordered_set<std::string> m_loadedScenes;

    std::unordered_map<std::string, json> m_loadedEntities;
    std::unordered_map<std::string, json> m_loadedLights;
    std::unordered_map<std::string, json> m_loadedCameras;

    void loadShaders(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager);
    void loadTextures(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager);
    void loadModels(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager);
    void loadPrefabs(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager);
    void loadScenes(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager);

    void loadEntities(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager);
    void loadLights(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager);
    void loadCameras(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager);
};
}