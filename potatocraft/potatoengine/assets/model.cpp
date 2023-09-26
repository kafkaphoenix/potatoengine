#include "potatoengine/assets/model.h"

#include <assimp/postprocess.h>

#include <assimp/Importer.hpp>

#include "potatoengine/renderer/buffer.h"

namespace potatoengine {

Model::Model(std::filesystem::path&& fp, std::optional<bool> gammaCorrection): m_filepath(std::move(fp.string())), m_directory(std::move(fp.parent_path().string())) {
    if (gammaCorrection.has_value()) {
        throw std::runtime_error("Gamma correction not yet implemented");
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_filepath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                                                             aiProcess_CalcTangentSpace | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices |
                                                             aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_SplitLargeMeshes | aiProcess_FindInvalidData);

    if (not scene or scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or !scene->mRootNode) {
        throw std::runtime_error("Failed to load model " + m_filepath + ": " + importer.GetErrorString());
    }

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
        m_meshes.emplace_back(std::move(processMesh(mesh, scene)));
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

CMesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
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

        if (mesh->mAABB.mMin not_eq aiVector3D(0.f, 0.f, 0.f) and mesh->mAABB.mMax not_eq aiVector3D(0.f, 0.f, 0.f)) {
            // TODO aabb
        }

        if (mesh->HasFaces()) {
            // TODO faces
        }

        vertices.emplace_back(std::move(vertex));
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    m_materials.emplace_back(std::move(loadMaterial(material)));

    // N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // diffuse: textureDiffuseN
    // specular: textureSpecularN
    // normal: textureNormalN
    // height: textureHeightN
    auto loadAndInsertTextures = [&](aiTextureType t, std::string type) {
        std::vector<std::shared_ptr<Texture>> loadedTextures = loadMaterialTextures(material, t, type);
        textures.insert(textures.end(), loadedTextures.begin(), loadedTextures.end()); // Can't be emplace
    };

    loadAndInsertTextures(aiTextureType_DIFFUSE, "textureDiffuse");
    loadAndInsertTextures(aiTextureType_SPECULAR, "textureSpecular");
    loadAndInsertTextures(aiTextureType_HEIGHT, "textureNormal");
    loadAndInsertTextures(aiTextureType_AMBIENT, "textureHeight");

    if (textures.empty()) {
        textures.emplace_back(std::make_shared<Texture>("assets/textures/default.jpg", "textureDiffuse"));  // TODO: add asset manager?
    }

    return CMesh(std::move(vertices), std::move(indices), std::move(textures));
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

        if (loadedTexture not_eq m_loadedTextures.end()) {
            textures.emplace_back(std::move(*loadedTexture));
        } else {
            std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(filepath, type);
            textures.emplace_back(newTexture);
            m_loadedTextures.emplace_back(std::move(newTexture));
        }
    }

    return textures;
}

CMaterial Model::loadMaterial(aiMaterial* mat) {
    CMaterial material{};
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
