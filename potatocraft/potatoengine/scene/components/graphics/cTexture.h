#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "potatoengine/assets/texture.h"
#include "potatoengine/pch.h"

namespace potatoengine {

struct CTexture {
    std::vector<std::string> filepaths;
    std::vector<std::shared_ptr<Texture>> textures;
    bool hasTransparency{};
    bool useFakeLighting{};
    bool useBlending{};
    float blendFactor{};
    bool useColor{};
    glm::vec4 color{};
    bool useReflection{};
    float reflectivity{};
    bool useRefraction{};
    float refractiveIndex{};

    CTexture() = default;
    explicit CTexture(std::vector<std::shared_ptr<Texture>>&& t, bool ht, bool ufl, bool ub, float bf, bool uc, glm::vec4&& c, bool ur, float r, bool urf, float ri)
        : textures(std::move(t)), hasTransparency(ht), useFakeLighting(ufl), useBlending(ub), blendFactor(bf), useColor(uc), color(std::move(c)), useReflection(ur), reflectivity(r), useRefraction(urf), refractiveIndex(ri) {}

    void print() const {
        std::string texturePaths;
        for (const std::string& filepath : filepaths) {
            texturePaths += "\n\t\t\t\ttexture: " + filepath;
        }
        CORE_INFO("\t\thasTransparency: {0}\n\t\t\t\tuseFakeLighting: {1}\n\t\t\t\tuseBlending: {2}\n\t\t\t\tblendFactor: {3}\n\t\t\t\tuseColor: {4}\n\t\t\t\tcolor: {5}\n\t\t\t\tuseReflection: {6}\n\t\t\t\treflectivity: {7}\n\t\t\t\tuseRefraction: {8}\n\t\t\t\trefractiveIndex: {9}{10}", hasTransparency, useFakeLighting, useBlending, blendFactor, useColor, glm::to_string(color), useReflection, reflectivity, useRefraction, refractiveIndex, texturePaths);
    }
};
}
