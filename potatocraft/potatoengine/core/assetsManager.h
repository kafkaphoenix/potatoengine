#pragma once

#include "potatoengine/pch.h"
#include "potatoengine/renderer/shader/shader.h"
#include "potatoengine/renderer/texture.h"
#include "potatoengine/scene/prefab.h"

namespace potatoengine::asset {

constexpr inline auto enum_range = [](auto front, auto back) {
    return std::views::iota(std::to_underlying(front), std::to_underlying(back) + 1) | std::views::transform([](auto e) { return decltype(front)(e); });
};

namespace texture {
enum class ID {
    Cube,
};
}
namespace shader {
enum class ID {
    VBasic,
    FBasic,
};
}
namespace model {
enum class ID {
    Cube,
};
}
namespace prefab {
enum class ID {
    Player,
    Chicken,
    BrickBlock,
};
}
namespace sound {
enum class ID {
    Default,
};
}
namespace font {
enum class ID {
    Default,
};
}

inline std::string_view to_string(const asset::texture::ID id) {
    switch (id) {
        using enum asset::texture::ID;
        case Cube:
            return "Cube";
    }
}

inline std::string_view to_string(const asset::shader::ID id) {
    switch (id) {
        using enum asset::shader::ID;
        case VBasic:
            return "VBasic";
        case FBasic:
            return "FBasic";
    }
}

inline std::string_view to_string(const asset::model::ID id) {
    switch (id) {
        using enum asset::model::ID;
        case Cube:
            return "Cube";
    }
}

inline std::string_view to_string(const asset::prefab::ID id) {
    switch (id) {
        using enum asset::prefab::ID;
        case Player:
            return "Player";
        case Chicken:
            return "Chicken";
        case BrickBlock:
            return "BrickBlock";
    }
}

inline std::string_view to_string(const asset::sound::ID id) {
    switch (id) {
        using enum asset::sound::ID;
        case Default:
            return "Default";
    }
}

inline std::string_view to_string(const asset::font::ID id) {
    switch (id) {
        using enum asset::font::ID;
        case Default:
            return "Default";
    }
}
}

namespace potatoengine {
class AssetsManager {
   public:
    template <typename Type, typename ID, typename... Args>
    void load(ID id, Args&&... args) {
        std::unique_ptr<Type> asset = std::make_unique<Type>(std::forward<Args>(args)...);
        std::string _id = generateID<Type, ID>(id);
        m_assets.emplace(_id, std::move(asset));
#ifdef DEBUG
        CORE_INFO("\tLoaded asset {0}", _id);
#endif
    }

    template <typename Type, typename ID>
    std::reference_wrapper<Type> get(ID id) {
        std::string _id = generateID<Type, ID>(id);
        return std::ref(*std::get<std::unique_ptr<Type>>(m_assets.at(_id)));
    }

    template <typename Type, typename ID>
    std::reference_wrapper<const Type> get(ID id) const {
        std::string _id = generateID<Type, ID>(id);
        return std::cref(*std::get<std::unique_ptr<Type>>(m_assets.at(_id)));
    }

    template <typename Type, typename ID, typename... Args>
    std::reference_wrapper<const Type> reload(ID id, Args&&... args) {
        std::unique_ptr<Type> asset = std::make_unique<Type>(std::forward<Args>(args)...);
        std::string _id = generateID<Type, ID>(id);

        auto it = std::ranges::find_if(m_assets, [&](const auto& pair) {
            return pair.first == _id and std::holds_alternative<std::unique_ptr<Type>>(pair.second);
        });

        if (it not_eq m_assets.end()) {
            auto& oldAsset = std::get<std::unique_ptr<Type>>(it->second);
            oldAsset = std::move(asset);
        } else [[unlikely]] {
            throw std::runtime_error("Asset " + _id + " not found or has an incompatible type. Reload failed!");
        }
#ifdef DEBUG
        CORE_INFO("Reloaded asset {0}", _id);
#endif
        return std::cref(*std::get<std::unique_ptr<Type>>(m_assets.at(_id)));
    }

    void print() const {
        for (const auto& [id, asset] : m_assets) {
            std::string_view fp = std::visit([](const auto& asset) { return asset->getFilepath(); }, asset);
            CORE_INFO("Asset {0}: {1}", id, fp);
        }
    }

    static std::shared_ptr<AssetsManager> Create() {
        return std::make_shared<AssetsManager>();
    }

   private:
    template <typename Type, typename ID>
    std::string generateID(ID id) const {
        std::string_view type = typeid(Type).name();
        std::string_view realType = type.substr(type.find_last_of(':') + 1);
        std::string _id = std::string(realType) + "_" + std::string(asset::to_string(id));
        return _id;
    }
    using AssetTypes = std::variant<std::unique_ptr<Texture>, std::unique_ptr<Shader>, std::unique_ptr<Prefab>>;
    std::unordered_map<std::string, AssetTypes> m_assets;
};
}