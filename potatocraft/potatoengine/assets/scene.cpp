#include "potatoengine/assets/scene.h"

#include "potatoengine/utils/timer.h"

namespace potatoengine {
Scene::Scene(std::filesystem::path&& fp) : m_filepath(std::move(fp.string())) {
    std::ifstream f(fp);
    if (not f.is_open()) [[unlikely]] {
        f.close();
        throw std::runtime_error("Failed to load scene: " + m_filepath);
    }
    json data = json::parse(f);
    f.close();

    read(data);
}

void Scene::read(const json& data) {
    if (data.contains("assets")) {
        if (data.at("assets").contains("shaders")) {
            for (const auto& [key, value] : data.at("assets").at("shaders").items()) {
                m_shaders[key] = value;
            }
        }
        if (data.at("assets").contains("textures")) {
            for (const auto& [key, value] : data.at("assets").at("textures").items()) {
                m_textures[key] = value;
            }
        }
        if (data.at("assets").contains("models")) {
            for (const auto& [key, value] : data.at("assets").at("models").items()) {
                m_models[key] = value;
            }
        }
        if (data.at("assets").contains("prefabs")) {
            for (const auto& [key, value] : data.at("assets").at("prefabs").items()) {
                m_prefabs[key] = value;
            }
        }
        if (data.at("assets").contains("scenes")) {
            for (const auto& [key, value] : data.at("assets").at("scenes").items()) {
                m_scenes[key] = value;
            }
        }
    }
    if (data.contains("entities")) {
        for (const auto& [key, value] : data.at("entities").items()) {
            m_entities[key] = value;
        }
    }
    if (data.contains("lights")) {
        for (const auto& [key, value] : data.at("lights").items()) {
            m_lights[key] = value;
        }
    }
    if (data.contains("cameras")) {
        for (const auto& [key, value] : data.at("cameras").items()) {
            m_cameras[key] = value;
        }
    }
    if (data.contains("systems")) {
        for (const auto& [key, value] : data.at("systems").items()) {
            m_systems[key] = value;
        }
    }
}

}