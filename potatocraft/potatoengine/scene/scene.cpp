#include "potatoengine/scene/scene.h"

#include "potatoengine/scene/components/camera/cCamera.h"
#include "potatoengine/scene/components/common/cName.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/utils/cDeleted.h"
#include "potatoengine/scene/entity.h"
#include "potatoengine/scene/systems/sUpdate.h"
#include "potatoengine/scene/utils.h"

namespace potatoengine {

Entity Scene::create(assets::PrefabID id, const std::optional<uint32_t>& uuid) {
    UUID _uuid = uuid.has_value() ? UUID(uuid.value()) : UUID();
    Entity e = clone({m_entityFactory.get(id), this}, _uuid);
    e.add<CUUID>(_uuid);

    return e;
}

void Scene::remove(Entity& e) {
    e.add<CDeleted>();
}

void Scene::print() {
    printScene(std::ref(m_registry));
}

Entity Scene::getEntity(std::string_view name) {
    for (const auto& [e, cName, _] : m_registry.view<CName, CUUID>().each()) {
        if (cName.name == name) {
            return Entity(e, this);
        }
    }
    throw std::runtime_error("Entity not found");
}

Entity Scene::getEntity(UUID& uuid) {
    for (const auto& [e, cUUID] : m_registry.view<CUUID>().each()) {
        if (cUUID.uuid == uuid) {
            return Entity(e, this);
        }
    }
    throw std::runtime_error("Entity not found");
}

void Scene::createPrefab(assets::PrefabID id) {
    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager not found!");
    }

    if (manager->contains<Prefab>(id)) {
        m_entityFactory.create(id, {m_registry.create(), this});
    }
}

void Scene::createPrefabs() {  // TODO this should be a gui option
    for (const auto& p : assets::enum_range(assets::PrefabID::Player, assets::PrefabID::Sun)) {
        createPrefab(p);
    }
}

void Scene::updatePrefab(assets::PrefabID id) {
    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager not found!");
    }

    if (manager->contains<Prefab>(id)) {
        m_entityFactory.update(id, {m_registry.create(), this}, std::ref(m_registry));
    }
}

void Scene::updatePrefabs() {  // TODO this should be a gui option
    for (const auto& p : assets::enum_range(assets::PrefabID::Player, assets::PrefabID::Sun)) {
        updatePrefab(p);
    }
}

template <typename T>
void Scene::onComponentAdded(Entity e, T& c) {
    throw std::runtime_error("Unsupported onComponentAdded method for component type " + std::string{entt::type_id<T>().name()});
}

template <typename T>
void Scene::onComponentCloned(Entity e, T& c) {
    throw std::runtime_error("Unsupported onComponentCloned method for component type " + std::string{entt::type_id<T>().name()});
}

Entity Scene::clone(Entity e, uint32_t uuid) {
    using namespace entt::literals;
    Entity dst = {m_registry.create(entt::entity{uuid}), this};
    for (auto&& curr : m_registry.storage()) {
        if (auto& storage = curr.second; storage.contains(e)) {
            storage.push(dst, storage.value(e));
            auto cType = entt::resolve(storage.type());
            auto cData = cType.construct(storage.value(e));
            auto triggerEventFunc = cType.func("onComponentCloned"_hs);
            if (triggerEventFunc) {
                triggerEventFunc.invoke({}, e, cData);
            }
        }
    }
    return dst;
}

void Scene::clear(Entity& e) {
    uint32_t uuid = entt::to_integral(static_cast<entt::entity>(e));
    m_registry.destroy(e);
    m_registry.create(entt::entity{uuid});
    e.add<CUUID>(uuid);
}

Scene::Scene(std::weak_ptr<AssetsManager> am) : m_entityFactory(am), m_assetsManager(am) {
#ifdef DEBUG
    CORE_INFO("Creating scene...");
    CORE_INFO("Registering engine components...");
#endif
    registerComponents();
#ifdef DEBUG
    CORE_INFO("Scene created!");
#endif
}

void Scene::onUpdate(Time ts, std::weak_ptr<Renderer> r) {
    updateSystem(std::ref(m_registry), r, ts);
}
}