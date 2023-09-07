#pragma once

#include "potatoengine/assets/model.h"
#include "potatoengine/assets/prefab.h"
#include "potatoengine/assets/shader.h"
#include "potatoengine/assets/texture.h"
#include "potatoengine/assets/types.h"
#include "potatoengine/assets/utils.h"
#include "potatoengine/pch.h"

namespace potatoengine {
class AssetsManager {
   public:
    template <typename Type, typename ID, typename... Args>
    void load(ID id, Args&&... args) {
        std::shared_ptr<Type> asset = std::make_shared<Type>(std::forward<Args>(args)...);
        std::string _id = generateID<Type, ID>(id);
        m_assets.emplace(_id, std::move(asset));
#ifdef DEBUG
        CORE_INFO("\tLoaded asset {0}", _id);
#endif
    }

    template <typename Type, typename ID>
    bool exists(ID id) const {
        std::string _id = generateID<Type, ID>(id);
        return m_assets.find(_id) != m_assets.end();
    }

    template <typename Type, typename ID>
    std::shared_ptr<Type>& get(ID id) {
        if (!exists<Type>(id)) {
            if constexpr (std::is_enum_v<ID>) {
                throw std::runtime_error("Asset " + assets::to_string(id) + " not found!");
            } else if constexpr (std::is_same_v<ID, std::string>) {
                throw std::runtime_error("Asset " + id + " not found!");
            } else if constexpr (std::is_same_v<ID, const char*>) {
                throw std::runtime_error("Asset " + id + " not found!");
            }
        }

        std::string _id = generateID<Type, ID>(id);
        AssetTypes& assetVariant = m_assets.at(_id);

        auto sharedPtr = std::get_if<std::shared_ptr<Type>>(&assetVariant);
        if (not sharedPtr) {
            throw std::runtime_error("Asset type mismatch!");
        }

        return *sharedPtr;
    }

    template <typename Type, typename ID>
    const std::shared_ptr<Type>& get(ID id) const {
        if (!exists<Type>(id)) {
            if constexpr (std::is_enum_v<ID>) {
                throw std::runtime_error("Asset " + assets::to_string(id) + " not found!");
            } else if constexpr (std::is_same_v<ID, std::string>) {
                throw std::runtime_error("Asset " + id + " not found!");
            } else if constexpr (std::is_same_v<ID, const char*>) {
                throw std::runtime_error("Asset " + id + " not found!");
            }
        }
        std::string _id = generateID<Type, ID>(id);
        return std::static_pointer_cast<Type>(std::get<std::shared_ptr<Type>>(m_assets.at(_id)));
    }

    template <typename Type, typename ID, typename... Args>
    const std::shared_ptr<Type>& reload(ID id, Args&&... args) {
        std::shared_ptr<Type> asset = std::make_shared<Type>(std::forward<Args>(args)...);
        std::string _id = generateID<Type, ID>(id);

        auto it = m_assets.find(_id);
        if (it != m_assets.end() && std::holds_alternative<std::shared_ptr<Type>>(it->second)) {
            std::get<std::shared_ptr<Type>>(it->second) = asset;
        } else {
            throw std::runtime_error("Asset " + _id + " not found or has an incompatible type. Reload failed!");
        }
#ifdef DEBUG
        CORE_INFO("Reloaded asset {0}", _id);
#endif
        return std::static_pointer_cast<Type>(std::get<std::shared_ptr<Type>>(m_assets.at(_id)));
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
    using AssetTypes = std::variant<std::shared_ptr<Texture>, std::shared_ptr<Shader>, std::shared_ptr<Prefab>, std::shared_ptr<Model>>;
    std::unordered_map<std::string, AssetTypes> m_assets;

    template <typename Type, typename ID>
    std::string generateID(ID id) const {
        std::string type = typeid(Type).name();
        std::string realType = type.substr(type.find_last_of(':') + 1);
        if constexpr (std::is_enum_v<ID>) {
            return realType + "_" + assets::to_string(id);
        } else if constexpr (std::is_same_v<ID, std::string>) {
            return realType + "_" + id;
        } else if constexpr (std::is_same_v<ID, const char*>) {
            return realType + "_" + id;
        } else {
            throw std::runtime_error("Invalid ID type " + realType);
        }
    }
};
}