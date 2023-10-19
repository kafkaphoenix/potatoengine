#pragma once

#include <assimp/scene.h>

#include "potatoengine/pch.h"
#include "potatoengine/scene/components/graphics/cMaterial.h"
#include "potatoengine/scene/components/graphics/cMesh.h"
#include "potatoengine/assets/texture.h"

namespace potatoengine {

class Model {
   public:
    Model(std::filesystem::path&& fp, std::optional<bool> gammaCorrection = std::nullopt);
    Model& operator=(const Model&) = delete;

    std::string_view getFilepath() const noexcept { return m_filepath; }

    std::vector<CMesh>& getMeshes() noexcept { return m_meshes; }

    std::vector<CMaterial>& getMaterials() noexcept { return m_materials; }

    bool operator==(const Model& other) const {
        return m_filepath == other.m_filepath;
    }

   private:
    std::string m_filepath{};
    std::string m_directory{};
    std::vector<CMesh> m_meshes{};
    std::vector<CMaterial> m_materials{};
    std::vector<std::shared_ptr<Texture>> m_loadedTextures{};

    void processNode(aiNode* node, const aiScene* scene);
    CMesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType t, std::string type);
    CMaterial loadMaterial(aiMaterial* mat);
};

}
