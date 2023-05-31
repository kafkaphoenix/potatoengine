#include "potatoengine/scene/scene.h"

#include "potatoengine/scene/components.h"
#include "potatoengine/scene/entity.h"
#include "potatoengine/utils/uuid.h"

namespace potatoengine {

Entity Scene::create(const std::string &name) {
    UUID id = UUID();
    Entity entity = {m_registry.create(), this};
    entity.add<ID>(id);
    entity.add<Name>(name);

    m_entities[id] = entity;
    CORE_INFO("Created entity {0} id: {1}", name, id);

    return entity;
}

void Scene::destroy(Entity entity) {
    m_entities.erase(entity.get<ID>().id);
    m_registry.destroy(entity);
}

Entity Scene::createFromPrefab(const std::string &name) {
    return create(name);
}

}