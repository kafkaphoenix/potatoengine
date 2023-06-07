#include "potatoengine/scene/scene.h"

#include "potatoengine/scene/components.h"
#include "potatoengine/scene/entity.h"

namespace potatoengine {

Scene::Scene(const std::shared_ptr<AssetsManager>& am) : m_assetsManager(am), m_efactory(am) {}

void Scene::create(asset::prefab::ID t, const std::optional<uint64_t>& uuid) {
    if (m_entities >= m_maxEntities) {
        throw std::runtime_error("Scene has reached max entities!");
    }
    Entity e = {m_registry.create(), this};
    UUID _uuid = uuid.has_value() ? UUID(uuid.value()) : UUID();
    e.add<UUIDComponent>(_uuid);

    m_efactory.create(t, e);
    ++m_entities;
}

void Scene::destroy(Entity e) {
    m_registry.destroy(e);
    --m_entities;
}

void Scene::print() {
    CORE_INFO("Scene entities:");
    m_registry.view<UUIDComponent>().each([this](auto e, auto& uuid) {
        CORE_INFO("\tEntity UUID: {0}", uuid.uuid);

        for (auto&& curr : m_registry.storage()) {
            auto& storage = curr.second;
            if (storage.contains(e)) {
                entt::id_type cid = curr.first;
                entt::type_info ctype = storage.type();
                std::string_view cname = ctype.name().substr(ctype.name().find_last_of(':') + 1);
                CORE_INFO("\t\tComponent {0} ID: {1}", cname, cid);

                void* data = storage.get(e);
                if (cname == "Name") {
                    Name* name = static_cast<Name*>(data);
                    CORE_INFO("\t\t\tData: {0}", name->name);
                }

                if (cname == "UUIDComponent") {
                    UUIDComponent* uuid = static_cast<UUIDComponent*>(data);
                    CORE_INFO("\t\t\tData: {0}", uuid->uuid);
                }
            }
        }
    });
}
}