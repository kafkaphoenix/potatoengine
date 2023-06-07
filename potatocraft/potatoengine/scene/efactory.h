#pragma once

#include "potatoengine/core/assetsManager.h"
#include "potatoengine/scene/prefab.h"

namespace potatoengine {
class Entity;

class Efactory {
   public:
    Efactory(const std::shared_ptr<AssetsManager>& am);
    void create(asset::prefab::ID t, Entity& e);
    void destroy(asset::prefab::ID t);
    void update(asset::prefab::ID t);

   private:
    std::unordered_map<std::string, Entity> m_protos;
    std::shared_ptr<AssetsManager> m_assetsManager;
};
}