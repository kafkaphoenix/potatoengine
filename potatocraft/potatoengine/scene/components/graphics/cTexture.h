#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "potatoengine/assets/texture.h"
#include "potatoengine/pch.h"

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
    std::vector<std::shared_ptr<Texture>> textures;
    glm::vec4 color{};
    float blendFactor{};
    float reflectivity{};
    float refractiveIndex{};
    bool hasTransparency{};
    bool useLighting{};
    bool useReflection{};
    bool useRefraction{};
    std::string _drawMode;
    DrawMode drawMode{};

    CTexture() = default;
    explicit CTexture(std::vector<std::string>&& fps, glm::vec4&& c, float bf, float r, float ri, bool ht, bool ul, bool ur, bool uf, std::string&& dm) : filepaths(std::move(fps)), color(std::move(c)), blendFactor(bf), reflectivity(r), refractiveIndex(ri), hasTransparency(ht), useLighting(ul), useReflection(ur), useRefraction(uf), _drawMode(dm) {}

    void print() const {
        std::string texturePaths;
        if (filepaths.size() == 0) {
            texturePaths = "\n\t\t\t\t none";
        } else {
            for (const std::string& filepath : filepaths) {
                texturePaths += "\n\t\t\t\t " + filepath;
            }
        }
        CORE_TRACE("\t\tcolor: {0}\n\t\t\t\tblendFactor: {1}\n\t\t\t\treflectivity: {2}\n\t\t\t\trefractiveIndex: {3}\n\t\t\t\thasTransparency: {4}\n\t\t\t\tuseLighting: {5}\n\t\t\t\tuseReflection: {6}\n\t\t\t\tuseRefraction: {7}\n\t\t\t\tdrawMode: {8}\n\t\t\t\ttextures: {9}", glm::to_string(color), blendFactor, reflectivity, refractiveIndex, hasTransparency, useLighting, useReflection, useRefraction, _drawMode, texturePaths);
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
        } else if (_drawMode == "textureAtlasBlend") { // blend two textures in the atlas // TODO: this is not implemented
            drawMode = DrawMode::TEXTURE_ATLAS_BLEND;
        } else if (_drawMode == "textureBlendColor") { // blend texture with a color
            drawMode = DrawMode::TEXTURE_BLEND_COLOR;
        } else if (_drawMode == "textureAtlasBlendColor") { // blend texture in the atlas with a color
            drawMode = DrawMode::TEXTURE_ATLAS_BLEND_COLOR;
        } else {
            throw std::runtime_error("Unknown draw mode " + _drawMode);
        }
    }
};
}
