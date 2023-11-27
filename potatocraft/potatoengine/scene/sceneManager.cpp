#include "potatoengine/scene/sceneManager.h"

#include "potatoengine/scene/components/camera/cActiveCamera.h"
#include "potatoengine/scene/components/camera/cCamera.h"
#include "potatoengine/scene/components/camera/serializer.h"
#include "potatoengine/scene/components/common/cName.h"
#include "potatoengine/scene/components/common/cTag.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/graphics/cFBO.h"
#include "potatoengine/scene/components/graphics/cTextureAtlas.h"
#include "potatoengine/scene/components/physics/cTransform.h"
#include "potatoengine/scene/components/utils/cDeleted.h"
#include "potatoengine/scene/components/utils/cNoise.h"
#include "potatoengine/scene/components/world/cChunkManager.h"
#include "potatoengine/scene/components/world/cLight.h"
#include "potatoengine/scene/components/world/cSkybox.h"
#include "potatoengine/scene/components/world/cTime.h"
#include "potatoengine/scene/entity.h"
#include "potatoengine/scene/systems/sEvent.h"
#include "potatoengine/scene/systems/sUpdate.h"
#include "potatoengine/scene/utils.h"
#include "potatoengine/utils/timer.h"

using json = nlohmann::json;
namespace potatoengine {

Entity SceneManager::createEntity(std::string_view id, const std::optional<uint32_t>& uuid) {
    UUID _uuid = uuid.has_value() ? UUID(uuid.value()) : UUID();
    Entity e = cloneEntity({m_entityFactory.get(id), this}, _uuid);
    e.add<CUUID>(_uuid);

    return e;
}

void SceneManager::removeEntity(Entity& e) {
    e.add<CDeleted>();
}

void SceneManager::print() {
    printScene(std::ref(m_registry));
}

Entity SceneManager::getEntity(std::string_view name) {
    for (const auto& [e, cName, _] : m_registry.view<CName, CUUID>().each()) {
        if (cName.name == name) {
            return Entity(e, this);
        }
    }
    throw std::runtime_error("Entity not found");
}

Entity SceneManager::getEntity(UUID& uuid) {
    for (const auto& [e, cUUID] : m_registry.view<CUUID>().each()) {
        if (cUUID.uuid == uuid) {
            return Entity(e, this);
        }
    }
    throw std::runtime_error("Entity not found");
}

void SceneManager::createPrefab(std::string_view id) {
    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager not found!");
    }

    if (manager->contains<Prefab>(id)) {
        m_entityFactory.create(id, {m_registry.create(), this});
    }
}

void SceneManager::updatePrefab(std::string_view id) {
    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager not found!");
    }

    if (manager->contains<Prefab>(id)) {
        m_entityFactory.update(id, {m_registry.create(), this}, std::ref(m_registry));
    }
}

void SceneManager::loadScene(std::string_view id) {
    CORE_INFO("Loading scene elements...");

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager not found!");
    }

    if (manager->contains<Scene>(id)) {
        SceneLoader loadedScene(manager);
        loadedScene.load(id);
        m_loadedScenes.emplace(id, std::move(loadedScene));
    }
}

void SceneManager::createScene(std::string id) {
    CORE_INFO("Creating scene...");
    Timer timer;

    if (m_activeScene == id) {
        throw std::runtime_error("Scene " + std::string{id} + " is already active!");
    }

    if (not m_loadedScenes.contains(id.data())) {
        throw std::runtime_error("Scene " + std::string{id} + " is not loaded!");
    }

    if (not m_activeScene.empty()) {
        clearScene(m_activeScene);
    }

    const auto& renderer = m_renderer.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer not found!");
    }

    CORE_INFO("Linking scene shaders...");
    const auto& loadedScene = m_loadedScenes.at(id.data());
    for (std::string id : loadedScene.getLoadedShaders()) {
        renderer->addShader(std::move(id));
    }

    CORE_INFO("Creating scene prefabs...");
    for (std::string_view id : loadedScene.getLoadedPrefabs()) {
        createPrefab(id);
    }

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    CORE_INFO("Creating scene entities...");
    for (const auto& [name, data] : loadedScene.getLoadedEntities()) {
        std::string_view prefab = data.at("prefab").get<std::string_view>();
        Entity e = createEntity(prefab);
        e.add<CName>(std::string(name));
        e.add<CTag>(prefab.data());
        if (data.contains("options")) {
            json options = data.at("options");
            if (options.contains("isKinematic")) {
                e.get<CRigidBody>().isKinematic = options.at("isKinematic").get<bool>();
            }
            if (options.contains("position")) {
                json position = options.at("position");
                e.get<CTransform>().position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
            }
            if (options.contains("rotation")) {
                json rotation = options.at("rotation");
                glm::vec3 rot = {rotation.at("x").get<float>(), rotation.at("y").get<float>(), rotation.at("z").get<float>()};
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
                shape.size = {size.at("x").get<float>(), size.at("y").get<float>(), size.at("z").get<float>()};
                shape.meshes.clear();
                shape.createMesh();
            }
            if (options.contains("repeatTexture")) {
                CShape& cShape = e.get<CShape>();
                cShape.repeatTexture = options.at("repeatTexture").get<bool>();
                cShape.meshes.clear();
                cShape.createMesh();
            }
            if (options.contains("body")) {
                CBody& cBody = e.get<CBody>();
                std::string filepath = options.at("body").get<std::string>();
                cBody.filepath = filepath;
                cBody.meshes.clear();
                cBody.materials.clear();
                auto model = *manager->get<Model>(filepath);  // We need a copy of the model
                cBody.meshes = std::move(model.getMeshes());
                cBody.materials = std::move(model.getMaterials());
            }
            if (options.contains("filepaths")) {
                json filepaths = options.at("filepaths");
                CTexture& c = e.get<CTexture>();
                c.filepaths.clear();
                c.textures.clear();
                c.filepaths.reserve(filepaths.size());
                c.textures.reserve(filepaths.size());
                for (const auto& filepath : filepaths) {
                    c.filepaths.emplace_back(filepath.get<std::string>());
                    c.textures.emplace_back(manager->get<Texture>(filepath.get<std::string>()));
                }
            }
            if (options.contains("color")) {
                json color = options.at("color");
                e.get<CTexture>().color = {color.at("r").get<float>(), color.at("g").get<float>(), color.at("b").get<float>(), color.at("a").get<float>()};
            }
            if (options.contains("blendFactor")) {
                e.get<CTexture>().blendFactor = options.at("blendFactor").get<float>();
            }
            if (options.contains("reflectivity")) {
                e.get<CTexture>().reflectivity = options.at("reflectivity").get<float>();
            }
            if (options.contains("refractiveIndex")) {
                e.get<CTexture>().refractiveIndex = options.at("refractiveIndex").get<float>();
            }
            if (options.contains("hasTransparency")) {
                e.get<CTexture>().hasTransparency = options.at("hasTransparency").get<bool>();
            }
            if (options.contains("useLighting")) {
                e.get<CTexture>().useLighting = options.at("useLighting").get<bool>();
            }
            if (options.contains("useReflection")) {
                e.get<CTexture>().useReflection = options.at("useReflection").get<bool>();
            }
            if (options.contains("useRefraction")) {
                e.get<CTexture>().useRefraction = options.at("useRefraction").get<bool>();
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
                e.get<CTransform>().scale = {scale.at("x").get<float>(), scale.at("y").get<float>(), scale.at("z").get<float>()};
            }
            if (prefab == "chunk_config") {
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
                    cChunkManager._meshAlgorithm = options.at("meshAlgorithm").get<std::string>();
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
            if (prefab == "skybox") {
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
                    e.get<CSkybox>().fogColor = {fogColor.at("r").get<float>(), fogColor.at("g").get<float>(), fogColor.at("b").get<float>()};
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

    CORE_INFO("Creating scene lights...");
    for (const auto& [name, data] : loadedScene.getLoadedLights()) {
        std::string_view prefab = data.at("prefab").get<std::string_view>();
        Entity e = createEntity(prefab);
        e.add<CName>(std::string(name));
        e.add<CTag>(prefab.data());
        if (data.contains("options")) {
            CTransform& cTransform = e.get<CTransform>();
            CLight& cLight = e.get<CLight>();
            json options = data.at("options");
            if (options.contains("isKinematic")) {
                e.get<CRigidBody>().isKinematic = options.at("isKinematic").get<bool>();
            }
            if (options.contains("position")) {
                json position = options.at("position");
                cTransform.position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
            }
            if (options.contains("rotation")) {
                json rotation = options.at("rotation");
                glm::vec3 rot = {rotation.at("x").get<float>(), rotation.at("y").get<float>(), rotation.at("z").get<float>()};
                cTransform.rotate(glm::quat(glm::radians(rot)));
            }
            if (options.contains("scale")) {
                json scale = options.at("scale");
                cTransform.scale = {scale.at("x").get<float>(), scale.at("y").get<float>(), scale.at("z").get<float>()};
            }
            if (options.contains("intensity")) {
                cLight.intensity = options.at("intensity").get<float>();
            }
            if (options.contains("color")) {
                json color = options.at("color");
                cLight.color = {color.at("r").get<float>(), color.at("g").get<float>(), color.at("b").get<float>()};
            }
            if (options.contains("isVisible")) {
                e.get<CShaderProgram>().isVisible = options.at("isVisible").get<bool>();
            }
            if (options.contains("body")) {
                CBody& cBody = e.get<CBody>();
                std::string filepath = options.at("body").get<std::string>();
                cBody.filepath = filepath;
                cBody.meshes.clear();
                cBody.materials.clear();
                auto model = *manager->get<Model>(filepath);  // We need a copy of the model
                cBody.meshes = std::move(model.getMeshes());
                cBody.materials = std::move(model.getMaterials());
            }
        }
    }

    CORE_INFO("Creating scene cameras...");
    for (const auto& [name, data] : loadedScene.getLoadedCameras()) {
        std::string_view prefab = data.at("prefab").get<std::string_view>();
        Entity e = createEntity(prefab);
        e.add<CName>(std::string(name));
        e.add<CTag>(prefab.data());
        if (data.contains("options")) {
            CCamera& cCamera = e.get<CCamera>();
            CTransform& cTransform = e.get<CTransform>();
            json options = data.at("options");
            if (options.contains("isKinematic")) {
                e.get<CRigidBody>().isKinematic = options.at("isKinematic").get<bool>();
            }
            if (options.contains("position")) {
                json position = options.at("position");
                cTransform.position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
            }
            if (options.contains("rotation")) {
                json rotation = options.at("rotation");
                glm::vec3 rot = {rotation.at("x").get<float>(), rotation.at("y").get<float>(), rotation.at("z").get<float>()};
                cTransform.rotate(glm::quat(glm::radians(rot))); // TODO: fix this will glitch with camera movement
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
                cBody.filepath = filepath;
                cBody.meshes.clear();
                cBody.materials.clear();
                auto model = *manager->get<Model>(filepath);  // We need a copy of the model
                cBody.meshes = std::move(model.getMeshes());
                cBody.materials = std::move(model.getMaterials());
            }
        }
    }

    CORE_INFO("Creating scene systems...");
    for (const auto& [name, data] : loadedScene.getLoadedSystems()) {
        std::string_view prefab = data.at("prefab").get<std::string_view>();
        Entity e = createEntity(prefab);
        e.add<CName>(std::string(name));
        e.add<CTag>(prefab.data());
        if (data.contains("options")) {
            json options = data.at("options");
            // TODO: implement
        }
    }

    CORE_INFO("Creating scene FBOs...");
    for (const auto& [name, data] : loadedScene.getLoadedFBOs()) {
        std::string_view prefab = data.at("prefab").get<std::string_view>();
        Entity e = createEntity(prefab);
        e.add<CName>(std::string(name));
        e.add<CTag>(prefab.data());
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
            throw std::runtime_error("Unknown fbo attachment: " + fbo.attachment);
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
            throw std::runtime_error("Unknown fbo mode " + fbo._mode);
        }
        fbo.mode = mode;

        renderer->addFramebuffer(std::string(fbo.fbo), fbo.width, fbo.height, attachment);
    }

    CORE_INFO("Creating sub-scenes...");
    // TODO: implement
    
    m_registry.sort<CDistanceFromCamera>([](const CDistanceFromCamera& lhs, const CDistanceFromCamera& rhs) {
        return lhs.distance < rhs.distance;
    });

    m_registry.sort<CUUID, CDistanceFromCamera>();

    m_activeScene = std::move(id);
    CORE_INFO("Scene {} creation TIME: {}", m_activeScene, timer.getSeconds());
}

void SceneManager::updateScene(std::string_view id) {
    // TODO: implement
}

void SceneManager::clearScene(std::string_view id) {
    CORE_WARN("Detaching GameState");
    if (m_activeScene == id) {
        m_registry.clear();
        m_entityFactory.clear();
        m_activeScene = "";
    } else {
        throw std::runtime_error("Scene " + std::string{id} + " is not active!");
    }
}

template <typename T>
void SceneManager::onComponentAdded(Entity e, T& c) {
    throw std::runtime_error("Unsupported onComponentAdded method for component type " + std::string{entt::type_id<T>().name()});
}

template <typename T>
void SceneManager::onComponentCloned(Entity e, T& c) {
    throw std::runtime_error("Unsupported onComponentCloned method for component type " + std::string{entt::type_id<T>().name()});
}

Entity SceneManager::cloneEntity(Entity e, uint32_t uuid) {
    using namespace entt::literals;

    Entity dst = {m_registry.create(entt::entity{uuid}), this};

    for (auto&& curr : m_registry.storage()) {
        if (auto& storage = curr.second; storage.contains(e)) {
            storage.push(dst, storage.value(e));
            entt::meta_type cType = entt::resolve(storage.type());
            entt::meta_any cData = cType.construct(storage.value(e));
            entt::meta_func triggerEventFunc = cType.func("onComponentCloned"_hs);
            if (triggerEventFunc) {
                triggerEventFunc.invoke({}, e, cData);
            }
        }
    }
    return dst;
}

void SceneManager::clearEntity(Entity& e) {
    uint32_t uuid = entt::to_integral(static_cast<entt::entity>(e));
    m_registry.destroy(e);
    m_registry.create(entt::entity{uuid});
    e.add<CUUID>(uuid);
}

SceneManager::SceneManager(std::weak_ptr<AssetsManager> am, std::weak_ptr<Renderer> r) : m_entityFactory(am), m_assetsManager(am), m_renderer(r) {
    CORE_INFO("Initializing scene manager...");
    CORE_INFO("Registering engine components...");
    registerComponents();
    CORE_INFO("Scene manager created!");
}

void SceneManager::onEvent(Event& e) {
    eventSystem(std::ref(m_registry), e);
}

void SceneManager::onUpdate(Time ts, std::weak_ptr<Renderer> r) {
    updateSystem(std::ref(m_registry), r, ts);
}
}