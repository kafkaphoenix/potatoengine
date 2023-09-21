#pragma once

#include <entt/entt.hpp>

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/assets/prefab.h"

namespace potatoengine {
class Entity;

class EntityFactory {
   public:
    EntityFactory(std::weak_ptr<AssetsManager> am);
    void create(assets::PrefabID id, Entity e);
    void destroy(assets::PrefabID id, entt::registry& r);
    void update(assets::PrefabID id, Entity e, entt::registry& r);
    entt::entity get(assets::PrefabID id);
    bool contains(assets::PrefabID id) const noexcept { return m_prototypes.contains(id); }

   private:
    std::unordered_map<assets::PrefabID, entt::entity> m_prototypes;
    std::weak_ptr<AssetsManager> m_assetsManager;
};
}