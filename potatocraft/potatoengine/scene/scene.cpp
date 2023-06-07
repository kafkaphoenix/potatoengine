#include "potatoengine/scene/scene.h"

#include "potatoengine/scene/components.h"
#include "potatoengine/scene/entity.h"
#include "potatoengine/scene/systems.h"
#include "potatoengine/scene/utils.h"

namespace potatoengine {

Entity Scene::create(const asset::prefab::ID p, const std::optional<uint64_t>& uuid) {
    Entity e = clone({m_efactory.get(p), this});
    UUID _uuid = uuid.has_value() ? UUID(uuid.value()) : UUID();
    e.add<UUIDComponent>(_uuid);

    return e;
}

Entity Scene::clone(Entity e) {
    Entity dst = {m_registry.create(), this};
    for (auto&& curr : m_registry.storage()) {
        if (auto& storage = curr.second; storage.contains(e)) {
            storage.emplace(dst, storage.get(e));
        }
    }
    return dst;
}

void Scene::destroy(Entity e) {
    e.add<Deleted>();
}

void Scene::print() {
    printScene(std::ref(m_registry));
}

template <typename T>
void Scene::onComponentAdded(Entity e, T& c) {}

template <>
void Scene::onComponentAdded(Entity e, CameraComponent& c) {
}

void Scene::registerPrefabs() {
    for (const auto p : asset::enum_range(asset::prefab::ID::Player, asset::prefab::ID::BrickBlock)) {
        m_efactory.create(p, {m_registry.create(), this});
    }
}

Scene::Scene(const std::shared_ptr<AssetsManager>& am) : m_assetsManager(am), m_efactory(am) {
#ifdef DEBUG
    CORE_INFO("Creating scene...");
    CORE_INFO("Registering components...");
#endif
    registerComponents();
#ifdef DEBUG
    CORE_INFO("Registering prefabs...");
#endif
    registerPrefabs();
#ifdef DEBUG
    CORE_INFO("Scene created!");
#endif
}

void Scene::onUpdate(Time ts) {
    updateSystems(std::ref(m_registry), ts);
}
}