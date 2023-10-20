#include "potatoengine/scene/sceneManager.h"

#include "potatoengine/scene/components/camera/cCamera.h"
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
#include "potatoengine/scene/systems/sUpdate.h"
#include "potatoengine/scene/utils.h"
#include "potatoengine/utils/timer.h"

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

void SceneManager::createScene(std::string_view id) {
#ifdef DEBUG
    CORE_INFO("Creating scene...");
    Timer timer;
#endif
    if (m_activeScene == id) {
        throw std::runtime_error("Scene " + std::string{id} + " is already active!");
    }

    if (not m_loadedScenes.contains(id.data())) {
        throw std::runtime_error("Scene " + std::string{id} + " is not loaded!");
    }

    if (not m_activeScene.empty()) {
        clearScene(m_activeScene);
    }
#ifdef DEBUG
    CORE_INFO("Linking scene shaders...");
#endif
    const auto& renderer = m_renderer.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer not found!");
    }

    const auto& loadedScene = m_loadedScenes.at(id.data());
    for (std::string id : loadedScene.getLoadedShaders()) {
        renderer->addShader(std::move(id));
    }

#ifdef DEBUG
    CORE_INFO("Creating scene prefabs...");
#endif
    for (std::string_view id : loadedScene.getLoadedPrefabs()) {
        createPrefab(id);
    }

#ifdef DEBUG
    CORE_INFO("Creating scene entities...");
#endif
    for (const auto& [name, data] : loadedScene.getLoadedEntities()) {
        std::string_view prefab = data.at("prefab").get<std::string_view>();
        Entity e = createEntity(prefab);
        e.add<CName>(std::string(name));
        e.add<CTag>(prefab.data());
        if (data.contains("options")) {
            json options = data.at("options");
            if (prefab == "skybox") {
                if (options.contains("time")) {
                    e.get<CTime>().setTime(options.at("time").get<float>());
                }
                if (options.contains("acceleration")) {
                    e.get<CTime>().acceleration = options.at("acceleration").get<float>();
                }
                if (options.contains("useFog")) {
                    e.get<CSkybox>().useFog = options.at("useFog").get<bool>();
                }
                continue;
            }
            if (options.contains("position")) {
                json position = options.at("position");
                e.get<CTransform>().position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
            }
            if (options.contains("rotation")) {
                json rotation = options.at("rotation");
                glm::vec3 rot = {rotation.at("x").get<float>(), rotation.at("y").get<float>(), rotation.at("z").get<float>()};
                e.get<CTransform>().rotation = glm::quat(glm::radians(rot));
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
            if (options.contains("filepaths")) {
                const auto& manager = m_assetsManager.lock();
                if (not manager) {
                    throw std::runtime_error("Assets manager is null!");
                }

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
            if (options.contains("useLighting")) {
                e.get<CTexture>().useLighting = options.at("useLighting").get<bool>();
            }
            if (options.contains("drawMode")) {
                e.get<CTexture>()._drawMode = options.at("drawMode").get<std::string>();
                e.get<CTexture>().setDrawMode();
            }
            if (options.contains("textureAtlas")) {
                CTextureAtlas& c = e.get<CTextureAtlas>();
                json textureAtlasOptions = options.at("textureAtlas");
                if (textureAtlasOptions.contains("index")) {
                    c.index = textureAtlasOptions.at("index").get<int>();
                }
                if (textureAtlasOptions.contains("rows")) {
                    c.rows = textureAtlasOptions.at("rows").get<int>();
                }
            }
            if (options.contains("scale")) {
                json scale = options.at("scale");
                e.get<CTransform>().scale = {scale.at("x").get<float>(), scale.at("y").get<float>(), scale.at("z").get<float>()};
            }
            if (prefab == "chunk_config") {
                if (options.contains("chunkSize")) {
                    e.get<CChunkManager>().chunkSize = options.at("chunkSize").get<int>();
                }
                if (options.contains("blockSize")) {
                    e.get<CChunkManager>().blockSize = options.at("blockSize").get<int>();
                }
                if (options.contains("width")) {
                    e.get<CChunkManager>().width = options.at("width").get<int>();
                }
                if (options.contains("height")) {
                    e.get<CChunkManager>().height = options.at("height").get<int>();
                }
                if (options.contains("meshType")) {
                    e.get<CChunkManager>()._meshType = options.at("meshType").get<std::string>();
                    e.get<CChunkManager>().setMeshType();
                }
                if (options.contains("meshAlgorithm")) {
                    e.get<CChunkManager>()._meshAlgorithm = options.at("meshAlgorithm").get<std::string>();
                    e.get<CChunkManager>().setMeshAlgorithm();
                }
                if (options.contains("useBiomes")) {
                    e.get<CChunkManager>().useBiomes = options.at("useBiomes").get<bool>();
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
        }
    }

#ifdef DEBUG
    CORE_INFO("Creating scene lights...");
#endif
    for (const auto& [name, data] : loadedScene.getLoadedLights()) {
        std::string_view prefab = data.at("prefab").get<std::string_view>();
        Entity e = createEntity(prefab);
        e.add<CName>(std::string(name));
        e.add<CTag>(prefab.data());
        if (data.contains("options")) {
            json options = data.at("options");
            if (options.contains("position")) {
                json position = options.at("position");
                e.get<CTransform>().position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
            }
            if (options.contains("scale")) {
                json scale = options.at("scale");
                e.get<CTransform>().scale = {scale.at("x").get<float>(), scale.at("y").get<float>(), scale.at("z").get<float>()};
            }
            if (options.contains("intensity")) {
                e.get<CLight>().intensity = options.at("intensity").get<float>();
            }
            if (options.contains("color")) {
                json color = options.at("color");
                e.get<CLight>().color = {color.at("r").get<float>(), color.at("g").get<float>(), color.at("b").get<float>()};
            }
            if (options.contains("rotation")) {
                json rotation = options.at("rotation");
                glm::vec3 rot = {rotation.at("x").get<float>(), rotation.at("y").get<float>(), rotation.at("z").get<float>()};
                e.get<CTransform>().rotation = glm::quat(glm::radians(rot));
            }
        }
    }

#ifdef DEBUG
    CORE_INFO("Creating scene cameras...");
#endif
    for (const auto& [name, data] : loadedScene.getLoadedCameras()) {
        std::string_view prefab = data.at("prefab").get<std::string_view>();
        Entity e = createEntity(prefab);
        e.add<CName>(std::string(name));
        e.add<CTag>(prefab.data());
        if (data.contains("options")) {
            json options = data.at("options");
            if (options.contains("position")) {
                json position = options.at("position");
                e.get<CTransform>().position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
            }
            if (options.contains("rotation")) {
                json rotation = options.at("rotation");
                glm::vec3 rot = {rotation.at("x").get<float>(), rotation.at("y").get<float>(), rotation.at("z").get<float>()};
                e.get<CTransform>().rotation = glm::quat(glm::radians(rot));
            }
            // e.get<CCamera>().fov = options.at("fov").get<float>();
            // e.get<CCamera>().near = options.at("near").get<float>();
            // e.get<CCamera>().far = options.at("far").get<float>();
        }
    }

#ifdef DEBUG
    CORE_INFO("Creating scene systems...");
#endif
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

#ifdef DEBUG
    CORE_INFO("Creating scene FBOs...");
#endif
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

    m_registry.sort<CDistanceFromCamera>([](const CDistanceFromCamera& lhs, const CDistanceFromCamera& rhs) {
        return lhs.distance < rhs.distance;
    });

    m_registry.sort<CUUID, CDistanceFromCamera>();

    m_activeScene = id;
#ifdef DEBUG
    CORE_INFO("Scene creation TIME: {}", timer.getSeconds());
#endif
}

void SceneManager::updateScene(std::string_view id) {
    // TODO: implement
}

void SceneManager::clearScene(std::string_view id) {
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
#ifdef DEBUG
    CORE_INFO("Initializing scene manager...");
    CORE_INFO("Registering engine components...");
#endif
    registerComponents();
#ifdef DEBUG
    CORE_INFO("Scene manager created!");
#endif
}

void SceneManager::onUpdate(Time ts, std::weak_ptr<Renderer> r) {
    updateSystem(std::ref(m_registry), r, ts);
}
}