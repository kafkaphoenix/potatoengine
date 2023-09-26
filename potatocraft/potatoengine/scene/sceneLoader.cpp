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

#ifdef DEBUG
    CORE_INFO("Loading assets...");
    Timer timer = Timer();
#endif
    loadShaders(scene, manager);
#ifdef DEBUG
    CORE_INFO("Loading shaders TIME: {0}", timer.getSeconds());
    timer.reset();
#endif
    loadTextures(scene, manager);
#ifdef DEBUG
    CORE_INFO("Loading textures TIME: {0}", timer.getSeconds());
    timer.reset();
#endif
    loadModels(scene, manager);
#ifdef DEBUG
    CORE_INFO("Loading models TIME: {0}", timer.getSeconds());
    timer.reset();
#endif
    loadPrefabs(scene, manager);
#ifdef DEBUG
    CORE_INFO("Loading prefabs TIME: {0}", timer.getSeconds());
    timer.reset();
#endif
    loadScenes(scene, manager);  // TODO: implement
#ifdef DEBUG
    CORE_INFO("Loading scenes TIME: {0}", timer.getSeconds());
    CORE_INFO("Assets loaded!");
    timer.reset();
#endif
    loadEntities(scene, manager);
#ifdef DEBUG
    CORE_INFO("Loading entities TIME: {0}", timer.getSeconds());
    timer.reset();
#endif
    loadLights(scene, manager);
#ifdef DEBUG
    CORE_INFO("Loading lights TIME: {0}", timer.getSeconds());
    timer.reset();
#endif
    loadCameras(scene, manager);
#ifdef DEBUG
    CORE_INFO("Loading cameras TIME: {0}", timer.getSeconds());
#endif
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

}
