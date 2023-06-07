#pragma once

#include <entt/entt.hpp>

#include "potatoengine/core/assetsManager.h"
#include "potatoengine/scene/efactory.h"
#include "potatoengine/utils/uuid.h"

namespace potatoengine {
class Entity;

class Scene {
   public:
    Scene(const std::shared_ptr<AssetsManager>& am);
    void create(asset::prefab::ID t, const std::optional<uint64_t>& uuid = std::nullopt);
    void destroy(Entity e);
    void print();

   private:
    std::shared_ptr<AssetsManager> m_assetsManager;
    Efactory m_efactory;
    entt::registry m_registry;
    const int64_t m_maxEntities = 1000;
    int64_t m_entities = 0;

    friend class Entity;
};
}