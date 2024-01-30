#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "assets/texture.h"
#include "core/application.h"
#include "pch.h"
#include "utils/mapJsonSerializer.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CTexture {
    enum class DrawMode {
      COLOR,
      TEXTURE,
      TEXTURE_ATLAS,
      TEXTURES_BLEND,
      TEXTURE_ATLAS_BLEND,
      TEXTURE_BLEND_COLOR,
      TEXTURE_ATLAS_BLEND_COLOR
    };

    std::vector<std::string> filepaths;
    std::vector<std::shared_ptr<assets::Texture>> textures;
    glm::vec4 color{};
    float blendFactor{};
    float reflectivity{};
    float refractiveIndex{};
    bool hasTransparency{};
    bool useLighting{};
    bool useReflection{};
    bool useRefraction{};
    std::string _drawMode;
    DrawMode drawMode;

    CTexture() = default;
    explicit CTexture(std::vector<std::string>&& fps, glm::vec4&& c, float bf,
                      float r, float ri, bool ht, bool ul, bool ur, bool uf,
                      std::string&& dm)
      : filepaths(std::move(fps)), color(std::move(c)), blendFactor(bf),
        reflectivity(r), refractiveIndex(ri), hasTransparency(ht),
        useLighting(ul), useReflection(ur), useRefraction(uf), _drawMode(dm) {}

    void print() const {
      std::string texturePaths;
      if (filepaths.size() == 0) {
        texturePaths = "\n\t\t\t\t\t\t none";
      } else {
        for (std::string_view filepath : filepaths) {
          texturePaths += std::format("\n\t\t\t\t\t\t {}", filepath);
        }
      }
      ENGINE_BACKTRACE(
        "\t\tcolor: {0}\n\t\t\t\t\t\tblendFactor: "
        "{1}\n\t\t\t\t\t\treflectivity: "
        "{2}\n\t\t\t\t\t\trefractiveIndex: "
        "{3}\n\t\t\t\t\t\thasTransparency: {4}\n\t\t\t\t\t\tuseLighting: "
        "{5}\n\t\t\t\t\t\tuseReflection: "
        "{6}\n\t\t\t\t\t\tuseRefraction: {7}\n\t\t\t\t\t\tdrawMode: "
        "{8}\n\t\t\t\t\t\ttextures: {9}",
        glm::to_string(color), blendFactor, reflectivity, refractiveIndex,
        hasTransparency, useLighting, useReflection, useRefraction, _drawMode,
        texturePaths);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["color"] = glm::to_string(color);
      info["blendFactor"] = std::to_string(blendFactor);
      info["reflectivity"] = std::to_string(reflectivity);
      info["refractiveIndex"] = std::to_string(refractiveIndex);
      info["hasTransparency"] = hasTransparency ? "true" : "false";
      info["useLighting"] = useLighting ? "true" : "false";
      info["useReflection"] = useReflection ? "true" : "false";
      info["useRefraction"] = useRefraction ? "true" : "false";
      info["drawMode"] = _drawMode;

      for (uint32_t i = 0; i < textures.size(); ++i) {
        info["texture " + std::to_string(i)] = getTextureInfo(i);
      }

      return info;
    }

    std::string getTextureInfo(uint32_t index) const {
      return MapToJson(textures.at(index)->getInfo());
    }

    void setDrawMode() { // TODO maybe send assets manager to this function?
      if (_drawMode == "color") {
        drawMode = DrawMode::COLOR;
      } else if (_drawMode == "texture") {
        drawMode = DrawMode::TEXTURE;
      } else if (_drawMode == "textureAtlas") {
        drawMode = DrawMode::TEXTURE_ATLAS;
      } else if (_drawMode == "texturesBlend") { // blend two textures
        drawMode = DrawMode::TEXTURES_BLEND;
      } else if (_drawMode ==
                 "textureAtlasBlend") { // blend two textures in the atlas //
                                        // TODO: this is not implemented
        drawMode = DrawMode::TEXTURE_ATLAS_BLEND;
      } else if (_drawMode ==
                 "textureBlendColor") { // blend texture with a color
        drawMode = DrawMode::TEXTURE_BLEND_COLOR;
      } else if (_drawMode ==
                 "textureAtlasBlendColor") { // blend texture in the atlas with
                                             // a color
        drawMode = DrawMode::TEXTURE_ATLAS_BLEND_COLOR;
      } else {
        ENGINE_ASSERT(false, "Unknown draw mode {}", _drawMode);
      }
    }

    void setTextures() {
      if (filepaths.size() == 0) {
        return;
      }
      const auto& assets_manager = Application::Get().getAssetsManager();

      textures.reserve(filepaths.size());
      for (std::string_view filepath : filepaths) {
        textures.emplace_back(assets_manager->get<assets::Texture>(filepath));
      }
    }

    void reloadTextures(std::vector<std::string>&& fps) {
      ENGINE_ASSERT(fps != filepaths, "Texture filepaths are the same");
      filepaths = std::move(fps);
      textures.clear();
      setTextures();
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e,
                                                   CTexture& c) {
  c.setDrawMode();
  c.setTextures();

  m_registry.replace<CTexture>(e, c);
}