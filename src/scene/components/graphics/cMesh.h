#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "assets/texture.h"
#include "pch.h"
#include "renderer/buffer.h"
#include "renderer/shaderProgram.h"
#include "renderer/vao.h"
#include "scene/components/graphics/cMaterial.h"
#include "scene/components/graphics/cTexture.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/world/cSkybox.h"

namespace potatoengine {

struct CMesh {
    std::vector<std::shared_ptr<Texture>> textures;
    std::shared_ptr<VAO> vao;
    std::vector<Vertex> vertices; // TODO: remove this
    std::shared_ptr<VBO> vbo;
    std::vector<uint32_t> indices;
    std::string vertexType;

    CMesh() = default;
    explicit CMesh(std::vector<Vertex>&& v, std::vector<uint32_t>&& i,
                   std::vector<std::shared_ptr<engine::Texture>>&& t, std::string&& vt = "basic")
        : vertices(std::move(v)), indices(std::move(i)), textures(std::move(t)), vertexType(std::move(vt)) {}

    void setupMesh() {
      vao = VAO::Create();
      if (vertexType == "basic") {
        vao->attachVertex(VBO::Create(vertices), VAO::VertexType::VERTEX);
      } else if (vertexType == "shape") {
        vao->attachVertex(VBO::Create(vertices), VAO::VertexType::SHAPE_VERTEX);
      } else if (vertexType == "terrain") { // TODO maybe a better way to do this using vertices?
        vao->attachVertex(std::move(vbo), VAO::VertexType::TERRAIN_VERTEX);
      } else {
        ENGINE_ASSERT(false, "Unknown vertex type {}", vertexType);
      }
      vao->setIndex(IBO::Create(indices));
    }

    void updateMesh() {
      if (vertexType == "basic") {
        vao->updateVertex(VBO::Create(vertices), 0, VAO::VertexType::VERTEX);
      } else if (vertexType == "shape") {
        vao->updateVertex(VBO::Create(vertices), 0, VAO::VertexType::SHAPE_VERTEX);
      } else if (vertexType == "terrain") {
        vao->updateVertex(VBO::Create(vertices), 0, VAO::VertexType::TERRAIN_VERTEX);
      } else {
        ENGINE_ASSERT(false, "Unknown vertex type {}", vertexType);
      }
    }

    const std::shared_ptr<VAO>& getVAO() {
      if (not vao) {
        setupMesh();
      }
      return vao;
    }

    void bindTextures(std::unique_ptr<ShaderProgram>& sp, CTexture* cTexture, CTextureAtlas* cTextureAtlas,
                      CTexture* cSkyboxTexture, CMaterial* cMaterial) {
      using namespace entt::literals; // TODO rethink this method

      sp->resetActiveUniforms();
      sp->use();
      sp->setFloat("useFog", static_cast<float>(entt::monostate<"useFog"_hs>{}));
      sp->setFloat("fogDensity", static_cast<float>(entt::monostate<"fogDensity"_hs>{}));
      sp->setFloat("fogGradient", static_cast<float>(entt::monostate<"fogGradient"_hs>{}));
      sp->setVec3("fogColor", static_cast<glm::vec3>(entt::monostate<"fogColor"_hs>{}));

      if (cTexture) {
        uint32_t i = 1;
        for (auto& texture : cTexture->textures) {
          sp->setInt(texture->getType().data() + std::to_string(i), i);
          texture->bindSlot(i);
          ++i;
        }
        if (cTexture->drawMode == CTexture::DrawMode::COLOR or
            cTexture->drawMode == CTexture::DrawMode::TEXTURE_BLEND_COLOR or
            cTexture->drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND_COLOR) {
          sp->setFloat("useColor", 1.f);
          sp->setVec4("color", cTexture->color);
        }
        if (cTexture->useLighting) {
          sp->setFloat("useLighting", 1.f);
          sp->setVec3("lightPosition", static_cast<glm::vec3>(entt::monostate<"lightPosition"_hs>{}));
          sp->setVec3("lightColor", static_cast<glm::vec3>(entt::monostate<"lightColor"_hs>{}));
        }
        if (cTexture->drawMode == CTexture::DrawMode::TEXTURE_ATLAS or
            cTexture->drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND or
            cTexture->drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND_COLOR) {
          if (sp->getName() == "basic") { // terrain shader get texture atlas data from vertex
            sp->setFloat("useTextureAtlas", 1.f);
            int index = cTextureAtlas->index;
            int rows = cTextureAtlas->rows;
            sp->setFloat("numRows", rows);
            int col = index % rows;
            float coll = static_cast<float>(col) / rows;
            int row = index / rows;
            float roww = static_cast<float>(row) / rows;
            sp->setVec2("offset", glm::vec2(coll, roww));
          }
        }
        if (cTexture->drawMode == CTexture::DrawMode::TEXTURES_BLEND or
            cTexture->drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND or
            cTexture->drawMode == CTexture::DrawMode::TEXTURE_BLEND_COLOR or
            cTexture->drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND_COLOR) {
          sp->setFloat("useBlending", 1.f);
          sp->setFloat("blendFactor", cTexture->blendFactor);
        }
        if (static_cast<float>(entt::monostate<"useSkyBlending"_hs>{}) == 1.f and sp->getName() == "basic") {
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
      } else {
        uint32_t diffuseN = 1;
        uint32_t specularN = 1;
        uint32_t normalN = 1;
        uint32_t heightN = 1;
        uint32_t i = 1;
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
            ENGINE_ASSERT(false, "Unknown texture type {}", type);
          }
          sp->setInt(type.data() + number, i);
          texture->bindSlot(i);
          ++i;
        }
        if (textures.size() == 0) {
          ENGINE_ASSERT(cMaterial, "No material found for entity");
          sp->setFloat("noTexture", 1.f);
          sp->setVec3("materialColor", cMaterial->diffuse);
        } else {
          if (diffuseN == 1) {
            sp->setFloat("useNormal", 1.f);
          }
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
        texturePaths += std::format("\n\t\t\ttexture: {}", texture->getFilepath());
      }
      ENGINE_TRACE("\t\tvertices: {0}\n\t\tindices: {1}{2}", vertices.size(), indices.size(), texturePaths);
    }
};
}