#include "scene/sceneManager.h"

#include <glm/gtx/string_cast.hpp>

#include "core/application.h"
#include "scene/components/camera/cActiveCamera.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/camera/serializer.h"
#include "scene/components/common/cName.h"
#include "scene/components/common/cTag.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/graphics/cFBO.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/input/cActiveInput.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/utils/cDeleted.h"
#include "scene/components/utils/cNoise.h"
#include "scene/components/world/cChunkManager.h"
#include "scene/components/world/cLight.h"
#include "scene/components/world/cSkybox.h"
#include "scene/components/world/cTime.h"
#include "scene/systems/sEvent.h"
#include "scene/systems/sUpdate.h"
#include "scene/utils.h"
#include "utils/timer.h"

using json = nlohmann::json;
using namespace entt::literals;

namespace potatoengine {

SceneManager::SceneManager() {
  ENGINE_TRACE("Initializing scene manager...");
  ENGINE_TRACE("Registering engine components...");
  RegisterComponents();
  ENGINE_TRACE("Scene manager created!");
}

void SceneManager::onEvent(Event& e) { eventSystem(m_registry, e); }

void SceneManager::onUpdate(const Time& ts) { updateSystem(m_registry, ts); }

const std::map<std::string, std::string, NumericComparator>&
SceneManager::getMetrics() {
  if (not m_dirtyMetrics) {
    return m_metrics;
  }

  m_metrics.clear();
  m_metrics["Active scene"] = m_activeScene;
  int total = m_registry.storage<entt::entity>().in_use();
  int created = m_registry.storage<entt::entity>().size();
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

entt::entity SceneManager::createEntity(std::string_view prefabID,
                                        std::string_view prototypeID,
                                        const std::optional<uint32_t>& uuid) {
  UUID _uuid = uuid.has_value() ? UUID(uuid.value()) : UUID();
  entt::entity e = cloneEntity(getPrototype(prefabID, prototypeID), _uuid);
  return e;
}

entt::entity SceneManager::cloneEntity(const entt::entity& e, uint32_t uuid) {
  entt::entity cloned = m_registry.create();

  for (const auto& curr : m_registry.storage()) {
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

  m_registry.emplace<CUUID>(cloned,
                            uuid); // TODO if not uuid provided get from entity
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
  return cloned;
}

void SceneManager::removeEntity(entt::entity& e) {
  m_registry.emplace<CDeleted>(e);
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

entt::entity SceneManager::getEntity(std::string_view name) {
  for (const auto& [e, cName, _] : m_registry.view<CName, CUUID>().each()) {
    if (cName.name == name) {
      return e;
    }
  }
  ENGINE_ASSERT(false, "Entity not found");
}

entt::entity SceneManager::getEntity(UUID& uuid) {
  for (const auto& [e, cUUID] : m_registry.view<CUUID>().each()) {
    if (cUUID.uuid == uuid) {
      return e;
    }
  }
  ENGINE_ASSERT(false, "Entity not found");
}

const std::map<std::string, entt::entity, NumericComparator>&
SceneManager::getAllNamedEntities() {
  if (not m_dirtyNamedEntities) {
    return m_namedEntities;
  }

  m_namedEntities.clear();
  m_registry.view<CName, CUUID>().each(
    [&](entt::entity e, const CName& cName, const CUUID& cUUID) {
      m_namedEntities.emplace(cName.name, e);
    });
  m_dirtyNamedEntities = false;

  return m_namedEntities;
}

void SceneManager::createPrototypes(std::string_view prefabID) {
  m_entityFactory.createPrototypes(prefabID, m_registry);
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

void SceneManager::updatePrototypes(std::string_view prefabID) {
  m_entityFactory.updatePrototypes(prefabID, m_registry);
}

void SceneManager::destroyPrototypes(std::string_view prefabID) {
  m_entityFactory.destroyPrototypes(prefabID, m_registry);
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

const EntityFactory::Prototypes&
SceneManager::getPrototypes(std::string_view prefabID) {
  return m_entityFactory.getPrototypes(prefabID);
}

const std::map<std::string, EntityFactory::Prototypes, NumericComparator>&
SceneManager::getAllPrototypes() {
  return m_entityFactory.getAllPrototypes();
}

bool SceneManager::containsPrototypes(std::string_view prefabID) const {
  return m_entityFactory.containsPrototypes(prefabID);
}

void SceneManager::createPrototype(std::string_view prefabID,
                                   std::string_view prototypeID) {
  m_entityFactory.createPrototype(prefabID, prototypeID, m_registry);
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

void SceneManager::updatePrototype(std::string_view prefabID,
                                   std::string_view prototypeID) {
  m_entityFactory.updatePrototype(prefabID, prototypeID, m_registry);
}

void SceneManager::destroyPrototype(std::string_view prefabID,
                                    std::string_view prototypeID) {
  m_entityFactory.destroyPrototype(prefabID, prototypeID, m_registry);
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

entt::entity SceneManager::getPrototype(std::string_view prefabID,
                                        std::string_view prototypeID) {
  return m_entityFactory.getPrototype(prefabID, prototypeID);
}

bool SceneManager::containsPrototype(std::string_view prefabID,
                                     std::string_view prototypeID) const {
  return m_entityFactory.containsPrototype(prefabID, prototypeID);
}

void SceneManager::loadScene(
  std::string_view sceneID,
  const std::unique_ptr<AssetsManager>& assetsManager) {
  ENGINE_TRACE("Loading scene elements...");
  SceneLoader loadedScene;
  loadedScene.load(sceneID, assetsManager);
  m_loadedScenes.emplace(sceneID, std::move(loadedScene));
  ENGINE_TRACE("Scene elements loaded!");
}

void SceneManager::createScene(
  std::string sceneID, const std::unique_ptr<AssetsManager>& assetsManager,
  const std::unique_ptr<Renderer>& renderer, bool reload) {
  Timer timer;
  if (reload) {
    ENGINE_INFO("Reloading scene...");
  } else {
    ENGINE_INFO("Creating scene...");
  }

  ENGINE_ASSERT(m_activeScene.empty() or reload, "Scene {} already exists!",
                sceneID);
  ENGINE_ASSERT(m_loadedScenes.contains(sceneID), "Scene {} is not loaded!",
                sceneID);

  ENGINE_TRACE("Linking scene shaders...");
  const auto& loadedScene = m_loadedScenes.at(sceneID);
  for (std::string id : loadedScene.getLoadedShaders()) {
    renderer->addShaderProgram(std::move(id), assetsManager);
  }

  // A Prefab asset is a json file that contains a list of entity prototypes
  for (std::string_view prefabID : loadedScene.getLoadedPrefabs()) {
    ENGINE_TRACE("Creating scene prefabs prototypes...");
    createPrototypes(prefabID);
    ENGINE_TRACE("Creating scene entities...");
    createEntities(prefabID, loadedScene, assetsManager, renderer);
  }

  if (reload) {
    ENGINE_INFO("Scene {} reloading TIME: {:.6f}s", m_activeScene,
                timer.getSeconds());
  } else {
    ENGINE_INFO("Scene {} creation TIME: {:.6f}s", sceneID, timer.getSeconds());
    m_activeScene = std::move(sceneID);
  }
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
  PrintScene(m_registry);
}

void SceneManager::createEntities(
  std::string_view prefabID, const SceneLoader& loadedScene,
  const std::unique_ptr<AssetsManager>& assetsManager,
  const std::unique_ptr<Renderer>& renderer) {
  ENGINE_TRACE("Creating scene normal entities...");
  for (const auto& [name, data] :
       loadedScene.getLoadedNormalEntities(prefabID)) {
    std::string_view prototypeID = data.at("prototype").get<std::string_view>();
    entt::entity e = createEntity(prefabID, prototypeID);
    m_registry.emplace<CName>(e, std::string(name));
    m_registry.emplace<CTag>(e, prototypeID.data());
    if (data.contains("options")) {
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        m_registry.get<CRigidBody>(e).isKinematic =
          options.at("isKinematic").get<bool>();
      }
      if (options.contains("position")) {
        json position = options.at("position");
        m_registry.get<CTransform>(e).position = {
          position.at("x").get<float>(), position.at("y").get<float>(),
          position.at("z").get<float>()};
      }
      if (options.contains("rotation")) {
        json rotation = options.at("rotation");
        glm::vec3 rot = {rotation.at("x").get<float>(),
                         rotation.at("y").get<float>(),
                         rotation.at("z").get<float>()};
        m_registry.get<CTransform>(e).rotate(glm::quat(glm::radians(rot)));
      }
      if (m_registry.all_of<CCamera, CTransform>(e)) {
        CCamera& cCamera = m_registry.get<CCamera>(e);
        CTransform& cTransform = m_registry.get<CTransform>(e);
        deserializeCamera(cCamera, options);
        cCamera.calculateProjection();
        cCamera.calculateView(cTransform.position, cTransform.rotation);
      }
      if (options.contains("isActive")) {
        bool isActiveCamera = options.at("isActive").get<bool>();
        if (isActiveCamera and not m_registry.all_of<CActiveCamera>(e)) {
          m_registry.emplace<CActiveCamera>(e);
        } else if (not isActiveCamera and m_registry.all_of<CActiveCamera>(e)) {
          m_registry.remove<CActiveCamera>(e);
        }
      }
      if (options.contains("hasInput")) {
        bool hasInput = options.at("hasInput").get<bool>();
        if (hasInput and not m_registry.all_of<CActiveInput>(e)) {
          m_registry.emplace<CActiveInput>(e);
        } else if (not hasInput and m_registry.all_of<CActiveInput>(e)) {
          m_registry.remove<CActiveInput>(e);
        }
      }
      if (options.contains("inputMode")) {
        CInput& cInput = m_registry.get<CInput>(e);
        cInput._mode = options.at("inputMode").get<std::string>();
        cInput.setMode();
      }
      if (options.contains("translationSpeed")) {
        m_registry.get<CInput>(e).translationSpeed =
          options.at("translationSpeed").get<float>();
      }
      if (options.contains("verticalSpeed")) {
        m_registry.get<CInput>(e).verticalSpeed =
          options.at("verticalSpeed").get<float>();
      }
      if (options.contains("mouseSensitivity")) {
        m_registry.get<CInput>(e).mouseSensitivity =
          options.at("mouseSensitivity").get<float>();
      }
      if (options.contains("rotationSpeed")) {
        m_registry.get<CInput>(e).rotationSpeed =
          options.at("rotationSpeed").get<float>();
      }
      if (options.contains("size")) {
        CShape& shape = m_registry.get<CShape>(e);
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
        CShape& cShape = m_registry.get<CShape>(e);
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
        CBody& cBody = m_registry.get<CBody>(e);
        std::string filepath = options.at("body").get<std::string>();
        if (cBody.filepath not_eq filepath) {
          ENGINE_TRACE("Changing body filepath from {0} to {1} for entity {2}",
                       cBody.filepath, filepath, name);
          cBody.filepath = filepath;
          cBody.meshes.clear();
          cBody.materials.clear();
          auto model =
            *assetsManager->get<Model>(filepath); // We need a copy of the model
          cBody.meshes = std::move(model.getMeshes());
          cBody.materials = std::move(model.getMaterials());
        }
      }
      if (options.contains("filepaths")) {
        CTexture& c = m_registry.get<CTexture>(e);
        json filepaths = options.at("filepaths");
        std::vector<std::string> newFilepaths =
          filepaths.get<std::vector<std::string>>();
        if (c.filepaths not_eq newFilepaths) {
          std::string oldPaths;
          std::string newPaths;
          for (const auto& filepath : c.filepaths) {
            oldPaths += filepath + " ";
          }
          for (const auto& filepath : newFilepaths) {
            newPaths += filepath + " ";
          }
          ENGINE_TRACE("Changing texture filepaths from {} to {} for entity {}",
                       oldPaths, newPaths, name);
          c.filepaths = std::move(newFilepaths);
          c.textures.clear();
          c.textures.reserve(c.filepaths.size());
          for (const auto& filepath : c.filepaths) {
            c.textures.emplace_back(assetsManager->get<Texture>(filepath));
          }
        }
      }
      if (options.contains("color")) {
        json color = options.at("color");
        m_registry.get<CTexture>(e).color = {
          color.at("r").get<float>(), color.at("g").get<float>(),
          color.at("b").get<float>(), color.at("a").get<float>()};
      }
      if (options.contains("blendFactor")) {
        m_registry.get<CTexture>(e).blendFactor =
          options.at("blendFactor").get<float>();
      }
      if (options.contains("reflectivity")) {
        m_registry.get<CTexture>(e).reflectivity =
          options.at("reflectivity").get<float>();
      }
      if (options.contains("refractiveIndex")) {
        m_registry.get<CTexture>(e).refractiveIndex =
          options.at("refractiveIndex").get<float>();
      }
      if (options.contains("hasTransparency")) {
        m_registry.get<CTexture>(e).hasTransparency =
          options.at("hasTransparency").get<bool>();
      }
      if (options.contains("useLighting")) {
        m_registry.get<CTexture>(e).useLighting =
          options.at("useLighting").get<bool>();
      }
      if (options.contains("useReflection")) {
        m_registry.get<CTexture>(e).useReflection =
          options.at("useReflection").get<bool>();
      }
      if (options.contains("useRefraction")) {
        m_registry.get<CTexture>(e).useRefraction =
          options.at("useRefraction").get<bool>();
      }
      if (options.contains("isVisible")) {
        m_registry.get<CShaderProgram>(e).isVisible =
          options.at("isVisible").get<bool>();
      }
      if (options.contains("drawMode")) {
        m_registry.get<CTexture>(e)._drawMode =
          options.at("drawMode").get<std::string>();
        m_registry.get<CTexture>(e).setDrawMode();
      }
      if (options.contains("textureAtlas")) {
        CTextureAtlas& cTextureAtlas = m_registry.get<CTextureAtlas>(e);
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
        m_registry.get<CTransform>(e).scale = {scale.at("x").get<float>(),
                                               scale.at("y").get<float>(),
                                               scale.at("z").get<float>()};
      }
      if (prototypeID == "chunk_config") {
        CChunkManager& cChunkManager = m_registry.get<CChunkManager>(e);
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
        CNoise& noise = m_registry.get<CNoise>(e);
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
      if (prototypeID == "skybox") {
        CTime& cTime = m_registry.get<CTime>(e);
        if (options.contains("time")) {
          cTime.setTime(options.at("time").get<float>());
        }
        if (options.contains("acceleration")) {
          cTime.acceleration = options.at("acceleration").get<float>();
        }
        if (options.contains("useFog")) {
          m_registry.get<CSkybox>(e).useFog = options.at("useFog").get<bool>();
        }
        if (options.contains("fogColor")) {
          json fogColor = options.at("fogColor");
          m_registry.get<CSkybox>(e).fogColor = {fogColor.at("r").get<float>(),
                                                 fogColor.at("g").get<float>(),
                                                 fogColor.at("b").get<float>()};
        }
        if (options.contains("fogDensity")) {
          m_registry.get<CSkybox>(e).fogDensity =
            options.at("fogDensity").get<float>();
        }
        if (options.contains("fogGradient")) {
          m_registry.get<CSkybox>(e).fogGradient =
            options.at("fogGradient").get<float>();
        }
      }
    }
  }

  ENGINE_TRACE("Creating scene light entities...");
  for (const auto& [name, data] :
       loadedScene.getLoadedLightEntities(prefabID)) {
    std::string_view prototypeID = data.at("prototype").get<std::string_view>();
    entt::entity e = createEntity(prefabID, prototypeID);
    m_registry.emplace<CName>(e, std::string(name));
    m_registry.emplace<CTag>(e, prototypeID.data());
    if (data.contains("options")) {
      CTransform& cTransform = m_registry.get<CTransform>(e);
      CLight& cLight = m_registry.get<CLight>(e);
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        m_registry.get<CRigidBody>(e).isKinematic =
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
        m_registry.get<CShaderProgram>(e).isVisible =
          options.at("isVisible").get<bool>();
      }
      if (options.contains("body")) {
        CBody& cBody = m_registry.get<CBody>(e);
        std::string filepath = options.at("body").get<std::string>();
        if (cBody.filepath not_eq filepath) {
          ENGINE_TRACE("Changing body filepath from {0} to {1} for entity {2}",
                       cBody.filepath, filepath, name);
          cBody.filepath = filepath;
          cBody.meshes.clear();
          cBody.materials.clear();
          auto model =
            *assetsManager->get<Model>(filepath); // We need a copy of the model
          cBody.meshes = std::move(model.getMeshes());
          cBody.materials = std::move(model.getMaterials());
        }
      }
    }
  }

  ENGINE_TRACE("Creating scene camera entities...");
  for (const auto& [name, data] :
       loadedScene.getLoadedCameraEntities(prefabID)) {
    std::string_view prototypeID = data.at("prototype").get<std::string_view>();
    entt::entity e = createEntity(prefabID, prototypeID);
    m_registry.emplace<CName>(e, std::string(name));
    m_registry.emplace<CTag>(e, prototypeID.data());
    if (data.contains("options")) {
      CCamera& cCamera = m_registry.get<CCamera>(e);
      CTransform& cTransform = m_registry.get<CTransform>(e);
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        m_registry.get<CRigidBody>(e).isKinematic =
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
        if (isActiveCamera and not m_registry.all_of<CActiveCamera>(e)) {
          m_registry.emplace<CActiveCamera>(e);
        } else if (not isActiveCamera and m_registry.all_of<CActiveCamera>(e)) {
          m_registry.remove<CActiveCamera>(e);
        }
      }
      if (options.contains("hasInput")) {
        bool hasInput = options.at("hasInput").get<bool>();
        if (hasInput and not m_registry.all_of<CActiveInput>(e)) {
          m_registry.emplace<CActiveInput>(e);
        } else if (not hasInput and m_registry.all_of<CActiveInput>(e)) {
          m_registry.remove<CActiveInput>(e);
        }
      }
      if (options.contains("inputMode")) {
        CInput& cInput = m_registry.get<CInput>(e);
        cInput._mode = options.at("inputMode").get<std::string>();
        cInput.setMode();
      }
      if (options.contains("translationSpeed")) {
        m_registry.get<CInput>(e).translationSpeed =
          options.at("translationSpeed").get<float>();
      }
      if (options.contains("verticalSpeed")) {
        m_registry.get<CInput>(e).verticalSpeed =
          options.at("verticalSpeed").get<float>();
      }
      if (options.contains("mouseSensitivity")) {
        m_registry.get<CInput>(e).mouseSensitivity =
          options.at("mouseSensitivity").get<float>();
      }
      if (options.contains("rotationSpeed")) {
        m_registry.get<CInput>(e).rotationSpeed =
          options.at("rotationSpeed").get<float>();
      }
      if (options.contains("isVisible")) {
        m_registry.get<CShaderProgram>(e).isVisible =
          options.at("isVisible").get<bool>();
      }
      if (options.contains("body")) {
        CBody& cBody = m_registry.get<CBody>(e);
        std::string filepath = options.at("body").get<std::string>();
        if (cBody.filepath not_eq filepath) {
          ENGINE_TRACE("Changing body filepath from {0} to {1} for entity {2}",
                       cBody.filepath, filepath, name);
          cBody.filepath = filepath;
          cBody.meshes.clear();
          cBody.materials.clear();
          auto model =
            *assetsManager->get<Model>(filepath); // We need a copy of the model
          cBody.meshes = std::move(model.getMeshes());
          cBody.materials = std::move(model.getMaterials());
        }
      }
    }
  }

  ENGINE_TRACE("Creating scene system entities...");
  for (const auto& [name, data] :
       loadedScene.getLoadedSystemEntities(prefabID)) {
    std::string_view prototypeID = data.at("prototype").get<std::string_view>();
    entt::entity e = createEntity(prefabID, prototypeID);
    m_registry.emplace<CName>(e, std::string(name));
    m_registry.emplace<CTag>(e, prototypeID.data());
    if (data.contains("options")) {
      json options = data.at("options");
      // TODO: implement
    }
  }

  ENGINE_TRACE("Creating scene FBO entities...");
  for (const auto& [name, data] : loadedScene.getLoadedFBOEntities(prefabID)) {
    std::string_view prototypeID = data.at("prototype").get<std::string_view>();
    entt::entity e = createEntity(prefabID, prototypeID);
    m_registry.emplace<CName>(e, std::string(name));
    m_registry.emplace<CTag>(e, prototypeID.data());
    CFBO& fbo = m_registry.get<CFBO>(e);
    if (data.contains("options")) {
      json options = data.at("options");
      if (options.contains("width")) {
        fbo.width = options.at("width").get<int>();
      }
      if (options.contains("height")) {
        fbo.height = options.at("height").get<int>();
      }
      if (options.contains("attachment")) {
        fbo._attachment = std::move(options.at("attachment").get<std::string>());
        fbo.setAttachment();
      }
      if (options.contains("mode")) {
        fbo._mode = std::move(options.at("mode").get<std::string>());
        fbo.setMode();
      }
    }
    renderer->addFramebuffer(std::string(fbo.fbo), fbo.width, fbo.height,
                             fbo.attachment);
  }

  m_registry.sort<CDistanceFromCamera>(
    [](const CDistanceFromCamera& lhs, const CDistanceFromCamera& rhs) {
      return lhs.distance < rhs.distance;
    });

  m_registry.sort<CUUID, CDistanceFromCamera>();
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

void SceneManager::reloadScene(
  const std::unique_ptr<AssetsManager>& assetsManager,
  const std::unique_ptr<Renderer>& renderer) {
  ENGINE_ASSERT(not m_activeScene.empty(), "No scene is active!");
  ENGINE_WARN("Reloading scene {}", m_activeScene);

  m_registry.clear(); // delete instances and prototypes but reusing them later
  m_entityFactory.clear();
  renderer->clear();
  createScene(m_activeScene, assetsManager, renderer, true);
  m_dirtyMetrics = true;
  m_dirtyNamedEntities = true;
}

void SceneManager::clearScene(const std::unique_ptr<Renderer>& renderer) {
  ENGINE_ASSERT(not m_activeScene.empty(), "No scene is active!");
  ENGINE_WARN("Clearing scene {}", m_activeScene);

  m_loadedScenes.erase(m_activeScene);
  m_registry.clear(); // soft delete / = {};  would delete them completely but
                      // does not invoke signals/mixin methods
  m_entityFactory.clear();
  renderer->clear();
  m_activeScene.clear();
  m_metrics.clear();
  m_metrics.clear();
  m_namedEntities.clear();
  m_dirtyMetrics = false;
  m_dirtyNamedEntities = false;
  entt::monostate<"useFog"_hs>{} = 0.f;
  entt::monostate<"useSkyBlending"_hs>{} = 0.f;
}

template <typename Component>
void SceneManager::onComponentAdded(entt::entity e, Component& c) {
  ENGINE_ASSERT(false,
                "Unsupported onComponentAdded method for component type {}",
                entt::type_id<Component>().name());
}

template <typename Component>
void SceneManager::onComponentCloned(entt::entity e, Component& c) {
  ENGINE_ASSERT(false,
                "Unsupported onComponentCloned method for component type {}",
                entt::type_id<Component>().name());
}

std::unique_ptr<SceneManager> SceneManager::Create() {
  return std::make_unique<SceneManager>();
}
}