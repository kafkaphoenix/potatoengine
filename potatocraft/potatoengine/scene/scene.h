#pragma once

#include <entt/entt.hpp>

namespace potatoengine {
class Entity;

class Scene {
   public:
    Entity create(const std::string &name);
    void destroy(Entity entity);
    Entity createFromPrefab(const std::string &name);

   private:
    entt::registry m_registry;
    std::unordered_map<uint32_t, entt::entity> m_entities; // std::unique_ptr would be a terrible idea here

    friend class Entity;
};
}