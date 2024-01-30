#include "scene/sceneFactory.h"

#include <glm/gtx/string_cast.hpp>
#include <nlohmann/json.hpp>

#include "assets/model.h"
#include "assets/prefab.h"
#include "assets/scene.h"
#include "assets/shader.h"
#include "assets/texture.h"
#include "scene/components/camera/cActiveCamera.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/camera/cDistanceFromCamera.h"
#include "scene/components/camera/serializer.h"
#include "scene/components/core/cDeleted.h"
#include "scene/components/core/cName.h"
#include "scene/components/core/cTag.h"
#include "scene/components/core/cTime.h"
#include "scene/components/core/cUUID.h"
#include "scene/components/graphics/cBody.h"
#include "scene/components/graphics/cFBO.h"
#include "scene/components/graphics/cShaderProgram.h"
#include "scene/components/graphics/cShape.h"
#include "scene/components/graphics/cTexture.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/input/cActiveInput.h"
#include "scene/components/input/cInput.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/terrain/cChunkManager.h"
#include "scene/components/utils/cNoise.h"
#include "scene/components/world/cLight.h"
#include "scene/components/world/cSkybox.h"
#include "utils/timer.h"
#include "utils/uuid.h"

using json = nlohmann::json;
using namespace entt::literals;

namespace potatoengine {

SceneFactory::SceneFactory() : m_entityFactory() {}

entt::entity SceneFactory::createEntity(std::string_view prefab_id,
                                        std::string&& prototypeID,
                                        entt::registry& registry,
                                        std::string&& name,
                                        std::optional<std::string> tag,
                                        std::optional<uint32_t> uuid) {
  UUID _uuid = uuid.has_value() ? UUID(uuid.value()) : UUID();
  std::string _tag = tag.has_value() ? tag.value() : prototypeID;
  entt::entity e = cloneEntity(
    m_entityFactory.getPrototypes(prefab_id, {prototypeID}).at(prototypeID),
    _uuid, registry, std::move(name), std::move(_tag));
  return e;
}

entt::entity SceneFactory::cloneEntity(const entt::entity& e, uint32_t uuid,
                                       entt::registry& registry,
                                       std::optional<std::string> name,
                                       std::optional<std::string> tag) {
  entt::entity cloned = registry.create();

  for (const auto& curr : registry.storage()) {
    if (auto& storage = curr.second; storage.contains(e)) {
      storage.push(cloned, storage.value(e));
      entt::meta_type cType = entt::resolve(storage.type());
      entt::meta_any cData = cType.construct(storage.value(e));
      entt::meta_func triggerEventFunc = cType.func("onComponentCloned"_hs);
      if (triggerEventFunc) {
        triggerEventFunc.invoke({}, e, cData);
      }
    }
  }

  registry.emplace<CUUID>(cloned, uuid);
  if (name.has_value()) {
    registry.emplace<CName>(cloned, std::move(name.value()));
  }
  if (tag.has_value()) {
    registry.emplace<CTag>(cloned, std::move(tag.value()));
  }
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
  return cloned;
}

void SceneFactory::createScene(
  std::string scene_id, std::string scene_path,
  const std::unique_ptr<assets::AssetsManager>& assets_manager,
  const std::unique_ptr<RenderManager>& render_manager, entt::registry& registry) {
  Timer timer;
  ENGINE_INFO("Creating scene...");

  assets::Scene scene = assets::Scene(scene_path);
  createShaderPrograms(scene, assets_manager, render_manager);
  ENGINE_INFO("Shader programs creation TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  createTextures(scene, assets_manager);
  ENGINE_INFO("Textures creation TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  createModels(scene, assets_manager);
  ENGINE_INFO("Models creation TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  createEntitiesFromPrefabs(scene, assets_manager, render_manager, registry);
  ENGINE_INFO("Entities creation TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  createChildrenScenes(scene, assets_manager);
  ENGINE_INFO("Children scenes creation TIME: {:.6f}s", timer.getSeconds());
  timer.reset();
  assets_manager->load<assets::Scene>(scene_id, std::move(scene));
  ENGINE_INFO("Scene {} creation TIME: {:.6f}s", scene_id, timer.getSeconds());
  m_active_scene = std::move(scene_id);

  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

void SceneFactory::reloadScene(
  const std::unique_ptr<assets::AssetsManager>& assets_manager,
  const std::unique_ptr<RenderManager>& render_manager, entt::registry& registry,
  bool reload_prototypes) {
  Timer timer;
  ENGINE_ASSERT(not m_active_scene.empty(), "No scene is active!");
  ENGINE_INFO("Reloading scene {}", m_active_scene);

  const auto& scene = assets_manager->get<assets::Scene>(m_active_scene);
  if (reload_prototypes) {
    registry.clear(); // delete instances and prototypes but reusing them later
    m_entityFactory.clearPrototypes();

    for (const auto& [prefab_name, options] : scene->getPrefabs()) {
      ENGINE_TRACE("Reloading scene prefabs prototypes...");
      const auto& prefab = assets_manager->get<assets::Prefab>(prefab_name);
      m_entityFactory.createPrototypes(
        prefab_name, prefab->getTargetedPrototypes(), registry, assets_manager);
    }
  } else {
    auto to_destroy = registry.view<CUUID>();
    registry.destroy(to_destroy.begin(), to_destroy.end());
  }
  ENGINE_TRACE("Reloading scene entities...");
  createSceneEntities(*scene, assets_manager, render_manager, registry);

  ENGINE_INFO("Scene {} reloading TIME: {:.6f}s", m_active_scene,
              timer.getSeconds());

  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

void SceneFactory::clearScene(const std::unique_ptr<RenderManager>& render_manager,
                              entt::registry& registry) {
  ENGINE_ASSERT(not m_active_scene.empty(), "No scene is active!");
  ENGINE_WARN("Clearing scene {}", m_active_scene);

  registry.clear(); // soft delete / = {};  would delete them completely but
                    // does not invoke signals/mixin methods
  m_entityFactory.clearPrototypes();
  render_manager->clear();
  m_active_scene.clear();
  m_metrics.clear();
  m_namedEntities.clear();
  m_dirtyMetrics = false;
  m_dirtyNamedEntities = false;
}

void SceneFactory::createShaderPrograms(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager,
  const std::unique_ptr<RenderManager>& render_manager) {
  for (const auto& [shader_program, shader_program_data] : scene.getShaders()) {
    for (const auto& [shader_type, filepath] : shader_program_data.items()) {
      assets_manager->load<assets::Shader>(shader_type, filepath);
    } // TODO maybe remove shader as asset?
    render_manager->addShaderProgram(std::string(shader_program), assets_manager);
  }
}

void SceneFactory::createTextures(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [texture, options] : scene.getTextures()) {
    bool flipY = options.contains("flip_vertically")
                   ? options.at("flip_vertically").get<bool>()
                   : true;
    bool flipOption = flipY ? assets::Texture::FLIP_VERTICALLY
                            : assets::Texture::DONT_FLIP_VERTICALLY;
    assets_manager->load<assets::Texture>(
      texture, options.at("filepath").get<std::string>(),
      options.at("type").get<std::string>(), flipOption);
  }
}

void SceneFactory::createModels(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  for (const auto& [model, filepath] : scene.getModels()) {
    assets_manager->load<assets::Model>(model, filepath);
  }
}

void SceneFactory::createSceneEntities(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager,
  const std::unique_ptr<RenderManager>& render_manager, entt::registry& registry) {

  createNormalEntities(scene, assets_manager, registry);
  createLightEntities(scene, assets_manager, registry);
  createCameraEntities(scene, assets_manager, registry);
  createSystemEntities(scene, registry);
  createFBOEntities(scene, registry, render_manager);

  registry.sort<CDistanceFromCamera>(
    [](const CDistanceFromCamera& lhs, const CDistanceFromCamera& rhs) {
      return lhs.distance < rhs.distance;
    });

  registry.sort<CUUID, CDistanceFromCamera>();
}

void SceneFactory::createEntitiesFromPrefabs(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager,
  const std::unique_ptr<RenderManager>& render_manager, entt::registry& registry) {
  for (const auto& [prefab_name, options] : scene.getPrefabs()) {
    auto prefab = assets::Prefab(
      options.at("filepath").get<std::string>(),
      options.at("targeted_prototypes").get<std::vector<std::string>>());
    ENGINE_TRACE("Creating scene prototypes for prefab {}...", prefab_name);
    std::vector<std::string> targetedPrototypes =
      prefab.getTargetedPrototypes();
    assets_manager->load<assets::Prefab>(prefab_name, std::move(prefab));
    m_entityFactory.createPrototypes(prefab_name, targetedPrototypes, registry,
                                     assets_manager);
  }
  ENGINE_TRACE("Creating scene entities...");
  createSceneEntities(scene, assets_manager, render_manager, registry);
}

void SceneFactory::createChildrenScenes(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  // TODO: implement
}

void SceneFactory::createNormalEntities(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager,
  entt::registry& registry) {
  ENGINE_TRACE("Creating scene normal entities...");
  for (const auto& [name, data] : scene.getNormalEntities()) {
    std::string prototype = data.at("prototype").get<std::string>();
    entt::entity e =
      createEntity(data.at("prefab").get<std::string>(), std::string(prototype),
                   registry, std::string(name));
    if (data.contains("options")) {
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        registry.get<CRigidBody>(e).isKinematic =
          options.at("isKinematic").get<bool>();
      }
      if (options.contains("position")) {
        json position = options.at("position");
        registry.get<CTransform>(e).position = {position.at("x").get<float>(),
                                                position.at("y").get<float>(),
                                                position.at("z").get<float>()};
      }
      if (options.contains("rotation")) {
        json rotation = options.at("rotation");
        glm::vec3 rot = {rotation.at("x").get<float>(),
                         rotation.at("y").get<float>(),
                         rotation.at("z").get<float>()};
        registry.get<CTransform>(e).rotate(glm::quat(glm::radians(rot)));
      }
      if (registry.all_of<CCamera, CTransform>(e)) {
        CCamera& cCamera = registry.get<CCamera>(e);
        CTransform& cTransform = registry.get<CTransform>(e);
        deserializeCamera(cCamera, options);
        cCamera.calculateProjection();
        cCamera.calculateView(cTransform.position, cTransform.rotation);
      }
      if (options.contains("isActive")) {
        bool isActiveCamera = options.at("isActive").get<bool>();
        if (isActiveCamera and not registry.all_of<CActiveCamera>(e)) {
          registry.emplace<CActiveCamera>(e);
        } else if (not isActiveCamera and registry.all_of<CActiveCamera>(e)) {
          registry.remove<CActiveCamera>(e);
        }
      }
      if (options.contains("hasInput")) {
        bool hasInput = options.at("hasInput").get<bool>();
        if (hasInput and not registry.all_of<CActiveInput>(e)) {
          registry.emplace<CActiveInput>(e);
        } else if (not hasInput and registry.all_of<CActiveInput>(e)) {
          registry.remove<CActiveInput>(e);
        }
      }
      if (options.contains("inputMode")) {
        CInput& cInput = registry.get<CInput>(e);
        cInput._mode = options.at("inputMode").get<std::string>();
        cInput.setMode();
      }
      if (options.contains("translationSpeed")) {
        registry.get<CInput>(e).translationSpeed =
          options.at("translationSpeed").get<float>();
      }
      if (options.contains("verticalSpeed")) {
        registry.get<CInput>(e).verticalSpeed =
          options.at("verticalSpeed").get<float>();
      }
      if (options.contains("mouseSensitivity")) {
        registry.get<CInput>(e).mouseSensitivity =
          options.at("mouseSensitivity").get<float>();
      }
      if (options.contains("rotationSpeed")) {
        registry.get<CInput>(e).rotationSpeed =
          options.at("rotationSpeed").get<float>();
      }
      if (options.contains("size")) {
        CShape& shape = registry.get<CShape>(e);
        json size = options.at("size");
        glm::vec3 sizeVec = {size.at("x").get<float>(),
                             size.at("y").get<float>(),
                             size.at("z").get<float>()};
        if (shape.size not_eq sizeVec) {
          ENGINE_TRACE("Changing shape size from {0} to {1} for entity {2}",
                       glm::to_string(shape.size), glm::to_string(sizeVec),
                       name);
          shape.size = sizeVec;
          shape.meshes.clear();
          shape.createMesh();
        }
      }
      if (options.contains("repeatTexture")) {
        CShape& cShape = registry.get<CShape>(e);
        bool repeatTexture = options.at("repeatTexture").get<bool>();
        if (cShape.repeatTexture not_eq repeatTexture) {
          ENGINE_TRACE(
            "Changing shape repeatTexture from {0} to {1} for entity {2}",
            cShape.repeatTexture, repeatTexture, name);
          cShape.repeatTexture = repeatTexture;
          cShape.meshes.clear();
          cShape.createMesh();
        }
      }
      if (options.contains("body")) {
        registry.get<CBody>(e).reloadMesh(
          options.at("body").get<std::string>());
      }
      if (options.contains("filepaths")) {
        registry.get<CTexture>(e).reloadTextures(
          options.at("filepaths").get<std::vector<std::string>>());
      }
      if (options.contains("color")) {
        json color = options.at("color");
        registry.get<CTexture>(e).color = {
          color.at("r").get<float>(), color.at("g").get<float>(),
          color.at("b").get<float>(), color.at("a").get<float>()};
      }
      if (options.contains("blendFactor")) {
        registry.get<CTexture>(e).blendFactor =
          options.at("blendFactor").get<float>();
      }
      if (options.contains("reflectivity")) {
        registry.get<CTexture>(e).reflectivity =
          options.at("reflectivity").get<float>();
      }
      if (options.contains("refractiveIndex")) {
        registry.get<CTexture>(e).refractiveIndex =
          options.at("refractiveIndex").get<float>();
      }
      if (options.contains("hasTransparency")) {
        registry.get<CTexture>(e).hasTransparency =
          options.at("hasTransparency").get<bool>();
      }
      if (options.contains("useLighting")) {
        registry.get<CTexture>(e).useLighting =
          options.at("useLighting").get<bool>();
      }
      if (options.contains("useReflection")) {
        registry.get<CTexture>(e).useReflection =
          options.at("useReflection").get<bool>();
      }
      if (options.contains("useRefraction")) {
        registry.get<CTexture>(e).useRefraction =
          options.at("useRefraction").get<bool>();
      }
      if (options.contains("isVisible")) {
        registry.get<CShaderProgram>(e).isVisible =
          options.at("isVisible").get<bool>();
      }
      if (options.contains("drawMode")) {
        registry.get<CTexture>(e)._drawMode =
          options.at("drawMode").get<std::string>();
        registry.get<CTexture>(e).setDrawMode();
      }
      if (options.contains("textureAtlas")) {
        CTextureAtlas& cTextureAtlas = registry.get<CTextureAtlas>(e);
        json textureAtlasOptions = options.at("textureAtlas");
        if (textureAtlasOptions.contains("index")) {
          cTextureAtlas.index = textureAtlasOptions.at("index").get<int>();
        }
        if (textureAtlasOptions.contains("rows")) {
          cTextureAtlas.rows = textureAtlasOptions.at("rows").get<int>();
        }
      }
      if (options.contains("scale")) {
        json scale = options.at("scale");
        registry.get<CTransform>(e).scale = {scale.at("x").get<float>(),
                                             scale.at("y").get<float>(),
                                             scale.at("z").get<float>()};
      }
      if (prototype == "chunk_config") {
        CChunkManager& cChunkManager = registry.get<CChunkManager>(e);
        if (options.contains("chunkSize")) {
          cChunkManager.chunkSize = options.at("chunkSize").get<int>();
        }
        if (options.contains("blockSize")) {
          cChunkManager.blockSize = options.at("blockSize").get<int>();
        }
        if (options.contains("width")) {
          cChunkManager.width = options.at("width").get<int>();
        }
        if (options.contains("height")) {
          cChunkManager.height = options.at("height").get<int>();
        }
        if (options.contains("meshType")) {
          cChunkManager._meshType = options.at("meshType").get<std::string>();
          cChunkManager.setMeshType();
        }
        if (options.contains("meshAlgorithm")) {
          cChunkManager._meshAlgorithm =
            options.at("meshAlgorithm").get<std::string>();
          cChunkManager.setMeshAlgorithm();
        }
        if (options.contains("useBiomes")) {
          cChunkManager.useBiomes = options.at("useBiomes").get<bool>();
        }
      }
      if (options.contains("noise")) {
        CNoise& noise = registry.get<CNoise>(e);
        json noiseOptions = options.at("noise");
        if (noiseOptions.contains("type")) {
          noise._type = noiseOptions.at("type").get<std::string>();
          noise.setNoiseType();
        }
        if (noiseOptions.contains("seed")) {
          noise.seed = noiseOptions.at("seed").get<int>();
          noise.setSeed();
        }
        if (noiseOptions.contains("octaves")) {
          noise.octaves = noiseOptions.at("octaves").get<int>();
          noise.setOctaves();
        }
        if (noiseOptions.contains("frequency")) {
          noise.frequency = noiseOptions.at("frequency").get<float>();
          noise.setFrequency();
        }
        if (noiseOptions.contains("persistence")) {
          noise.persistence = noiseOptions.at("persistence").get<float>();
          noise.setPersistence();
        }
        if (noiseOptions.contains("lacunarity")) {
          noise.lacunarity = noiseOptions.at("lacunarity").get<float>();
          noise.setLacunarity();
        }
        if (noiseOptions.contains("amplitude")) {
          noise.amplitude = noiseOptions.at("amplitude").get<int>();
        }
        if (noiseOptions.contains("positive")) {
          noise.positive = noiseOptions.at("positive").get<bool>();
        }
      }
      if (prototype == "skybox") {
        CTime& cTime = registry.get<CTime>(e);
        if (options.contains("time")) {
          cTime.setTime(options.at("time").get<float>());
        }
        if (options.contains("acceleration")) {
          cTime.acceleration = options.at("acceleration").get<float>();
        }
        if (options.contains("useFog")) {
          registry.get<CSkybox>(e).useFog = options.at("useFog").get<bool>();
        }
        if (options.contains("fogColor")) {
          json fogColor = options.at("fogColor");
          registry.get<CSkybox>(e).fogColor = {fogColor.at("r").get<float>(),
                                               fogColor.at("g").get<float>(),
                                               fogColor.at("b").get<float>()};
        }
        if (options.contains("fogDensity")) {
          registry.get<CSkybox>(e).fogDensity =
            options.at("fogDensity").get<float>();
        }
        if (options.contains("fogGradient")) {
          registry.get<CSkybox>(e).fogGradient =
            options.at("fogGradient").get<float>();
        }
      }
    }
  }
}

void SceneFactory::createLightEntities(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager,
  entt::registry& registry) {
  ENGINE_TRACE("Creating scene light entities...");
  for (const auto& [name, data] : scene.getLightEntities()) {
    entt::entity e = createEntity(data.at("prefab").get<std::string>(),
                                  data.at("prototype").get<std::string>(),
                                  registry, std::string(name));
    if (data.contains("options")) {
      CTransform& cTransform = registry.get<CTransform>(e);
      CLight& cLight = registry.get<CLight>(e);
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        registry.get<CRigidBody>(e).isKinematic =
          options.at("isKinematic").get<bool>();
      }
      if (options.contains("position")) {
        json position = options.at("position");
        cTransform.position = {position.at("x").get<float>(),
                               position.at("y").get<float>(),
                               position.at("z").get<float>()};
      }
      if (options.contains("rotation")) {
        json rotation = options.at("rotation");
        glm::vec3 rot = {rotation.at("x").get<float>(),
                         rotation.at("y").get<float>(),
                         rotation.at("z").get<float>()};
        cTransform.rotate(glm::quat(glm::radians(rot)));
      }
      if (options.contains("scale")) {
        json scale = options.at("scale");
        cTransform.scale = {scale.at("x").get<float>(),
                            scale.at("y").get<float>(),
                            scale.at("z").get<float>()};
      }
      if (options.contains("intensity")) {
        cLight.intensity = options.at("intensity").get<float>();
      }
      if (options.contains("color")) {
        json color = options.at("color");
        cLight.color = {color.at("r").get<float>(), color.at("g").get<float>(),
                        color.at("b").get<float>()};
      }
      if (options.contains("isVisible")) {
        registry.get<CShaderProgram>(e).isVisible =
          options.at("isVisible").get<bool>();
      }
      if (options.contains("body")) {
        registry.get<CBody>(e).reloadMesh(
          options.at("body").get<std::string>());
      }
    }
  }
}

void SceneFactory::createCameraEntities(
  const assets::Scene& scene,
  const std::unique_ptr<assets::AssetsManager>& assets_manager,
  entt::registry& registry) {
  ENGINE_TRACE("Creating scene camera entities...");
  for (const auto& [name, data] : scene.getCameraEntities()) {
    entt::entity e = createEntity(data.at("prefab").get<std::string>(),
                                  data.at("prototype").get<std::string>(),
                                  registry, std::string(name));
    if (data.contains("options")) {
      CCamera& cCamera = registry.get<CCamera>(e);
      CTransform& cTransform = registry.get<CTransform>(e);
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        registry.get<CRigidBody>(e).isKinematic =
          options.at("isKinematic").get<bool>();
      }
      if (options.contains("position")) {
        json position = options.at("position");
        cTransform.position = {position.at("x").get<float>(),
                               position.at("y").get<float>(),
                               position.at("z").get<float>()};
      }
      if (options.contains("rotation")) {
        json rotation = options.at("rotation");
        glm::vec3 rot = {rotation.at("x").get<float>(),
                         rotation.at("y").get<float>(),
                         rotation.at("z").get<float>()};
        cTransform.rotate(glm::quat(glm::radians(
          rot))); // TODO: fix this will glitch with camera movement
      }
      deserializeCamera(cCamera, options);
      cCamera.calculateProjection();
      if (options.contains("isActive")) {
        bool isActiveCamera = options.at("isActive").get<bool>();
        if (isActiveCamera and not registry.all_of<CActiveCamera>(e)) {
          registry.emplace<CActiveCamera>(e);
        } else if (not isActiveCamera and registry.all_of<CActiveCamera>(e)) {
          registry.remove<CActiveCamera>(e);
        }
      }
      if (options.contains("hasInput")) {
        bool hasInput = options.at("hasInput").get<bool>();
        if (hasInput and not registry.all_of<CActiveInput>(e)) {
          registry.emplace<CActiveInput>(e);
        } else if (not hasInput and registry.all_of<CActiveInput>(e)) {
          registry.remove<CActiveInput>(e);
        }
      }
      if (options.contains("inputMode")) {
        CInput& cInput = registry.get<CInput>(e);
        cInput._mode = options.at("inputMode").get<std::string>();
        cInput.setMode();
      }
      if (options.contains("translationSpeed")) {
        registry.get<CInput>(e).translationSpeed =
          options.at("translationSpeed").get<float>();
      }
      if (options.contains("verticalSpeed")) {
        registry.get<CInput>(e).verticalSpeed =
          options.at("verticalSpeed").get<float>();
      }
      if (options.contains("mouseSensitivity")) {
        registry.get<CInput>(e).mouseSensitivity =
          options.at("mouseSensitivity").get<float>();
      }
      if (options.contains("rotationSpeed")) {
        registry.get<CInput>(e).rotationSpeed =
          options.at("rotationSpeed").get<float>();
      }
      if (options.contains("isVisible")) {
        registry.get<CShaderProgram>(e).isVisible =
          options.at("isVisible").get<bool>();
      }
      if (options.contains("body")) {
        registry.get<CBody>(e).reloadMesh(
          options.at("body").get<std::string>());
      }
    }
  }
}

void SceneFactory::createSystemEntities(const assets::Scene& scene,
                                        entt::registry& registry) {
  ENGINE_TRACE("Creating scene system entities...");
  for (const auto& [name, data] : scene.getSystemEntities()) {
    entt::entity e = createEntity(data.at("prefab").get<std::string>(),
                                  data.at("prototype").get<std::string>(),
                                  registry, std::string(name));
    if (data.contains("options")) {
      json options = data.at("options");
      // TODO: implement
    }
  }
}

void SceneFactory::createFBOEntities(
  const assets::Scene& scene, entt::registry& registry,
  const std::unique_ptr<RenderManager>& render_manager) {
  ENGINE_TRACE("Creating scene FBO entities...");
  for (const auto& [name, data] : scene.getFBOEntities()) {
    entt::entity e = createEntity(data.at("prefab").get<std::string>(),
                                  data.at("prototype").get<std::string>(),
                                  registry, std::string(name));
    CFBO& fbo = registry.get<CFBO>(e);
    if (data.contains("options")) {
      json options = data.at("options");
      if (options.contains("width")) {
        fbo.width = options.at("width").get<int>();
      }
      if (options.contains("height")) {
        fbo.height = options.at("height").get<int>();
      }
      if (options.contains("attachment")) {
        fbo._attachment =
          std::move(options.at("attachment").get<std::string>());
        fbo.setAttachment();
      }
      if (options.contains("mode")) {
        fbo._mode = std::move(options.at("mode").get<std::string>());
        fbo.setMode();
      }
    }
    render_manager->addFramebuffer(std::string(fbo.fbo), fbo.width, fbo.height,
                             fbo.attachment);
  }
}

void SceneFactory::removeEntity(entt::entity& e, entt::registry& registry) {
  registry.emplace<CDeleted>(e);
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

const std::map<std::string, std::string, NumericComparator>&
SceneFactory::getMetrics(entt::registry& registry) {
  if (not m_dirtyMetrics) {
    return m_metrics;
  }

  m_metrics.clear();
  m_metrics["Active scene"] = m_active_scene;
  int total = registry.storage<entt::entity>().in_use();
  int created = registry.storage<entt::entity>().size();
  int prototypes = 0;
  for (const auto& [key, prototypesMap] : m_entityFactory.getAllPrototypes()) {
    prototypes += prototypesMap.size();
    m_metrics["Prefab " + key + " Prototypes Alive"] =
      std::to_string(prototypesMap.size());
  }
  m_metrics["Prototypes Total Alive"] = std::to_string(prototypes);
  m_metrics["Instances Total Alive"] = std::to_string(total - prototypes);
  m_metrics["Entities Total Alive"] = std::to_string(total);
  m_metrics["Entities Total Created"] = std::to_string(created);
  m_metrics["Entities Total Released"] = std::to_string(created - total);
  m_dirtyMetrics = false;

  return m_metrics;
}

const std::map<std::string, entt::entity, NumericComparator>&
SceneFactory::getNamedEntities(entt::registry& registry) {
  if (not m_dirtyNamedEntities) {
    return m_namedEntities;
  }

  m_namedEntities.clear();
  registry.view<CName, CUUID>().each(
    [&](entt::entity e, const CName& cName, const CUUID& cUUID) {
      m_namedEntities.emplace(cName.name, e);
    });
  m_dirtyNamedEntities = false;

  return m_namedEntities;
}

}