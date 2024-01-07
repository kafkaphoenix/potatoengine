#pragma once

#include <entt/entt.hpp>

#include "assets/prefab.h"
#include "utils/numericComparator.h"

namespace potatoengine {
class Entity;

class EntityFactory {
  public:
    using Prototypes = std::map<std::string, entt::entity, NumericComparator>;

    void createPrototypes(std::string_view prefabID, Entity&& e);
    void updatePrototypes(std::string_view prefabID, Entity&& e);
    void destroyPrototypes(std::string_view prefabID, entt::registry& r);
    const Prototypes& getPrototypes(std::string_view prefabID);
    const std::map<std::string, Prototypes, NumericComparator>&
    getAllPrototypes();
    bool containsPrototypes(std::string_view prefabID) const;

    void createPrototype(std::string_view prefabID,
                         std::string_view prototypeID, Entity&& e);
    void updatePrototype(std::string_view prefabID,
                         std::string_view prototypeID, Entity&& e);
    void destroyPrototype(std::string_view prefabID,
                          std::string_view prototypeID, entt::registry& r);
    entt::entity getPrototype(std::string_view prefabID,
                              std::string_view prototypeID);
    bool containsPrototype(std::string_view prefabID,
                           std::string_view prototypeID) const;

    const std::map<std::string, std::string, NumericComparator>&
    getPrototypesCountByPrefab();
    // does not destroy entt entities, just clears the map
    void clear();

  private:
    std::map<std::string, Prototypes, NumericComparator> m_prefabs;
    std::map<std::string, std::string, NumericComparator>
      m_prototypesCountByPrefab;
    bool m_dirty{};
};
}