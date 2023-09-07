#pragma once

#include <assimp/scene.h>

#include "potatoengine/pch.h"
#include "potatoengine/scene/components.h"

namespace potatoengine {

class Model {
   public:
    Model(const std::filesystem::path& fp, std::optional<bool> gammaCorrection = std::nullopt);

    std::string_view getFilepath() const noexcept { return m_filepath; }

    std::vector<Mesh>& getMeshes() noexcept { return m_meshes; }

    std::vector<Material>& getMaterials() noexcept { return m_materials; }

    bool operator==(const Model& other) const {
        return m_filepath == other.m_filepath;
    }

   private:
    std::string m_filepath{};
    std::string m_directory{};
    std::vector<Mesh> m_meshes{};
    std::vector<Material> m_materials{};
    std::vector<std::shared_ptr<Texture>> m_loadedTextures{};

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType t, std::string type);
    Material loadMaterial(aiMaterial* mat);
};

}
