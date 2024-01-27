#pragma once

#include <entt/entt.hpp>

#include "assets/assetsManager.h"
#include "assets/prefab.h"
#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

class EntityFactory {
  public:
    using Prototypes = std::map<std::string, entt::entity, NumericComparator>;

    void createPrototypes(
      std::string_view prefab_name,
      const std::vector<std::string>& prototypeIDs, entt::registry& registry,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void updatePrototypes(
      std::string_view prefab_name,
      const std::vector<std::string>& prototypeIDs, entt::registry& registry,
      const std::unique_ptr<assets::AssetsManager>& assets_manager);
    void destroyPrototypes(std::string_view prefab_name,
                           const std::vector<std::string>& prototypeIDs,
                           entt::registry& registry);
    Prototypes
    getPrototypes(std::string_view prefab_name,
                  const std::vector<std::string>& prototypeIDs);
    bool containsPrototypes(std::string_view prefab_name,
                            const std::vector<std::string>& prototypeIDs) const;

    const std::map<std::string, Prototypes, NumericComparator>&
    getAllPrototypes();
    const std::map<std::string, std::string, NumericComparator>&
    getPrototypesCountByPrefab();
    // does not destroy entt entities, just clears the map
    void clearPrototypes();

  private:
    std::map<std::string, Prototypes, NumericComparator> m_prefabs;
    std::map<std::string, std::string, NumericComparator>
      m_prototypesCountByPrefab;
    bool m_dirty{};
};
}