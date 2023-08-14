#pragma once

#include <entt/entt.hpp>

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/core/time.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/scene/efactory.h"
#include "potatoengine/utils/uuid.h"

namespace potatoengine {
class Entity;

class Scene {
   public:
    Scene(std::weak_ptr<AssetsManager> am);
    Entity create(assets::PrefabID id, const std::optional<uint64_t>& uuid = std::nullopt);
    Entity clone(Entity e);
    void destroy(Entity e);
    void print() noexcept;
    void onUpdate(Time ts, std::weak_ptr<Renderer> r);
    Entity getEntity(std::string_view name);
    Entity getEntity(UUID uuid);
    void registerPrefabs();

    template <typename T>
    void onComponentAdded(Entity e, T& c);

   private:
    Efactory m_efactory;
    entt::registry m_registry;

    friend class Entity;
};
}