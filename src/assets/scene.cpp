#include "assets/scene.h"

#include "utils/timer.h"

namespace potatoengine {
Scene::Scene(std::filesystem::path&& fp) : m_filepath(std::move(fp.string())) {
  std::ifstream f(fp);
  ENGINE_ASSERT(f.is_open(), "Failed to open scene file!");
  ENGINE_ASSERT(f.peek() != std::ifstream::traits_type::eof(), "Scene file is empty!");
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
    if (data.at("entities").contains("normals")) {
      for (const auto& [key, value] : data.at("entities").at("normals").items()) {
        m_normalEntities[key] = value;
      }
    }
    if (data.at("entities").contains("lights")) {
      for (const auto& [key, value] : data.at("entities").at("lights").items()) {
        m_lightEntities[key] = value;
      }
    }
    if (data.at("entities").contains("cameras")) {
      for (const auto& [key, value] : data.at("entities").at("cameras").items()) {
        m_cameraEntities[key] = value;
      }
    }
    if (data.at("entities").contains("systems")) {
      for (const auto& [key, value] : data.at("entities").at("systems").items()) {
        m_systemEntities[key] = value;
      }
    }
    if (data.at("entities").contains("fbos")) {
      for (const auto& [key, value] : data.at("entities").at("fbos").items()) {
        m_fboEntities[key] = value;
      }
    }
  }
}

const std::map<std::string, std::string, NumericComparator>& Scene::getInfo() {
  if (not m_info.empty()) {
    return m_info;
  }

  m_info["Type"] = "Scene";
  m_info["Filepath"] = m_filepath;
  m_info["Shaders"] = std::to_string(m_shaders.size());
  m_info["Textures"] = std::to_string(m_textures.size());
  m_info["Models"] = std::to_string(m_models.size());
  m_info["Prefabs"] = std::to_string(m_prefabs.size());
  m_info["Scenes"] = std::to_string(m_scenes.size());
  m_info["Normal entities"] = std::to_string(m_normalEntities.size());
  m_info["Light entities"] = std::to_string(m_lightEntities.size());
  m_info["Camera entities"] = std::to_string(m_cameraEntities.size());
  m_info["System entities"] = std::to_string(m_systemEntities.size());
  m_info["FBO entities"] = std::to_string(m_fboEntities.size());

  return m_info;
}
}