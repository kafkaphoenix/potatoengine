#pragma once

#include <entt/entt.hpp>

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/assets/prefab.h"

namespace potatoengine {
class Entity;

class EntityFactory {
   public:
    EntityFactory(std::weak_ptr<AssetsManager> am);
    void create(std::string_view id, Entity e);
    void destroy(std::string_view id, entt::registry& r);
    void update(std::string_view id, Entity e, entt::registry& r);
    entt::entity get(std::string_view id);
    bool contains(std::string_view id) const noexcept { return m_prototypes.contains(id.data()); }
    void clear();

   private:
    std::unordered_map<std::string, entt::entity> m_prototypes;
    std::weak_ptr<AssetsManager> m_assetsManager;
};
}