#include "scene/sceneManager.h"

#include <glm/gtx/string_cast.hpp>

#include "scene/components/camera/cActiveCamera.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/camera/serializer.h"
#include "scene/components/common/cName.h"
#include "scene/components/common/cTag.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/graphics/cFBO.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/utils/cDeleted.h"
#include "scene/components/utils/cNoise.h"
#include "scene/components/world/cChunkManager.h"
#include "scene/components/world/cLight.h"
#include "scene/components/world/cSkybox.h"
#include "scene/components/world/cTime.h"
#include "scene/entity.h"
#include "scene/systems/sEvent.h"
#include "scene/systems/sUpdate.h"
#include "scene/utils.h"
#include "utils/timer.h"

using json = nlohmann::json;
using namespace entt::literals;

namespace potatoengine {

SceneManager::SceneManager(std::weak_ptr<AssetsManager> am,
                           std::weak_ptr<Renderer> r)
  : m_entityFactory(am), m_assetsManager(am), m_renderer(r) {
  ENGINE_TRACE("Initializing scene manager...");
  ENGINE_TRACE("Registering engine components...");
  RegisterComponents();
  ENGINE_TRACE("Scene manager created!");
}

void SceneManager::onEvent(Event& e) { eventSystem(std::ref(m_registry), e); }

void SceneManager::onUpdate(Time ts, std::weak_ptr<Renderer> r) {
  updateSystem(std::ref(m_registry), r, ts);
}

void SceneManager::print() { PrintScene(std::ref(m_registry)); }

const std::map<std::string, std::string, NumericComparator>&
SceneManager::getMetrics() {
  if (not m_dirty) {
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
  m_dirty = false;

  return m_metrics;
}

Entity SceneManager::createEntity(std::string_view prefabID,
                                  std::string_view prototypeID,
                                  const std::optional<uint32_t>& uuid) {
  UUID _uuid = uuid.has_value() ? UUID(uuid.value()) : UUID();
  Entity e = cloneEntity(getPrototype(prefabID, prototypeID), _uuid);
  e.add<CUUID>(_uuid);
  m_dirty = true;
  return e;
}

Entity SceneManager::cloneEntity(Entity&& e, uint32_t uuid) {
  Entity cloned = Entity(m_registry.create(), this);

  for (auto&& curr : m_registry.storage()) {
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

  m_dirty = true;
  return cloned;
}

void SceneManager::removeEntity(Entity&& e) {
  e.add<CDeleted>();
  m_dirty = true;
}

Entity SceneManager::getEntity(std::string_view name) {
  for (const auto& [e, cName, _] : m_registry.view<CName, CUUID>().each()) {
    if (cName.name == name) {
      return Entity(e, this);
    }
  }
  ENGINE_ASSERT(false, "Entity not found");
}

Entity SceneManager::getEntity(UUID& uuid) {
  for (const auto& [e, cUUID] : m_registry.view<CUUID>().each()) {
    if (cUUID.uuid == uuid) {
      return Entity(e, this);
    }
  }
  ENGINE_ASSERT(false, "Entity not found");
}

const std::map<std::string, entt::entity, NumericComparator>&
SceneManager::getAllNamedEntities() {
  if (not m_dirty) {
    return m_namedEntities;
  }

  m_namedEntities.clear();
  m_registry.view<CName, CUUID>().each(
    [&](entt::entity e, const CName& cName, const CUUID& cUUID) {
      m_namedEntities.emplace(cName.name, e);
    });
  getMetrics();

  return m_namedEntities;
}

void SceneManager::createPrototypes(std::string_view prefabID) {
  // TODO think a better way, we need to create an entity so
  // we have access to the scene manager and avoid circular
  // dependency but we discard it afterwards to create one per prototype
  m_entityFactory.createPrototypes(prefabID, Entity(m_registry.create(), this));
  m_dirty = true;
}

void SceneManager::updatePrototypes(std::string_view prefabID) {
  m_entityFactory.updatePrototypes(prefabID, Entity(m_registry.create(), this));
}

void SceneManager::destroyPrototypes(std::string_view prefabID) {
  m_entityFactory.destroyPrototypes(prefabID, std::ref(m_registry));
  m_dirty = true;
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
  m_entityFactory.createPrototype(prefabID, prototypeID,
                                  Entity(m_registry.create(), this));
  m_dirty = true;
}

void SceneManager::updatePrototype(std::string_view prefabID,
                                   std::string_view prototypeID) {
  m_entityFactory.updatePrototype(prefabID, prototypeID,
                                  Entity(m_registry.create(), this));
}

void SceneManager::destroyPrototype(std::string_view prefabID,
                                    std::string_view prototypeID) {
  m_entityFactory.destroyPrototype(prefabID, prototypeID, std::ref(m_registry));
  m_dirty = true;
}

Entity SceneManager::getPrototype(std::string_view prefabID,
                                  std::string_view prototypeID) {
  return Entity(m_entityFactory.getPrototype(prefabID, prototypeID), this);
}

bool SceneManager::containsPrototype(std::string_view prefabID,
                                     std::string_view prototypeID) const {
  return m_entityFactory.containsPrototype(prefabID, prototypeID);
}

void SceneManager::loadScene(std::string_view sceneID) {
  ENGINE_TRACE("Loading scene elements...");
  SceneLoader loadedScene(m_assetsManager);
  loadedScene.load(sceneID);
  m_loadedScenes.emplace(sceneID, std::move(loadedScene));
  ENGINE_TRACE("Scene elements loaded!");
}

void SceneManager::createScene(std::string sceneID, bool reload) {
  if (reload) {
    ENGINE_INFO("Reloading scene...");
  } else {
    ENGINE_INFO("Creating scene...");
  }
  Timer timer;

  const auto& renderer = m_renderer.lock();
  ENGINE_ASSERT(renderer, "Renderer is null!");

  const auto& manager = m_assetsManager.lock();
  ENGINE_ASSERT(manager, "AssetsManager is null!");

  ENGINE_ASSERT(m_activeScene.empty() or reload, "Scene {} already exists!",
                sceneID);
  ENGINE_ASSERT(m_loadedScenes.contains(sceneID), "Scene {} is not loaded!",
                sceneID);

  ENGINE_TRACE("Linking scene shaders...");
  const auto& loadedScene = m_loadedScenes.at(sceneID);
  for (std::string id : loadedScene.getLoadedShaders()) {
    renderer->addShader(std::move(id));
  }

  // A Prefab asset is a json file that contains a list of entity prototypes
  for (std::string_view prefabID : loadedScene.getLoadedPrefabs()) {
    ENGINE_TRACE("Creating scene prefabs prototypes...");
    createPrototypes(prefabID);
    ENGINE_TRACE("Creating scene entities...");
    createEntities(prefabID, loadedScene, manager, renderer);
  }

  if (reload) {
    ENGINE_INFO("Scene {} reloading TIME: {:.6f}s", m_activeScene,
                timer.getSeconds());
  } else {
    ENGINE_INFO("Scene {} creation TIME: {:.6f}s", sceneID, timer.getSeconds());
    m_activeScene = std::move(sceneID);
  }
  m_dirty = true;
  PrintScene(std::ref(m_registry));
}

void SceneManager::createEntities(std::string_view prefabID,
                                  const SceneLoader& loadedScene,
                                  const std::shared_ptr<AssetsManager>& manager,
                                  const std::shared_ptr<Renderer>& renderer) {
  ENGINE_TRACE("Creating scene normal entities...");
  for (const auto& [name, data] :
       loadedScene.getLoadedNormalEntities(prefabID)) {
    std::string_view prototypeID = data.at("prototype").get<std::string_view>();
    Entity e = createEntity(prefabID, prototypeID);
    e.add<CName>(std::string(name));
    e.add<CTag>(prototypeID.data());
    if (data.contains("options")) {
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        e.get<CRigidBody>().isKinematic = options.at("isKinematic").get<bool>();
      }
      if (options.contains("position")) {
        json position = options.at("position");
        e.get<CTransform>().position = {position.at("x").get<float>(),
                                        position.at("y").get<float>(),
                                        position.at("z").get<float>()};
      }
      if (options.contains("rotation")) {
        json rotation = options.at("rotation");
        glm::vec3 rot = {rotation.at("x").get<float>(),
                         rotation.at("y").get<float>(),
                         rotation.at("z").get<float>()};
        e.get<CTransform>().rotate(glm::quat(glm::radians(rot)));
      }
      if (e.has_all<CCamera, CTransform>()) {
        CCamera& cCamera = e.get<CCamera>();
        CTransform& cTransform = e.get<CTransform>();
        deserializeCamera(cCamera, options);
        cCamera.calculateProjection();
        cCamera.calculateView(cTransform.position, cTransform.rotation);
      }
      if (options.contains("isActive")) {
        bool isActiveCamera = options.at("isActive").get<bool>();
        if (isActiveCamera and not e.has_all<CActiveCamera>()) {
          e.add<CActiveCamera>();
        } else if (not isActiveCamera and e.has_all<CActiveCamera>()) {
          e.remove<CActiveCamera>();
        }
      }
      if (options.contains("size")) {
        CShape& shape = e.get<CShape>();
        json size = options.at("size");
        glm::vec3 sizeVec = {size.at("x").get<float>(),
                             size.at("y").get<float>(),
                             size.at("z").get<float>()};
        if (shape.size not_eq sizeVec) {
          ENGINE_TRACE("Changing shape size from {0} to {1}",
                       glm::to_string(shape.size), glm::to_string(sizeVec));
          shape.size = sizeVec;
          shape.meshes.clear();
          shape.createMesh();
        }
      }
      if (options.contains("repeatTexture")) {
        CShape& cShape = e.get<CShape>();
        bool repeatTexture = options.at("repeatTexture").get<bool>();
        if (cShape.repeatTexture not_eq repeatTexture) {
          ENGINE_TRACE("Changing shape repeatTexture from {0} to {1}",
                       cShape.repeatTexture, repeatTexture);
          cShape.repeatTexture = repeatTexture;
          cShape.meshes.clear();
          cShape.createMesh();
        }
      }
      if (options.contains("body")) {
        CBody& cBody = e.get<CBody>();
        std::string filepath = options.at("body").get<std::string>();
        if (cBody.filepath not_eq filepath) {
          ENGINE_TRACE("Changing body filepath from {0} to {1}", cBody.filepath,
                       filepath);
          cBody.filepath = filepath;
          cBody.meshes.clear();
          cBody.materials.clear();
          auto model =
            *manager->get<Model>(filepath); // We need a copy of the model
          cBody.meshes = std::move(model.getMeshes());
          cBody.materials = std::move(model.getMaterials());
        }
      }
      if (options.contains("filepaths")) {
        CTexture& c = e.get<CTexture>();
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
          ENGINE_TRACE("Changing texture filepaths from {} to {}", oldPaths,
                       newPaths);
          c.filepaths = std::move(newFilepaths);
          c.textures.clear();
          c.textures.reserve(c.filepaths.size());
          for (const auto& filepath : c.filepaths) {
            c.textures.emplace_back(manager->get<Texture>(filepath));
          }
        }
      }
      if (options.contains("color")) {
        json color = options.at("color");
        e.get<CTexture>().color = {
          color.at("r").get<float>(), color.at("g").get<float>(),
          color.at("b").get<float>(), color.at("a").get<float>()};
      }
      if (options.contains("blendFactor")) {
        e.get<CTexture>().blendFactor = options.at("blendFactor").get<float>();
      }
      if (options.contains("reflectivity")) {
        e.get<CTexture>().reflectivity =
          options.at("reflectivity").get<float>();
      }
      if (options.contains("refractiveIndex")) {
        e.get<CTexture>().refractiveIndex =
          options.at("refractiveIndex").get<float>();
      }
      if (options.contains("hasTransparency")) {
        e.get<CTexture>().hasTransparency =
          options.at("hasTransparency").get<bool>();
      }
      if (options.contains("useLighting")) {
        e.get<CTexture>().useLighting = options.at("useLighting").get<bool>();
      }
      if (options.contains("useReflection")) {
        e.get<CTexture>().useReflection =
          options.at("useReflection").get<bool>();
      }
      if (options.contains("useRefraction")) {
        e.get<CTexture>().useRefraction =
          options.at("useRefraction").get<bool>();
      }
      if (options.contains("isVisible")) {
        e.get<CShaderProgram>().isVisible = options.at("isVisible").get<bool>();
      }
      if (options.contains("drawMode")) {
        e.get<CTexture>()._drawMode = options.at("drawMode").get<std::string>();
        e.get<CTexture>().setDrawMode();
      }
      if (options.contains("textureAtlas")) {
        CTextureAtlas& cTextureAtlas = e.get<CTextureAtlas>();
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
        e.get<CTransform>().scale = {scale.at("x").get<float>(),
                                     scale.at("y").get<float>(),
                                     scale.at("z").get<float>()};
      }
      if (prototypeID == "chunk_config") {
        CChunkManager& cChunkManager = e.get<CChunkManager>();
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
        CNoise& noise = e.get<CNoise>();
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
        CTime& cTime = e.get<CTime>();
        if (options.contains("time")) {
          cTime.setTime(options.at("time").get<float>());
        }
        if (options.contains("acceleration")) {
          cTime.acceleration = options.at("acceleration").get<float>();
        }
        if (options.contains("useFog")) {
          e.get<CSkybox>().useFog = options.at("useFog").get<bool>();
        }
        if (options.contains("fogColor")) {
          json fogColor = options.at("fogColor");
          e.get<CSkybox>().fogColor = {fogColor.at("r").get<float>(),
                                       fogColor.at("g").get<float>(),
                                       fogColor.at("b").get<float>()};
        }
        if (options.contains("fogDensity")) {
          e.get<CSkybox>().fogDensity = options.at("fogDensity").get<float>();
        }
        if (options.contains("fogGradient")) {
          e.get<CSkybox>().fogGradient = options.at("fogGradient").get<float>();
        }
      }
    }
  }

  ENGINE_TRACE("Creating scene light entities...");
  for (const auto& [name, data] :
       loadedScene.getLoadedLightEntities(prefabID)) {
    std::string_view prototypeID = data.at("prototype").get<std::string_view>();
    Entity e = createEntity(prefabID, prototypeID);
    e.add<CName>(std::string(name));
    e.add<CTag>(prototypeID.data());
    if (data.contains("options")) {
      CTransform& cTransform = e.get<CTransform>();
      CLight& cLight = e.get<CLight>();
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        e.get<CRigidBody>().isKinematic = options.at("isKinematic").get<bool>();
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
        e.get<CShaderProgram>().isVisible = options.at("isVisible").get<bool>();
      }
      if (options.contains("body")) {
        CBody& cBody = e.get<CBody>();
        std::string filepath = options.at("body").get<std::string>();
        if (cBody.filepath not_eq filepath) {
          ENGINE_TRACE("Changing body filepath from {0} to {1}", cBody.filepath,
                       filepath);
          cBody.filepath = filepath;
          cBody.meshes.clear();
          cBody.materials.clear();
          auto model =
            *manager->get<Model>(filepath); // We need a copy of the model
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
    Entity e = createEntity(prefabID, prototypeID);
    e.add<CName>(std::string(name));
    e.add<CTag>(prototypeID.data());
    if (data.contains("options")) {
      CCamera& cCamera = e.get<CCamera>();
      CTransform& cTransform = e.get<CTransform>();
      json options = data.at("options");
      if (options.contains("isKinematic")) {
        e.get<CRigidBody>().isKinematic = options.at("isKinematic").get<bool>();
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
      cCamera.calculateView(cTransform.position, cTransform.rotation);
      if (options.contains("isActive")) {
        bool isActiveCamera = options.at("isActive").get<bool>();
        if (isActiveCamera and not e.has_all<CActiveCamera>()) {
          e.add<CActiveCamera>();
        } else if (not isActiveCamera and e.has_all<CActiveCamera>()) {
          e.remove<CActiveCamera>();
        }
      }
      if (options.contains("isVisible")) {
        e.get<CShaderProgram>().isVisible = options.at("isVisible").get<bool>();
      }
      if (options.contains("body")) {
        CBody& cBody = e.get<CBody>();
        std::string filepath = options.at("body").get<std::string>();
        if (cBody.filepath not_eq filepath) {
          ENGINE_TRACE("Changing body filepath from {0} to {1}", cBody.filepath,
                       filepath);
          cBody.filepath = filepath;
          cBody.meshes.clear();
          cBody.materials.clear();
          auto model =
            *manager->get<Model>(filepath); // We need a copy of the model
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
    Entity e = createEntity(prefabID, prototypeID);
    e.add<CName>(std::string(name));
    e.add<CTag>(prototypeID.data());
    if (data.contains("options")) {
      json options = data.at("options");
      // TODO: implement
    }
  }

  ENGINE_TRACE("Creating scene FBO entities...");
  for (const auto& [name, data] : loadedScene.getLoadedFBOEntities(prefabID)) {
    std::string_view prototypeID = data.at("prototype").get<std::string_view>();
    Entity e = createEntity(prefabID, prototypeID);
    e.add<CName>(std::string(name));
    e.add<CTag>(prototypeID.data());
    CFBO& fbo = e.get<CFBO>();
    if (data.contains("options")) {
      json options = data.at("options");
      if (options.contains("width")) {
        fbo.width = options.at("width").get<int>();
      }
      if (options.contains("height")) {
        fbo.height = options.at("height").get<int>();
      }
      if (options.contains("attachment")) {
        fbo.attachment = std::move(options.at("attachment").get<std::string>());
      }
      if (options.contains("mode")) {
        fbo._mode = std::move(options.at("mode").get<std::string>());
      }
    }
    uint32_t attachment;
    if (fbo.attachment == "depth_texture") {
      attachment = engine::FBO::DEPTH_TEXTURE;
    } else if (fbo.attachment == "depth_renderbuffer") {
      attachment = engine::FBO::DEPTH_RENDERBUFFER;
    } else if (fbo.attachment == "stencil_renderbuffer") {
      attachment = engine::FBO::STENCIL_RENDERBUFFER;
    } else if (fbo.attachment == "depth_stencil_renderbuffer") {
      attachment = engine::FBO::DEPTH_STENCIL_RENDERBUFFER;
    } else {
      ENGINE_ASSERT(false, "Unknown fbo attachment: {}", fbo.attachment);
    }
    CFBO::Mode mode;
    if (fbo._mode == "normal") {
      mode = CFBO::Mode::Normal;
    } else if (fbo._mode == "inverse") {
      mode = CFBO::Mode::Inverse;
    } else if (fbo._mode == "greyscale") {
      mode = CFBO::Mode::Greyscale;
    } else if (fbo._mode == "blur") {
      mode = CFBO::Mode::Blur;
    } else if (fbo._mode == "edge") {
      mode = CFBO::Mode::Edge;
    } else if (fbo._mode == "sharpen") {
      mode = CFBO::Mode::Sharpen;
    } else if (fbo._mode == "night_vision") {
      mode = CFBO::Mode::NightVision;
    } else if (fbo._mode == "emboss") {
      mode = CFBO::Mode::Emboss;
    } else {
      ENGINE_ASSERT(false, "Unknown fbo mode: {}", fbo._mode);
    }
    fbo.mode = mode;

    renderer->addFramebuffer(std::string(fbo.fbo), fbo.width, fbo.height,
                             attachment);
  }

  m_registry.sort<CDistanceFromCamera>(
    [](const CDistanceFromCamera& lhs, const CDistanceFromCamera& rhs) {
      return lhs.distance < rhs.distance;
    });

  m_registry.sort<CUUID, CDistanceFromCamera>();
  m_dirty = true;
}

void SceneManager::reloadScene() {
  ENGINE_ASSERT(not m_activeScene.empty(), "No scene is active!");
  ENGINE_WARN("Reloading scene {}", m_activeScene);
  auto renderer = m_renderer.lock();
  ENGINE_ASSERT(renderer, "Renderer is null!");

  m_registry.clear(); // delete instances and prototypes but reusing them later
  m_entityFactory.clear();
  renderer->clear();
  createScene(m_activeScene, true);
  m_dirty = true;
}

void SceneManager::clearScene() {
  ENGINE_ASSERT(not m_activeScene.empty(), "No scene is active!");
  ENGINE_WARN("Clearing scene {}", m_activeScene);
  auto renderer = m_renderer.lock();
  ENGINE_ASSERT(renderer, "Renderer is null!");

  m_loadedScenes.erase(m_activeScene);
  m_registry.clear(); // soft delete / = {};  would delete them completely but
                      // does not invoke signals/mixin methods
  m_entityFactory.clear();
  renderer->clear();
  m_activeScene = "";
  m_metrics.clear();
  m_dirty = false;
}

template <typename T> void SceneManager::onComponentAdded(Entity& e, T& c) {
  ENGINE_ASSERT(false,
                "Unsupported onComponentAdded method for component type {}",
                entt::type_id<T>().name());
}

template <typename T> void SceneManager::onComponentCloned(Entity& e, T& c) {
  ENGINE_ASSERT(false,
                "Unsupported onComponentCloned method for component type {}",
                entt::type_id<T>().name());
}

}