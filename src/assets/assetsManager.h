#pragma once

#include "assets/model.h"
#include "assets/prefab.h"
#include "assets/scene.h"
#include "assets/shader.h"
#include "assets/texture.h"
#include "pch.h"

namespace potatoengine {
class AssetsManager {
  public:
    template <typename Type, typename... Args>
    void load(std::string_view id, Args&&... args) {
      ENGINE_ASSERT(not m_assets.contains(id.data()),
                    "Asset {} already exists!", id);
      m_assets.emplace(id, std::make_shared<Type>(std::forward<Args>(args)...));
      m_isDirty = true;
    }

    template <typename Type> bool contains(std::string_view id) const {
      return m_assets.contains(id.data()) and
             std::holds_alternative<std::shared_ptr<Type>>(
               m_assets.at(id.data()));
    }

    template <typename Type> std::shared_ptr<Type>& get(std::string_view id) {
      ENGINE_ASSERT(contains<Type>(id), "Asset {} not found!", id);
      AssetTypes& asset = m_assets.at(id.data());
      m_isDirty = true;
      return *std::get_if<std::shared_ptr<Type>>(&asset);
    }

    template <typename Type>
    const std::shared_ptr<Type>& get(std::string_view id) const {
      ENGINE_ASSERT(contains<Type>(id), "Asset {} not found!", id);
      AssetTypes& asset = m_assets.at(id.data());
      return std::get_if<std::shared_ptr<Type>>(&asset);
    }

    template <typename Type, typename... Args>
    const std::shared_ptr<Type>& reload(std::string_view id, Args&&... args) {
      std::shared_ptr<Type> asset =
        std::make_shared<Type>(std::forward<Args>(args)...);

      auto& maybeAsset = m_assets.find(id.data());
      ENGINE_ASSERT(maybeAsset not_eq m_assets.end(), "Asset {} not found!",
                    id);
      ENGINE_ASSERT(
        std::holds_alternative<std::shared_ptr<Type>>(maybeAsset->second),
        "Asset {} has an incompatible type!", id);

      std::get<std::shared_ptr<Type>>(maybeAsset->second) = asset;

      m_isDirty = true;
      ENGINE_TRACE("Reloaded asset {}", id);
      return std::static_pointer_cast<Type>(
        std::get<std::shared_ptr<Type>>(m_assets.at(id.data())));
    }

    void clear() noexcept {
      m_assets.clear();
      m_assetsByType.clear();
      m_metrics.clear();
      m_isDirty = false;
    }

    static std::shared_ptr<AssetsManager> Create() {
      return std::make_shared<AssetsManager>();
    }

    using AssetTypes =
      std::variant<std::shared_ptr<Texture>, std::shared_ptr<Shader>,
                   std::shared_ptr<Prefab>, std::shared_ptr<Model>,
                   std::shared_ptr<Scene>>;
    const std::unordered_map<std::string, AssetTypes>&
    getAssets() const noexcept {
      return m_assets;
    }

    const std::unordered_map<std::string,
                             std::unordered_map<std::string, AssetTypes>>&
    getAssetsByType() {
      if (not m_isDirty) {
        return m_assetsByType;
      }

      m_assetsByType.clear();
      m_metrics.clear();
      int textures = 0;
      int shaders = 0;
      int prefabs = 0;
      int models = 0;
      int scenes = 0;
      for (const auto& [key, value] : m_assets) {
        if (std::holds_alternative<std::shared_ptr<Texture>>(value)) {
          m_assetsByType["Texture"].emplace(key, value);
          textures += 1;
        } else if (std::holds_alternative<std::shared_ptr<Shader>>(value)) {
          m_assetsByType["Shader"].emplace(key, value);
          shaders += 1;
        } else if (std::holds_alternative<std::shared_ptr<Prefab>>(value)) {
          m_assetsByType["Prefab"].emplace(key, value);
          prefabs += 1;
        } else if (std::holds_alternative<std::shared_ptr<Model>>(value)) {
          m_assetsByType["Model"].emplace(key, value);
          models += 1;
        } else if (std::holds_alternative<std::shared_ptr<Scene>>(value)) {
          m_assetsByType["Scene"].emplace(key, value);
          scenes += 1;
        }
      }
      m_metrics["Textures"] = std::to_string(textures);
      m_metrics["Shaders"] = std::to_string(shaders);
      m_metrics["Prefabs"] = std::to_string(prefabs);
      m_metrics["Models"] = std::to_string(models);
      m_metrics["Scenes"] = std::to_string(scenes);
      m_metrics["Total"] = std::to_string(m_assets.size());
      m_isDirty = false;
      return m_assetsByType;
    }

    const std::map<std::string, std::string>& getMetrics() {
      if (not m_isDirty) {
        return m_metrics;
      }

      getAssetsByType();

      return m_metrics;
    }

  private:
    std::unordered_map<std::string, AssetTypes> m_assets{};
    std::unordered_map<std::string, std::unordered_map<std::string, AssetTypes>>
      m_assetsByType{};
    std::map<std::string, std::string> m_metrics{};
    bool m_isDirty{false};
};
}