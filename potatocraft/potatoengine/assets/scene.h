#pragma once

#include <nlohmann/json.hpp>

#include "potatoengine/pch.h"
using json = nlohmann::json;

namespace potatoengine {
class Scene {
   public:
    Scene(std::filesystem::path&& fp);

    std::string_view getFilepath() const noexcept { return m_filepath; }
    const std::unordered_map<std::string, json>& getShaders() const noexcept { return m_shaders; }
    const std::unordered_map<std::string, json>& getTextures() const noexcept { return m_textures; }
    const std::unordered_map<std::string, json>& getModels() const noexcept { return m_models; }
    const std::unordered_map<std::string, json>& getPrefabs() const noexcept { return m_prefabs; }
    const std::unordered_map<std::string, json>& getScenes() const noexcept { return m_scenes; }
    const std::unordered_map<std::string, json>& getEntities() const noexcept { return m_entities; }
    std::unordered_map<std::string, json>& getLights() noexcept { return m_lights; }
    std::unordered_map<std::string, json>& getCameras() noexcept { return m_cameras; }
    std::unordered_map<std::string, json>& getSystems() noexcept { return m_systems; }

    bool operator==(const Scene& other) const noexcept {
        return m_filepath == other.getFilepath();
    }

   private:
    std::string m_filepath{};
    std::unordered_map<std::string, json> m_shaders{};
    std::unordered_map<std::string, json> m_textures{};
    std::unordered_map<std::string, json> m_models{};
    std::unordered_map<std::string, json> m_prefabs{};
    std::unordered_map<std::string, json> m_scenes{};
    std::unordered_map<std::string, json> m_entities{};
    std::unordered_map<std::string, json> m_lights{};
    std::unordered_map<std::string, json> m_cameras{};
    std::unordered_map<std::string, json> m_systems{};

    void read(const json& j);
};
}