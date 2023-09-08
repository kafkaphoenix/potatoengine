#include "potatoengine/assets/model.h"

#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>

namespace potatoengine {

Model::Model(const std::filesystem::path& fp, std::optional<bool> gammaCorrection) {
    if (!std::filesystem::exists(fp)) [[unlikely]] {
        throw std::runtime_error("Model file does not exist: " + fp.string());
    }
    m_filepath = fp.string();

    if (gammaCorrection.has_value()) {
        throw std::runtime_error("Gamma correction not yet implemented");
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_filepath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                                                             aiProcess_CalcTangentSpace | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices |
                                                             aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_SplitLargeMeshes | aiProcess_FindInvalidData);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Failed to load model " + m_filepath + ": " + importer.GetErrorString());
    }

    m_directory = fp.parent_path().string();

    processNode(scene->mRootNode, scene);

#ifdef DEBUG
    CORE_INFO("\t\tmesh count: {0}", m_meshes.size());
    CORE_INFO("\t\ttexture count: {0}", m_loadedTextures.size());
    CORE_INFO("\t\tmaterial count: {0}", m_materials.size());
#endif
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};
    std::vector<std::shared_ptr<Texture>> textures;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        const auto& position = mesh->mVertices[i];  // assimp's vector doesn't directly convert to glm's vec3
        vertex.position = glm::vec3(position.x, position.y, position.z);

        if (mesh->HasNormals()) {
            const auto& normalVector = mesh->mNormals[i];
            vertex.normal = glm::vec3(normalVector.x, normalVector.y, normalVector.z);
        }

        if (mesh->mTextureCoords[0]) {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            const auto& textureCoords = mesh->mTextureCoords[0][i];
            vertex.textureCoords = glm::vec2(textureCoords.x, textureCoords.y);

            const auto& tangentVector = mesh->mTangents[i];
            vertex.tangent = glm::vec3(tangentVector.x, tangentVector.y, tangentVector.z);

            const auto& bitangentVector = mesh->mBitangents[i];
            vertex.bitangent = glm::vec3(bitangentVector.x, bitangentVector.y, bitangentVector.z);
        }

        if (mesh->HasBones()) {
            // TODO bones
        }

        if (mesh->HasVertexColors(0)) {
            const auto& color = mesh->mColors[0][i];
            vertex.color = glm::vec4(color.r, color.g, color.b, color.a);
        }

        if (mesh->mAABB.mMin != aiVector3D(0.f, 0.f, 0.f) and mesh->mAABB.mMax != aiVector3D(0.f, 0.f, 0.f)) {
            // TODO aabb
        }

        if (mesh->HasFaces()) {
            // TODO faces
        }

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    m_materials.push_back(loadMaterial(material));

    // N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN
    // height: texture_heightN
    auto loadAndInsertTextures = [&](aiTextureType t, std::string type) {
        std::vector<std::shared_ptr<Texture>> loadedTextures = loadMaterialTextures(material, t, type);
        textures.insert(textures.end(), loadedTextures.begin(), loadedTextures.end());
    };

    loadAndInsertTextures(aiTextureType_DIFFUSE, "texture_diffuse");
    loadAndInsertTextures(aiTextureType_SPECULAR, "texture_specular");
    loadAndInsertTextures(aiTextureType_HEIGHT, "texture_normal");
    loadAndInsertTextures(aiTextureType_AMBIENT, "texture_height");

    if (textures.empty()) {
        textures.push_back(std::make_shared<Texture>("assets/textures/default.jpg", "texture_diffuse"));
    }

    return Mesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType t, std::string type) {
    std::vector<std::shared_ptr<Texture>> textures;
    for (uint32_t i = 0; i < mat->GetTextureCount(t); i++) {
        aiString source;
        mat->GetTexture(t, i, &source);
        std::string filename = source.C_Str();
        std::string filepath = m_directory + "/" + filename;

        auto loadedTexture = std::find_if(m_loadedTextures.begin(), m_loadedTextures.end(),
                                          [&](const std::shared_ptr<Texture>& texture) {
                                              return texture->getFilepath() == filepath;
                                          });

        if (loadedTexture != m_loadedTextures.end()) {
            textures.push_back(*loadedTexture);
        } else {
            std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(filepath, type);
            textures.push_back(newTexture);
            m_loadedTextures.push_back(newTexture);
        }
    }

    return textures;
}

Material Model::loadMaterial(aiMaterial* mat) {
    Material material{};
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess{};

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.shininess = shininess;

    return material;
}

}
