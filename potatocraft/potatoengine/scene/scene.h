#pragma once

#include <entt/entt.hpp>

#include "potatoengine/core/assetsManager.h"
#include "potatoengine/scene/efactory.h"
#include "potatoengine/utils/uuid.h"
#include "potatoengine/core/time.h"


namespace potatoengine {
class Entity;

class Scene {
   public:
    Scene(const std::shared_ptr<AssetsManager>& am);
    Entity create(const asset::prefab::ID p, const std::optional<uint64_t>& uuid = std::nullopt);
    Entity clone(Entity e);
    void destroy(Entity e);
    void registerPrefabs();
    void print();
    void onUpdate(Time ts);

    template <typename T>
    void onComponentAdded(Entity e, T& component);

   private:
    std::shared_ptr<AssetsManager> m_assetsManager;
    Efactory m_efactory;
    entt::registry m_registry;

    friend class Entity;
};
}