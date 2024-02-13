#pragma once

#include <nlohmann/json.hpp>

#include "assets/asset.h"
#include "pch.h"
#include "utils/numericComparator.h"

using json = nlohmann::json;

namespace potatoengine::assets {
class Scene : public Asset {
  public:
    Scene(std::filesystem::path&& fp);

    virtual const std::map<std::string, std::string, NumericComparator>&
    getInfo() override final;

    const std::unordered_map<std::string, json>& getShaders() const {
      return m_shaders;
    }
    const std::unordered_map<std::string, json>& getTextures() const {
      return m_textures;
    }
    const std::unordered_map<std::string, json>& getModels() const {
      return m_models;
    }
    const std::unordered_map<std::string, json>& getPrefabs() const {
      return m_prefabs;
    }
    const std::unordered_map<std::string, json>& getScenes() const {
      return m_scenes;
    }

    const std::unordered_map<std::string, json>& getNormalEntities() const {
      return m_normalEntities;
    }
    const std::unordered_map<std::string, json>& getLightEntities() const {
      return m_lightEntities;
    }
    const std::unordered_map<std::string, json>& getCameraEntities() const {
      return m_cameraEntities;
    }
    const std::unordered_map<std::string, json>& getSystemEntities() const {
      return m_systemEntities;
    }
    const std::unordered_map<std::string, json>& getFBOEntities() const {
      return m_fboEntities;
    }

    virtual bool operator==(const Asset& other) const override final;

  private:
    std::string m_filepath;

    std::unordered_map<std::string, json> m_shaders;
    std::unordered_map<std::string, json> m_textures;
    std::unordered_map<std::string, json> m_models;
    std::unordered_map<std::string, json> m_prefabs;
    std::unordered_map<std::string, json> m_scenes;

    std::unordered_map<std::string, json> m_normalEntities;
    std::unordered_map<std::string, json> m_lightEntities;
    std::unordered_map<std::string, json> m_cameraEntities;
    std::unordered_map<std::string, json> m_systemEntities;
    std::unordered_map<std::string, json> m_fboEntities;

    std::map<std::string, std::string, NumericComparator> m_info;

    void read(const json& j);
};
}