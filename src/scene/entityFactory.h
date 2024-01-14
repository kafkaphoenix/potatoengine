#pragma once

#include <entt/entt.hpp>

#include "assets/assetsManager.h"
#include "utils/numericComparator.h"

namespace potatoengine {

class EntityFactory {
  public:
    using Prototypes = std::map<std::string, entt::entity, NumericComparator>;

    void createPrototypes(std::string_view prefabID, entt::registry& registry, const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void updatePrototypes(std::string_view prefabID, entt::registry& registry, const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void destroyPrototypes(std::string_view prefabID, entt::registry& registry);
    const Prototypes& getPrototypes(std::string_view prefabID);
    const std::map<std::string, Prototypes, NumericComparator>&
    getAllPrototypes();
    bool containsPrototypes(std::string_view prefabID) const;

    void createPrototype(std::string_view prefabID,
                         std::string_view prototypeID,
                         entt::registry& registry, const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void updatePrototype(std::string_view prefabID,
                         std::string_view prototypeID,
                         entt::registry& registry, const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void destroyPrototype(std::string_view prefabID,
                          std::string_view prototypeID,
                          entt::registry& registry);
    entt::entity getPrototype(std::string_view prefabID,
                              std::string_view prototypeID);
    bool containsPrototype(std::string_view prefabID,
                           std::string_view prototypeID) const;

    const std::map<std::string, std::string, NumericComparator>&
    getPrototypesCountByPrefab();
    // does not destroy entt entities, just clears the map
    void clear();

    // template <typename Component, typename... Args>
    // Component& update(Args&&... args);

    // template <typename Component, typename... Args>
    // Component& add(Args&&... args);

  private:
    std::map<std::string, Prototypes, NumericComparator> m_prefabs;
    std::map<std::string, std::string, NumericComparator>
      m_prototypesCountByPrefab;
    bool m_dirty{};
};
}