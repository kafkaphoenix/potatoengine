#include "scene/sceneLoader.h"

#include "assets/model.h"
#include "assets/prefab.h"
#include "assets/shader.h"
#include "assets/texture.h"
#include "utils/timer.h"

using json = nlohmann::json;

namespace potatoengine {

void SceneLoader::load(
  std::string_view sceneID,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  const auto& scene = assets_manager->get<assets::Scene>(sceneID);

  ENGINE_INFO("Loading assets...");
  Timer timer = Timer();
  loadShaders(scene, assets_manager);
  ENGINE_INFO("Loading shaders TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  loadTextures(scene, assets_manager);
  ENGINE_INFO("Loading textures TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  loadModels(scene, assets_manager);
  ENGINE_INFO("Loading models TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  loadPrefabs(scene, assets_manager);
  ENGINE_INFO("Loading prefabs TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  loadChildrenScenes(scene, assets_manager); // TODO: implement
  ENGINE_INFO("Loading children scenes TIME: {:.6f}s", timer.getSeconds());
  ENGINE_INFO("Assets loaded!");

  ENGINE_INFO("Loading entities...");
  timer.reset();
  loadNormalEntities(scene, assets_manager);
  ENGINE_INFO("Loading normal entities TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  loadLightEntities(scene, assets_manager);
  ENGINE_INFO("Loading light entities TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  loadCameraEntities(scene, assets_manager);
  ENGINE_INFO("Loading camera entities TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  loadSystemEntities(scene, assets_manager);
  ENGINE_INFO("Loading system entities TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  loadFBOEntities(scene, assets_manager);
  ENGINE_INFO("Loading FBO entities TIME: {:.6f}s", timer.getSeconds());
  ENGINE_INFO("Entities loaded!");
}

void SceneLoader::loadShaders(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [shaderProgram, shaderprogram_data] : scene->getShaders()) {
    for (const auto& [shader, filepath] : shaderprogram_data.items()) {
      assets_manager->load<assets::Shader>(shader, filepath);
    }
    m_loadedShaders.emplace(shaderProgram);
  }
}

void SceneLoader::loadTextures(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [texture, options] : scene->getTextures()) {
    bool flipY = options.contains("flip_vertically")
                   ? options.at("flip_vertically").get<bool>()
                   : true;
    bool flipOption = flipY ? assets::Texture::FLIP_VERTICALLY
                            : assets::Texture::DONT_FLIP_VERTICALLY;
    assets_manager->load<assets::Texture>(
      texture, options.at("filepath").get<std::string>(),
      options.at("type").get<std::string>(), flipOption);
    m_loadedTextures.emplace(texture);
  }
}

void SceneLoader::loadModels(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [model, filepath] : scene->getModels()) {
    assets_manager->load<assets::Model>(model, filepath);
    m_loadedModels.emplace(model);
  }
}

void SceneLoader::loadPrefabs(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [prefab, options] : scene->getPrefabs()) {
    assets_manager->load<assets::Prefab>(
      prefab, options.at("filepath").get<std::string>(),
      options.at("targeted_prototypes").get<std::unordered_set<std::string>>());
    m_loadedPrefabs.emplace(prefab);
    m_loadedNormalEntities.emplace(prefab,
                                   std::unordered_map<std::string, json>());
    m_loadedLightEntities.emplace(prefab,
                                  std::unordered_map<std::string, json>());
    m_loadedCameraEntities.emplace(prefab,
                                   std::unordered_map<std::string, json>());
    m_loadedSystemEntities.emplace(prefab,
                                   std::unordered_map<std::string, json>());
    m_loadedFBOEntities.emplace(prefab,
                                std::unordered_map<std::string, json>());
  }
}

void SceneLoader::loadChildrenScenes(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  // TODO: implement
}

void SceneLoader::loadNormalEntities(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [entity, options] : scene->getNormalEntities()) {
    m_loadedNormalEntities.at(options.at("prefab").get<std::string>())
      .emplace(entity, options);
  }
}

void SceneLoader::loadLightEntities(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [light, options] : scene->getLightEntities()) {
    m_loadedLightEntities.at(options.at("prefab").get<std::string>())
      .emplace(light, options);
  }
}

void SceneLoader::loadCameraEntities(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [camera, options] : scene->getCameraEntities()) {
    m_loadedCameraEntities.at(options.at("prefab").get<std::string>())
      .emplace(camera, options);
  }
}

void SceneLoader::loadSystemEntities(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [system, options] : scene->getSystemEntities()) {
    m_loadedSystemEntities.at(options.at("prefab").get<std::string>())
      .emplace(system, options);
  }
}

void SceneLoader::loadFBOEntities(
  const std::shared_ptr<assets::Scene>& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [fbo, options] : scene->getFBOEntities()) {
    m_loadedFBOEntities.at(options.at("prefab").get<std::string>())
      .emplace(fbo, options);
  }
}

}
