#pragma once

#include <entt/entt.hpp>

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/assets/prefab.h"

namespace potatoengine {
class Entity;

class Efactory {
   public:
    Efactory(std::weak_ptr<AssetsManager> am);
    void create(assets::PrefabID id, Entity e);
    void destroy(assets::PrefabID id, entt::registry& r);
    void update(assets::PrefabID id, Entity e, entt::registry& r);
    entt::entity get(assets::PrefabID id);

   private:
    std::unordered_map<assets::PrefabID, entt::entity> m_protos;
    std::weak_ptr<AssetsManager> m_assetsManager;

    void loadModel(const std::string& filepath, entt::meta_any& ec);
    void loadAtlas(const std::string& filepath, entt::meta_any& ec);
};
}