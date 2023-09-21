#pragma once

#include <glm/glm.hpp>

#include <entt/entt.hpp>
#include "potatoengine/pch.h"
#include "potatoengine/renderer/buffer.h"
#include "potatoengine/assets/texture.h"
#include "potatoengine/renderer/vao.h"
#include "potatoengine/scene/components/graphics/cMaterial.h"
#include "potatoengine/renderer/shaderProgram.h"
#include "potatoengine/scene/components/world/cSkybox.h"
#include "potatoengine/scene/components/graphics/cTexture.h"
#include "potatoengine/scene/components/graphics/cTextureAtlas.h"

namespace potatoengine {

struct CMesh {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};
    std::vector<std::shared_ptr<Texture>> textures;
    std::shared_ptr<VAO> vao;

    CMesh() = default;
    explicit CMesh(std::vector<Vertex>&& v, std::vector<uint32_t>&& i, std::vector<std::shared_ptr<engine::Texture>>&& t)
        : vertices(std::move(v)), indices(std::move(i)), textures(std::move(t)) {}

    void setupMesh() {
        vao = VAO::Create();
        vao->attachVertex(VBO::Create(vertices));
        vao->setIndex(IBO::Create(indices));
    }

    const std::shared_ptr<VAO>& getVAO() {
        if (not vao) {
            setupMesh();
        }
        return vao;
    }

    void bindTextures(std::unique_ptr<ShaderProgram>& sp, CTexture* cTexture, CTextureAtlas* cTextureAtlas, CTexture* cSkyboxTexture, const glm::vec3& cameraPosition, CMaterial* cMaterial) {
        using namespace entt::literals;
        sp->resetActiveUniforms();
        sp->use();
        uint32_t i = 1;
        sp->setFloat("useFog", static_cast<float>(entt::monostate<"useFog"_hs>{}));
        sp->setFloat("fogDensity", static_cast<float>(entt::monostate<"fogDensity"_hs>{}));
        sp->setFloat("fogGradient", static_cast<float>(entt::monostate<"fogGradient"_hs>{}));
        sp->setVec3("fogColor", static_cast<glm::vec3>(entt::monostate<"fogColor"_hs>{}));
        if (cTexture) {
            for (auto& texture : cTexture->textures) {
                if (cTexture->useFakeLighting) {
                    sp->setFloat("useFakeLighting", 1.f);
                    sp->setVec3("lightPosition",  static_cast<glm::vec3>(entt::monostate<"lightPosition"_hs>{}));
                    sp->setVec3("lightColor",  static_cast<glm::vec3>(entt::monostate<"lightColor"_hs>{}));
                }
                if (cTextureAtlas) {
                    sp->setFloat("useAtlas", 1.f);
                    int index = cTextureAtlas->index;
                    int rows = cTextureAtlas->rows;
                    sp->setFloat("numRows", rows);
                    int col = index % rows;
                    float coll = static_cast<float>(col) / static_cast<float>(rows);
                    int row = index / rows;
                    float roww = static_cast<float>(row) / static_cast<float>(rows);
                    sp->setVec2("offset", glm::vec2(coll, roww));
                }
                if (cTexture->useBlending) {
                    sp->setFloat("useBlending", 1.f);
                    sp->setFloat("blendFactor", cTexture->blendFactor);
                }
                if (cTexture->useColor) {
                    sp->setFloat("useColor", 1.f);
                    sp->setVec4("color", cTexture->color);
                }
                if (static_cast<float>(entt::monostate<"useSkyBlending"_hs>{}) == 1.f) {
                    sp->setFloat("useSkyBlending", static_cast<float>(entt::monostate<"useSkyBlending"_hs>{}));
                    sp->setFloat("skyBlendFactor", static_cast<float>(entt::monostate<"skyBlendFactor"_hs>{}));
                    int ti = 10;
                    for (auto& t : cSkyboxTexture->textures) {
                        sp->setInt(t->getType().data() + std::string("Sky") + std::to_string(ti), ti);
                        t->bindSlot(ti);
                        ti++;
                    }
                    if (cTexture->useReflection) {
                        sp->setFloat("useReflection", 1.f);
                        sp->setFloat("reflectivity", cTexture->reflectivity);
                        sp->setVec3("cameraPosition", cameraPosition);
                    }
                    if (cTexture->useRefraction) {
                        sp->setFloat("useRefraction", 1.f);
                        sp->setFloat("refractiveIndex", cTexture->refractiveIndex);
                    }
                }
                if (cMaterial) {
                    sp->setVec3("ambient", cMaterial->ambient);
                    sp->setVec3("diffuse", cMaterial->diffuse);
                    sp->setVec3("specular", cMaterial->specular);
                    sp->setFloat("shininess", cMaterial->shininess);
                }
                sp->setInt(texture->getType().data() + std::to_string(i), i);
                texture->bindSlot(i);
                ++i;
            }
        } else {
            uint32_t diffuseN = 1;
            uint32_t specularN = 1;
            uint32_t normalN = 1;
            uint32_t heightN = 1;
            for (auto& texture : textures) {
                std::string number;
                std::string_view type = texture->getType();
                if (type == "textureDiffuse") {
                    number = std::to_string(diffuseN++);
                } else if (type == "textureSpecular") {
                    number = std::to_string(specularN++);
                } else if (type == "textureNormal") {
                    number = std::to_string(normalN++);
                } else if (type == "textureHeight") {
                    number = std::to_string(heightN++);
                } else {
                    throw std::runtime_error("Unknown texture type " + std::string(type));
                }
                sp->setInt(type.data() + number, i);
                texture->bindSlot(i);
                ++i;
            }
            if (diffuseN == 1) {
                sp->setFloat("useNormal", 1.f);
            }
        }
        sp->unuse();
    }

    void unbindTextures(CTexture* cTexture) {
        auto& texturesToUnbind = (cTexture) ? cTexture->textures : textures;
        for (auto& texture : texturesToUnbind) {
            texture->unbindSlot();
        }
    }

    void print() const {
        std::string texturePaths;
        for (const auto& texture : textures) {
            texturePaths += "\n\t\tttexture: " + std::string(texture->getFilepath());
        }
        CORE_INFO("\t\tvertices: {0}\n\t\tindices: {1}{2}", vertices.size(), indices.size(), texturePaths);
    }
};
}