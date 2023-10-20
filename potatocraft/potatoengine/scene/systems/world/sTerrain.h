#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/graphics/cMesh.h"
#include "potatoengine/scene/components/graphics/cTexture.h"
#include "potatoengine/scene/components/utils/cNoise.h"
#include "potatoengine/scene/components/world/cChunk.h"
#include "potatoengine/scene/components/world/cChunkManager.h"

namespace potatoengine {

static constexpr glm::vec3 LIGHT_BLUE = {0.0f, 0.9725f, 0.9725f};
static constexpr glm::vec3 LIGHT_YELLOW = {0.9725f, 0.9725f, 0.0f};
static constexpr glm::vec3 LIGHT_GREEN = {0.0f, 0.9725f, 0.0f};
static constexpr glm::vec3 DARK_GREEN = {0.0f, 0.5f, 0.0f};
static constexpr glm::vec3 LIGHT_GREY = {0.5f, 0.5f, 0.5f};
static constexpr glm::vec3 WHITE = {0.9725f, 0.9725f, 0.9725f};

glm::vec3 calculateTriangleNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
    glm::vec3 u = v2 - v1;
    glm::vec3 v = v3 - v1;

    return glm::normalize(glm::cross(u, v));
}

glm::vec3 calculateQuadNormal(int col, int row, const std::vector<std::vector<float>>& heights) {
    const float heightLeft = col > 0 ? heights[row][col - 1] : heights[row][col];
    const float heightRight = col < heights[row].size() - 1 ? heights[row][col + 1] : heights[row][col];
    const float heightDown = row > 0 ? heights[row - 1][col] : heights[row][col];
    const float heightUp = row < heights.size() - 1 ? heights[row + 1][col] : heights[row][col];

    return glm::normalize(glm::vec3(heightLeft - heightRight, 2.f, heightDown - heightUp));
}

glm::vec3 calculateBiomeColor(float height, int amplitude) {
    height = (height + amplitude) / (2 * amplitude);

    if (height < 0.2f) {
        return LIGHT_BLUE;
    } else if (height < 0.3f) {
        return LIGHT_YELLOW;
    } else if (height < 0.6f) {
        return LIGHT_GREEN;
    } else if (height < 0.8f) {
        return DARK_GREEN;
    } else if (height < 0.9f) {
        return LIGHT_GREY;
    } else {
        return WHITE;
    }
}

glm::vec3 calculateBiomeTexture(float height, int amplitude, const CTextureAtlas& cTextureAtlas) {
    height = (height + amplitude) / (2 * amplitude);

    int rows = cTextureAtlas.rows;
    int index = 5;  // default

    if (height < 0.2f) {  // TODO contants
        index = 4;        // water
    } else if (height < 0.3f) {
        index = 3;  // sand
    } else if (height < 0.6f) {
        index = 2;  // grass
    } else if (height < 0.8f) {
        index = 1;  // dirt
    } else if (height < 0.9f) {
        index = 8;  // stone
    } else {
        index = 6;  // snow
    }

    int col = index % rows;
    float coll = static_cast<float>(col) / rows;
    int row = index / rows;
    float roww = static_cast<float>(row) / rows;

    return {static_cast<float>(coll) / rows, static_cast<float>(roww) / rows, 0.f};
}

std::vector<std::vector<float>> generateHeights(int chunkSize, const CNoise& noise, int xOffset, int zOffset) {
    std::vector<std::vector<float>> heights;  // z = row, x = col
    heights.reserve(chunkSize + 1);
    for (int row = 0; row < chunkSize + 1; ++row) {
        heights.emplace_back(std::vector<float>());
        heights[row].reserve(chunkSize + 1);
        for (int col = 0; col < chunkSize + 1; ++col) {
            heights[row].emplace_back(noise.getNoise(static_cast<float>(col + xOffset), static_cast<float>(row + zOffset)));
        }
    }

    return heights;
}

std::vector<std::vector<glm::vec3>> generateBiomes(std::vector<std::vector<float>> heights, int amplitude) {
    std::vector<std::vector<glm::vec3>> biomes;  // z = row, x = col
    biomes.reserve(heights.size());
    for (int row = 0; row < heights.size(); ++row) {
        biomes.emplace_back(std::vector<glm::vec3>());
        biomes[row].reserve(heights[row].size());
        for (int col = 0; col < heights[row].size(); ++col) {
            biomes[row].emplace_back(calculateBiomeColor(heights[row][col], amplitude));
        }
    }

    return biomes;
}

std::vector<std::vector<glm::vec3>> generateBiomesTextures(std::vector<std::vector<float>> heights, int amplitude, const CTextureAtlas& cTextureAtlas) {
    std::vector<std::vector<glm::vec3>> textures;  // z = row, x = col
    textures.reserve(heights.size());
    for (int row = 0; row < heights.size(); ++row) {
        textures.emplace_back(std::vector<glm::vec3>());
        textures[row].reserve(heights[row].size());
        for (int col = 0; col < heights[row].size(); ++col) {
            textures[row].emplace_back(calculateBiomeTexture(heights[row][col], amplitude, cTextureAtlas));
        }
    }

    return textures;
}

CMesh generateTriangleMesh(int chunkSize, int blockSize, const std::vector<std::vector<float>>& heights, const std::optional<std::reference_wrapper<std::vector<std::vector<glm::vec3>>>> biomes = std::nullopt) {
    CMesh mesh;
    int totalVertices = (chunkSize + 1) * (chunkSize + 1);  // Triangles share vertices making impossible to use more than one texture for all the mesh

    std::vector<TerrainVertex> vertices;
    vertices.reserve(totalVertices);

    std::vector<uint32_t> indices;
    indices.reserve(chunkSize * chunkSize * 6);

    // c++ is row major order (x, y, z) -> (width, height, depth) -> (col, height, row)
    for (int row = 0; row < chunkSize + 1; ++row) {
        for (int col = 0; col < chunkSize + 1; ++col) {
            float x = static_cast<float>(col) * blockSize;
            float y = heights[row][col] * blockSize;
            float z = static_cast<float>(row) * blockSize;

            glm::vec3 position(x, y, z);
            glm::vec3 normal = std::move(calculateQuadNormal(col, row, heights));  // TODO maybe this normal is not correct and should call the other method
            glm::vec2 textureCoords{};
            glm::vec3 color{};
            if (biomes.has_value()) {  // color per vertex
                color = biomes.value().get()[row][col];
            } else {  // one texture for all the mesh
                textureCoords = {static_cast<float>(col) / chunkSize, static_cast<float>(row) / chunkSize};
            }

            vertices.push_back({position, normal, textureCoords, color});
        }
    }

    for (int row = 0; row < chunkSize; ++row) {
        for (int col = 0; col < chunkSize; ++col) {
            uint32_t topLeft = (row * (chunkSize + 1)) + col;
            uint32_t topRight = topLeft + 1;
            uint32_t bottomLeft = ((row + 1) * (chunkSize + 1)) + col;
            uint32_t bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    mesh.vertexType = "terrain";
    mesh.vbo = VBO::CreateTerrain(vertices);
    mesh.indices = std::move(indices);
    return mesh;
}

void addQuadVertexData(std::vector<TerrainVertex>& vertices, std::vector<uint32_t>& indices, const std::array<glm::vec3, 4>& positions, glm::vec3 normal, const std::array<glm::vec2, 4>& textureCoordinates, const std::array<glm::vec3, 4>& colors) {
    uint32_t topLeftIndex = vertices.size();
    uint32_t topRightIndex = topLeftIndex + 1;
    uint32_t bottomLeftIndex = topLeftIndex + 2;
    uint32_t bottomRightIndex = topLeftIndex + 3;

    vertices.push_back({positions[0], normal, textureCoordinates[0], colors[0]});  // top left
    vertices.push_back({positions[2], normal, textureCoordinates[2], colors[2]});  // top right
    vertices.push_back({positions[1], normal, textureCoordinates[1], colors[1]});  // bottom left
    vertices.push_back({positions[3], normal, textureCoordinates[3], colors[3]});  // bottom right

    indices.push_back(topLeftIndex);
    indices.push_back(bottomLeftIndex);
    indices.push_back(topRightIndex);
    indices.push_back(topRightIndex);
    indices.push_back(bottomLeftIndex);
    indices.push_back(bottomRightIndex);
}

void addTriangleVertexData(std::vector<TerrainVertex>& vertices, std::vector<uint32_t>& indices, glm::vec3 first, glm::vec3 second, glm::vec3 third, glm::vec3 normal, glm::vec2 firstTextureCoordinates, glm::vec2 secondTextureCoordinates, glm::vec2 thirdTextureCoordinates, glm::vec3 firstColor, glm::vec3 secondColor, glm::vec3 thirdColor) {
    uint32_t firstIndex = vertices.size();
    uint32_t secondIndex = firstIndex + 1;
    uint32_t thirdIndex = firstIndex + 2;

    vertices.push_back({first, normal, firstTextureCoordinates, firstColor});
    vertices.push_back({second, normal, secondTextureCoordinates, secondColor});
    vertices.push_back({third, normal, thirdTextureCoordinates, thirdColor});

    indices.push_back(firstIndex);
    indices.push_back(secondIndex);
    indices.push_back(thirdIndex);
}

std::array<glm::vec3, 4> calculateQuadPositions(int col, int row, const std::vector<std::vector<float>>& heights, int blockSize) {
    std::array<glm::vec3, 4> positions;
    positions[0] = {static_cast<float>(col) * blockSize, heights[row][col] * blockSize, static_cast<float>(row) * blockSize};                  // top left
    positions[1] = {static_cast<float>(col) * blockSize, heights[row + 1][col] * blockSize, static_cast<float>(row + 1) * blockSize};          // bottom left
    positions[2] = {static_cast<float>(col + 1) * blockSize, heights[row][col + 1] * blockSize, static_cast<float>(row) * blockSize};          // top right
    positions[3] = {static_cast<float>(col + 1) * blockSize, heights[row + 1][col + 1] * blockSize, static_cast<float>(row + 1) * blockSize};  // bottom right

    return positions;
}

std::array<glm::vec3, 4> calculateBiomeColors(int col, int row, const std::vector<std::vector<glm::vec3>>& biomes) {
    std::array<glm::vec3, 4> colors;
    colors[0] = biomes[row][col];          // top left
    colors[1] = biomes[row + 1][col];      // bottom left
    colors[2] = biomes[row][col + 1];      // top right
    colors[3] = biomes[row + 1][col + 1];  // bottom right

    return colors;
}

std::array<glm::vec2, 4> calculateBiomeTextures(int col, int row, const std::vector<std::vector<glm::vec3>>& biomes) {
    std::array<glm::vec2, 4> textureCoordinates;  // TODO Do i need to multiply by blocksize?
    // TODO this is the offset I still need the texture coordinates
    textureCoordinates[0] = glm::vec2(biomes[row][col]);          // top left
    textureCoordinates[1] = glm::vec2(biomes[row + 1][col]);      // bottom left
    textureCoordinates[2] = glm::vec2(biomes[row][col + 1]);      // top right
    textureCoordinates[3] = glm::vec2(biomes[row + 1][col + 1]);  // bottom right

    return textureCoordinates;
}

void addQuad(std::vector<TerrainVertex>& vertices, std::vector<uint32_t>& indices, int col, int row, int chunkSize, int blockSize, bool duplicateVertices, CTexture::DrawMode drawMode, const std::vector<std::vector<float>>& heights, const std::optional<std::reference_wrapper<std::vector<std::vector<glm::vec3>>>> biomes = std::nullopt) {
    std::array<glm::vec3, 4> positions = calculateQuadPositions(col, row, heights, blockSize);
    std::array<glm::vec2, 4> textureCoordinates{};
    std::array<glm::vec3, 4> colors{};

    if (biomes.has_value()) {
        if (drawMode == CTexture::DrawMode::COLOR) {
            colors = calculateBiomeColors(col, row, biomes.value());
        } else if (drawMode == CTexture::DrawMode::TEXTURE_ATLAS) {
            textureCoordinates = calculateBiomeTextures(col, row, biomes.value());
        }
    }

    if (duplicateVertices) {
        addTriangleVertexData(vertices, indices, positions[0], positions[1], positions[2], calculateTriangleNormal(positions[0], positions[1], positions[2]), textureCoordinates[0], textureCoordinates[1], textureCoordinates[2], colors[0], colors[1], colors[2]);
        addTriangleVertexData(vertices, indices, positions[2], positions[1], positions[3], calculateTriangleNormal(positions[2], positions[1], positions[3]), textureCoordinates[2], textureCoordinates[1], textureCoordinates[3], colors[2], colors[1], colors[3]);
    } else {
        addQuadVertexData(vertices, indices, positions, calculateQuadNormal(col, row, heights), textureCoordinates, colors);
    }
}

CMesh generateQuadMesh(int chunkSize, int blockSize, bool duplicateVertices, CTexture::DrawMode drawMode, const std::vector<std::vector<float>>& heights, const std::optional<std::reference_wrapper<std::vector<std::vector<glm::vec3>>>> biomes = std::nullopt) {
    CMesh mesh;
    int totalVertices = chunkSize * chunkSize * 4;  // 4 vertices per quad, triangles share vertices
    if (duplicateVertices) {
        totalVertices = chunkSize * chunkSize * 6;  // 6 vertices per quad (2 triangles) for higher quality lighting
    }

    std::vector<TerrainVertex> vertices;
    vertices.reserve(totalVertices);

    std::vector<uint32_t> indices;
    indices.reserve(chunkSize * chunkSize * 6);

    // c++ is row major order (x, y, z) -> (width, height, depth) -> (col, height, row)
    for (int row = 0; row < chunkSize; ++row) {
        for (int col = 0; col < chunkSize; ++col) {
            if (biomes.has_value()) {
                addQuad(vertices, indices, col, row, chunkSize, blockSize, duplicateVertices, drawMode, heights, biomes);
            } else {
                addQuad(vertices, indices, col, row, chunkSize, blockSize, duplicateVertices, drawMode, heights);
            }
        }
    }

    mesh.vertexType = "terrain";
    mesh.vbo = VBO::CreateTerrain(vertices);
    mesh.indices = std::move(indices);
    return mesh;
}

CMesh generateTerrain(CChunkManager::MeshType meshType, CChunkManager::MeshAlgorithm meshAlgorithm, int chunkSize, int blockSize, CTexture::DrawMode drawMode, const std::vector<std::vector<float>>& heights, const std::optional<std::reference_wrapper<std::vector<std::vector<glm::vec3>>>> biomes = std::nullopt) {
    if (meshType == CChunkManager::MeshType::Plane) {
        if (meshAlgorithm == CChunkManager::MeshAlgorithm::SimplifiedTriangle) {
            if (biomes.has_value()) {
                return generateTriangleMesh(chunkSize, blockSize, heights, biomes);
            } else if (drawMode == CTexture::DrawMode::TEXTURE) {
                return generateTriangleMesh(chunkSize, blockSize, heights);
            } else {
                throw std::runtime_error("Draw mode " + std::to_string(static_cast<int>(drawMode)) + " not supported for mesh type " + std::to_string(static_cast<int>(meshType)) + " and mesh algorithm " + std::to_string(static_cast<int>(meshAlgorithm)));
            }
        } else if (meshAlgorithm == CChunkManager::MeshAlgorithm::SimplifiedQuad or meshAlgorithm == CChunkManager::MeshAlgorithm::Quad) {
            bool duplicateVertices = meshAlgorithm == CChunkManager::MeshAlgorithm::Quad;
            if (biomes.has_value()) {
                return generateQuadMesh(chunkSize, blockSize, duplicateVertices, drawMode, heights, biomes);
            } else {
                throw std::runtime_error("Draw mode " + std::to_string(static_cast<int>(drawMode)) + " not supported for mesh type " + std::to_string(static_cast<int>(meshType)) + " and mesh algorithm " + std::to_string(static_cast<int>(meshAlgorithm)));
            }
        }
    } else if (meshType == CChunkManager::MeshType::Chunk) {
        if (meshAlgorithm == CChunkManager::MeshAlgorithm::Greedy) {
            // return generateChunkMesh(chunkSize, blockSize, heights);
        }
    } else if (meshType == CChunkManager::MeshType::Sphere) {
        // return generateSphereMesh(chunkSize, blockSize, heights);
    }
}

void terrainSystem(entt::registry& reg) {  // TODO transform
    using namespace entt::literals;
    reg.view<CChunkManager, CTexture, CNoise, CUUID>().each([&](entt::entity e, CChunkManager& cChunkManager, const CTexture& cTexture, const CNoise& cNoise, const CUUID& cUUID) {
        CTextureAtlas* cTextureAtlas = nullptr;
        if (cTexture.drawMode == CTexture::DrawMode::TEXTURE_ATLAS or cTexture.drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND or cTexture.drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND_COLOR) {
            cTextureAtlas = reg.try_get<CTextureAtlas>(e);
            if (not cTextureAtlas) {
                throw std::runtime_error("Texture atlas not found for entity: " + cUUID.uuid);
            }
        }

        if (cChunkManager.chunks.empty()) {
            // Create chunks around 0 0 0
            // TODO y axis should infinite, maybe rename z to depth
            std::vector<std::vector<float>> heights;
            std::vector<std::vector<glm::vec3>> biomes;
            CChunk chunk;
            for (int row = -cChunkManager.width; row <= cChunkManager.width; ++row) {
                for (int col = -cChunkManager.height; col <= cChunkManager.height; ++col) {
                    chunk = CChunk{"plains"};
                    heights = generateHeights(cChunkManager.chunkSize, cNoise, col, row);
                    if (cChunkManager.useBiomes) {
                        if (cTexture.drawMode == CTexture::DrawMode::COLOR) {
                            biomes = generateBiomes(heights, cNoise.amplitude);
                        } else if (cTexture.drawMode == CTexture::DrawMode::TEXTURE_ATLAS or cTexture.drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND or cTexture.drawMode == CTexture::DrawMode::TEXTURE_ATLAS_BLEND_COLOR) {
                            biomes = generateBiomesTextures(heights, cNoise.amplitude, *cTextureAtlas);
                        }
                        chunk.terrainMesh = std::move(generateTerrain(cChunkManager.meshType, cChunkManager.meshAlgorithm, cChunkManager.chunkSize, cChunkManager.blockSize, cTexture.drawMode, heights, biomes));
                    } else {
                        chunk.terrainMesh = std::move(generateTerrain(cChunkManager.meshType, cChunkManager.meshAlgorithm, cChunkManager.chunkSize, cChunkManager.blockSize, cTexture.drawMode, heights));
                    }
                    chunk.transform.position = glm::vec3{static_cast<float>(col) * cChunkManager.chunkSize * cChunkManager.blockSize, 0.f, static_cast<float>(row) * cChunkManager.chunkSize * cChunkManager.blockSize};
                    cChunkManager.chunks.emplace(glm::vec3{static_cast<float>(col) * cChunkManager.chunkSize * cChunkManager.blockSize, 0.f, static_cast<float>(row) * cChunkManager.chunkSize * cChunkManager.blockSize}, std::move(chunk));  // TODO check if this work with blocksize
                }
            }
        }
    });
}
}