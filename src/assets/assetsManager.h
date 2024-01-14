#pragma once

#include "assets/asset.h"
#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine::assets {
class AssetsManager {
  public:
    template <typename Type, typename... Args>
    void load(std::string_view id, Args&&... args) {
      std::string_view type = typeid(Type).name();
      type = type.substr(type.find_last_of(':') + 1);
      auto& asset_map = m_assets[type.data()];
      ENGINE_ASSERT(not asset_map.contains(id.data()),
                    "Asset {} already exists for type {}!", id, type);
      asset_map.emplace(id, std::make_shared<Type>(std::forward<Args>(args)...));
      m_dirty = true;
    }

    template <typename Type> bool contains(std::string_view id) const {
      std::string_view type = typeid(Type).name();
      type = type.substr(type.find_last_of(':') + 1);
      ENGINE_ASSERT(m_assets.contains(type.data()),
                    "Type {} not found in assets manager!", type);
      return m_assets.at(type.data()).contains(id.data());
    }

    template <typename Type>
    std::shared_ptr<Type> get(std::string_view id) const {
      std::string_view type = typeid(Type).name();
      type = type.substr(type.find_last_of(':') + 1);
      ENGINE_ASSERT(contains<Type>(id), "Asset {} not found for type {}!", id,
                    type);
      return std::static_pointer_cast<Type>(
        m_assets.at(type.data()).at(id.data())); // I know the type is correct
    }

    template <typename Type, typename... Args>
    std::shared_ptr<Type> reload(std::string_view id, Args&&... args) {
      std::string_view type = typeid(Type).name();
      type = type.substr(type.find_last_of(':') + 1);
      ENGINE_ASSERT(contains<Type>(id), "Asset {} not found for type {}!", id,
                    type);
      auto& maybeAsset = m_assets.at(type.data()).at(id.data());
      std::shared_ptr<Asset> asset =
        std::make_shared<Type>(std::forward<Args>(args)...);
      maybeAsset = std::move(asset);

      m_dirty = true;
      ENGINE_TRACE("Reloaded asset {}", id);
      return std::static_pointer_cast<Type>(maybeAsset); // I know the type is correct
    }

    void clear() noexcept {
      m_assets.clear();
      m_metrics.clear();
      m_dirty = false;
    }

    static std::unique_ptr<AssetsManager> Create() {
      return std::make_unique<AssetsManager>();
    }

    const std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Asset>>>&
    getAssets() const noexcept {
      return m_assets;
    }

    const std::map<std::string, std::string, NumericComparator>& getMetrics() {
      if (not m_dirty) {
        return m_metrics;
      }

      m_metrics.clear();
      for (const auto& [key, value] : m_assets) {
        m_metrics.emplace(key, std::to_string(value.size()));
      }
      m_dirty = false;

      return m_metrics;
    }

  private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Asset>>>
      m_assets;
    std::map<std::string, std::string, NumericComparator> m_metrics;
    bool m_dirty{};
};
}