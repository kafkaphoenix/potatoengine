#pragma once

#include <assimp/scene.h>

#include "assets/texture.h"
#include "pch.h"
#include "scene/components/graphics/cMaterial.h"
#include "scene/components/graphics/cMesh.h"
#include "utils/numericComparator.h"

namespace potatoengine {

class Model {
  public:
    Model(std::filesystem::path&& fp, std::optional<bool> gammaCorrection = std::nullopt);
    Model& operator=(const Model&) = delete;

    const std::map<std::string, std::string, NumericComparator>& getInfo();
    const std::map<std::string, std::string, NumericComparator>& getLoadedTextureInfo(std::string_view textureID);

    std::vector<CMesh>& getMeshes() noexcept { return m_meshes; }
    std::vector<CMaterial>& getMaterials() noexcept { return m_materials; }

    bool operator==(const Model& other) const { return m_filepath == other.m_filepath; }

  private:
    std::string m_filepath{};
    std::string m_directory{};
    std::vector<CMesh> m_meshes{};
    std::vector<CMaterial> m_materials{};
    std::vector<std::shared_ptr<Texture>> m_loadedTextures{};

    std::map<std::string, std::string, NumericComparator> m_info{};
    std::map<std::string, std::map<std::string, std::string, NumericComparator>, NumericComparator>
        m_loadedTextureInfo{};

    void processNode(aiNode* node, const aiScene* scene);
    CMesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType t, std::string type);
    CMaterial loadMaterial(aiMaterial* mat);
};

}
