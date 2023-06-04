#pragma once

#include "potatoengine/pch.h"
#include "potatoengine/renderer/shader/shader.h"
#include "potatoengine/renderer/texture.h"

namespace potatoengine {

namespace asset {
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
    Entities,
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
}

class AssetsManager {
   public:
    template <typename Type, typename ID>
    void load(ID id, const std::string& filepath) {
        std::unique_ptr<Type> asset = std::make_unique<Type>(filepath);
        std::string _id = generateUUID<Type, ID>(id);
        m_assets.emplace(_id, std::move(asset));
        CORE_INFO("Loaded asset {0}", _id);
    }

    template <typename Type, typename ID>
    Type& get(ID id) {
        std::string _id = generateUUID<Type, ID>(id);
        return *std::get<std::unique_ptr<Type>>(m_assets.at(_id));
    }

    template <typename Type, typename ID>
    const Type& get(ID id) const {
        std::string _id = generateUUID<Type, ID>(id);
        return *std::get<std::unique_ptr<Type>>(m_assets.at(_id));
    }

    template <typename Type, typename ID>
    void reload(ID id, const std::string& filepath) {
        std::unique_ptr<Type> asset = std::make_unique<Type>(filepath);
        std::string _id = generateUUID<Type, ID>(id);
        if (auto it = m_assets.find(_id); it != m_assets.end()) {
            auto& oldAsset = it->second;
            if (auto* assetPtr = std::get_if<std::unique_ptr<Type>>(&oldAsset)) {
                *assetPtr = std::move(asset);
            } else {
                throw std::runtime_error("Asset {0} has an incompatible type", _id);
            }
        } else {
            throw std::runtime_error("Asset {0} not found", _id);
        }
        CORE_INFO("Reloaded asset {0}", _id);
    }

    void print() const {
        for (const auto& [id, asset] : m_assets) {
            const std::string& filepath = std::visit([](const auto& asset) { return asset->getFilepath(); }, asset);
            CORE_INFO("Asset {0}: {1}", id, filepath);
        }
    }

    static std::shared_ptr<AssetsManager> Create() {
        return std::make_shared<AssetsManager>();
    }

   private:
    template <typename Type, typename ID>
    std::string generateUUID(ID id) const {
        std::string type = typeid(Type).name();
        std::string realType = type.substr(type.find_last_of(':') + 1);
        std::string _id = realType + "_ID_" + std::to_string(static_cast<int>(id));
        return _id;
    };
    using AssetTypes = std::variant<std::unique_ptr<Texture>, std::unique_ptr<Shader>>;
    std::unordered_map<std::string, AssetTypes> m_assets;
};
}