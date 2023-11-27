#include "potatoengine/scene/sceneLoader.h"

#include "potatoengine/utils/timer.h"
using json = nlohmann::json;

namespace potatoengine {
SceneLoader::SceneLoader(std::weak_ptr<AssetsManager> am) : m_assetsManager(am) {}

void SceneLoader::load(std::string_view id) {
    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    const auto& scene = manager->get<Scene>(id);

    CORE_INFO("Loading assets...");
    Timer timer = Timer();
    loadShaders(scene, manager);
    CORE_INFO("Loading shaders TIME: {}", timer.getSeconds());
    timer.reset();
    loadTextures(scene, manager);
    CORE_INFO("Loading textures TIME: {}", timer.getSeconds());
    timer.reset();
    loadModels(scene, manager);
    CORE_INFO("Loading models TIME: {}", timer.getSeconds());
    timer.reset();
    loadPrefabs(scene, manager);
    CORE_INFO("Loading prefabs TIME: {}", timer.getSeconds());
    timer.reset();
    loadScenes(scene, manager);  // TODO: implement
    CORE_INFO("Loading sub-scenes TIME: {}", timer.getSeconds());
    CORE_INFO("Assets loaded!");
    timer.reset();
    loadEntities(scene, manager);
    CORE_INFO("Loading entities TIME: {}", timer.getSeconds());
    timer.reset();
    loadLights(scene, manager);
    CORE_INFO("Loading lights TIME: {}", timer.getSeconds());
    timer.reset();
    loadCameras(scene, manager);
    CORE_INFO("Loading cameras TIME: {}", timer.getSeconds());
    loadSystems(scene, manager);
    CORE_INFO("Loading systems TIME: {}", timer.getSeconds());
    loadFBOs(scene, manager);
    CORE_INFO("Loading FBOs TIME: {}", timer.getSeconds());
}

void SceneLoader::loadShaders(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [shaderProgram, shaders] : scene->getShaders()) {
        for (const auto& [shader, filepath] : shaders.items()) {
            manager->load<Shader>(shader, filepath);
        }
        m_loadedShaders.emplace(shaderProgram);
    }
}

void SceneLoader::loadTextures(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [texture, options] : scene->getTextures()) {
        bool flipY = options.contains("flip_vertically") ? options.at("flip_vertically").get<bool>() : true;
        bool flipOption = flipY ? Texture::FLIP_VERTICALLY : Texture::DONT_FLIP_VERTICALLY;
        manager->load<Texture>(texture, options.at("filepath").get<std::string>(), options.at("type").get<std::string>(), flipOption);
        m_loadedTextures.emplace(texture);
    }
}

void SceneLoader::loadModels(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [model, filepath] : scene->getModels()) {
        manager->load<Model>(model, filepath);
        m_loadedModels.emplace(model);
    }
}

void SceneLoader::loadPrefabs(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [prefab, options] : scene->getPrefabs()) {
        manager->load<Prefab>(prefab, options.at("filepath").get<std::string>(), options.at("targets").get<std::unordered_set<std::string>>());
        m_loadedPrefabs.emplace(prefab);
    }
}

void SceneLoader::loadScenes(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    // TODO: implement
}

void SceneLoader::loadEntities(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [entity, options] : scene->getEntities()) {
        m_loadedEntities.emplace(entity, options);
    }
}

void SceneLoader::loadLights(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [light, options] : scene->getLights()) {
        m_loadedLights.emplace(light, options);
    }
}

void SceneLoader::loadCameras(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [camera, options] : scene->getCameras()) {
        m_loadedCameras.emplace(camera, options);
    }
}

void SceneLoader::loadSystems(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [system, options] : scene->getSystems()) {
        m_loadedSystems.emplace(system, options);
    }
}

void SceneLoader::loadFBOs(const std::shared_ptr<Scene>& scene, const std::shared_ptr<AssetsManager>& manager) {
    for (const auto& [fbo, options] : scene->getFBOs()) {
        m_loadedFBOs.emplace(fbo, options);
    }
}

}
