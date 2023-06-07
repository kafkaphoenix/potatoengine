#pragma once

#include <entt/entt.hpp>

#include "potatoengine/core/assetsManager.h"
#include "potatoengine/scene/prefab.h"

namespace potatoengine {
class Entity;

class Efactory {
   public:
    Efactory(const std::shared_ptr<AssetsManager>& am);
    void create(const asset::prefab::ID p, Entity e);
    void destroy(const asset::prefab::ID p, entt::registry& r);
    void update(const asset::prefab::ID p, Entity e, entt::registry& r);
    entt::entity get(const asset::prefab::ID p);

   private:
    std::unordered_map<asset::prefab::ID, entt::entity> m_protos;
    std::shared_ptr<AssetsManager> m_assetsManager;
};
}