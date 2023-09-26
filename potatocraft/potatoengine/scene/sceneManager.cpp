#include "potatoengine/scene/sceneManager.h"

#include "potatoengine/scene/components/camera/cCamera.h"
#include "potatoengine/scene/components/common/cName.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/utils/cDeleted.h"
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

    if (m_activeScene != "") {
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
       renderer->add(std::move(id));
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
        Entity e = createEntity(data.at("prefab").get<std::string_view>());
        e.add<CName>(std::string(name));
        if (data.contains("options")) {
            json options = data.at("options");
            json position = options.at("position");
            e.get<CTransform>().position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
        }
    }

#ifdef DEBUG
    CORE_INFO("Creating scene lights...");
#endif
    for (const auto& [name, data] : loadedScene.getLoadedLights()) {
        Entity e = createEntity(data.at("prefab").get<std::string_view>());
        e.add<CName>(std::string(name));
        if (data.contains("options")) {
            json options = data.at("options");
            json position = options.at("position");
            CTransform& transform = e.get<CTransform>();
            transform.position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
            json color = options.at("color");
            e.get<CLight>().color = {color.at("r").get<float>(), color.at("g").get<float>(), color.at("b").get<float>()};
            json rotation = options.at("rotation");
            glm::vec3 rot = {rotation.at("x").get<float>(), rotation.at("y").get<float>(), rotation.at("z").get<float>()};
            transform.rotation = glm::quat(glm::radians(rot));
        }
    }

#ifdef DEBUG
    CORE_INFO("Creating scene cameras...");
#endif
    for (const auto& [name, data] : loadedScene.getLoadedCameras()) {
        Entity e = createEntity(data.at("prefab").get<std::string_view>());
        e.add<CName>(std::string(name));
        if (data.contains("options")) {
            json options = data.at("options");
            json position = options.at("position");
            CTransform& transform = e.get<CTransform>();
            transform.position = {position.at("x").get<float>(), position.at("y").get<float>(), position.at("z").get<float>()};
            json rotation = options.at("rotation");
            glm::vec3 rot = {rotation.at("x").get<float>(), rotation.at("y").get<float>(), rotation.at("z").get<float>()};
            transform.rotation = glm::quat(glm::radians(rot));
            // e.get<CCamera>().fov = options.at("fov").get<float>();
            // e.get<CCamera>().near = options.at("near").get<float>();
            // e.get<CCamera>().far = options.at("far").get<float>();
        }
    }
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