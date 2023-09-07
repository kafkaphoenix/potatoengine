#include "potatoengine/scene/scene.h"

#include "potatoengine/scene/components.h"
#include "potatoengine/scene/entity.h"
#include "potatoengine/scene/systems.h"
#include "potatoengine/scene/utils.h"

namespace potatoengine {

Entity Scene::create(assets::PrefabID id, const std::optional<uint64_t>& uuid) {
    Entity e = clone({m_efactory.get(id), this});
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

void Scene::print() noexcept {
    printScene(std::ref(m_registry));
}

Entity Scene::getEntity(std::string_view name) {
    for (auto&& [e, c] : m_registry.view<Name>().each()) {
        if (c.name == name) {
            return Entity(e, this);
        }
     }
     throw std::runtime_error("Entity not found");
}

Entity Scene::getEntity(UUID uuid) {
    for (auto&& [e, c] : m_registry.view<UUIDComponent>().each()) {
        if (c.uuid == uuid) {
            return Entity(e, this);
        }
    }
    throw std::runtime_error("Entity not found");
}

void Scene::registerPrefabs() {
    for (const auto p : assets::enum_range(assets::PrefabID::Player, assets::PrefabID::WoodBlock)) {
        m_efactory.create(p, {m_registry.create(), this});
    }
}

template <typename T>
void Scene::onComponentAdded(Entity e, T& c) {}

template <>
void Scene::onComponentAdded(Entity e, CameraComponent& c) {
}

Scene::Scene(std::weak_ptr<AssetsManager> am) : m_efactory(am) {
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

void Scene::onUpdate(Time ts, std::weak_ptr<Renderer> r) {
    updateSystems(std::ref(m_registry), r, ts);
}
}