#pragma once

#include "potatoengine/assets/model.h"
#include "potatoengine/assets/prefab.h"
#include "potatoengine/assets/scene.h"
#include "potatoengine/assets/shader.h"
#include "potatoengine/assets/texture.h"
#include "potatoengine/pch.h"

namespace potatoengine {
class AssetsManager {
   public:
    template <typename Type, typename... Args>
    void load(std::string_view id, Args&&... args) {
        if (m_assets.contains(id.data())) {
            throw std::runtime_error("Asset " + std::string(id) + " already exists!");
        }
        m_assets.emplace(id, std::make_shared<Type>(std::forward<Args>(args)...));
#ifdef DEBUG
        CORE_INFO("\tLoaded asset {}", id);
#endif
    }

    template <typename Type>
    bool contains(std::string_view id) const {
        return m_assets.contains(id.data()) and std::holds_alternative<std::shared_ptr<Type>>(m_assets.at(id.data()));
    }

    template <typename Type>
    std::shared_ptr<Type>& get(std::string_view id) {
        if (not contains<Type>(id)) {
            throw std::runtime_error("Asset " + std::string(id) + " not found!");
        }
        AssetTypes& asset = m_assets.at(id.data());
        return *std::get_if<std::shared_ptr<Type>>(&asset);
    }

    template <typename Type>
    const std::shared_ptr<Type>& get(std::string_view id) const {
        if (not contains<Type>(id)) {
            throw std::runtime_error("Asset " + std::string(id) + " not found!");
        }
        AssetTypes& asset = m_assets.at(id.data());
        return std::get_if<std::shared_ptr<Type>>(&asset);
    }

    template <typename Type, typename... Args>
    const std::shared_ptr<Type>& reload(std::string_view id, Args&&... args) {
        std::shared_ptr<Type> asset = std::make_shared<Type>(std::forward<Args>(args)...);

        auto& maybeAsset = m_assets.find(id);
        if (maybeAsset not_eq m_assets.end() and std::holds_alternative<std::shared_ptr<Type>>(maybeAsset->second)) {
            std::get<std::shared_ptr<Type>>(maybeAsset->second) = asset;
        } else {
            throw std::runtime_error("Asset " + std::string(id) + " not found or has an incompatible type. Reload failed!");
        }
#ifdef DEBUG
        CORE_INFO("Reloaded asset {}", id);
#endif
        return std::static_pointer_cast<Type>(std::get<std::shared_ptr<Type>>(m_assets.at(id.data())));
    }

    void print() const noexcept {
        for (const auto& [id, asset] : m_assets) {
            std::string_view fp = std::visit([](const auto& asset) { return asset->getFilepath(); }, asset);
            CORE_INFO("Asset {0}: {1}", id, fp);
        }
    }

    static std::shared_ptr<AssetsManager> Create() noexcept {
        return std::make_shared<AssetsManager>();
    }

   private:
    using AssetTypes = std::variant<std::shared_ptr<Texture>, std::shared_ptr<Shader>, std::shared_ptr<Prefab>, std::shared_ptr<Model>, std::shared_ptr<Scene>>;
    std::unordered_map<std::string, AssetTypes> m_assets;
};
}