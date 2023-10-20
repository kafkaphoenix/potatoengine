#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "glm/gtx/hash.hpp"

#include "potatoengine/scene/components/world/cChunk.h"

namespace potatoengine {

struct CChunkManager {
    enum class MeshType {
        Plane,
        Sphere,
        Chunk,
    };

    enum class MeshAlgorithm {
        // Plane
        SimplifiedTriangle, // 1 vertex pero triangle
        SimplifiedQuad, // 4 vertices per quad
        Quad, // 6 vertices per quad
        // Chunk
        Greedy, // optimize quads
    };

    int chunkSize{16};
    int blockSize{1};
    int width{3};
    int height{3};
    std::unordered_map<glm::vec3, CChunk> chunks;  // TODO: change to ivec3
    std::string _meshType{};
    MeshType meshType;
    std::string _meshAlgorithm{};
    MeshAlgorithm meshAlgorithm;
    bool useBiomes{};

    CChunkManager() = default;
    explicit CChunkManager(int w, int h, int cs, int bs, std::unordered_map<glm::vec3, CChunk>&& c, std::string&& mt, std::string&& ma, bool ub)
        : width(w), height(h), chunkSize(cs), blockSize(bs), chunks(std::move(c)), _meshType(std::move(mt)), _meshAlgorithm(std::move(ma)), useBiomes(ub) {}
    void print() const {
        std::string c;
        for (const auto& [position, chunk] : chunks) {
            c += "\n\t\t\t\t\tchunk: " + glm::to_string(position) + " " + chunk._biome;
        }
        CORE_INFO("\t\twidth: {0}\n\t\t\t\theight: {1}\n\t\t\t\tchunkSize: {2}\n\t\t\t\tblockSize: {3}\n\t\t\t\tmeshType: {4}\n\t\t\t\tmeshAlgorithm: {5}\n\t\t\t\tuseBiomes: {6}\n\t\t\t\tchunks: {7}",
                  width, height, chunkSize, blockSize, _meshType, _meshAlgorithm, useBiomes, c);
    }

    void setMeshType() {
        if (_meshType == "plane") {
            this->meshType = MeshType::Plane;
        } else if (_meshType == "sphere") {
            this->meshType = MeshType::Sphere;
        } else if (_meshType == "chunk") {
            this->meshType = MeshType::Chunk;
        } else {
            throw std::runtime_error("Unknown mesh type " + _meshType);
        }
    }

    void setMeshAlgorithm() {
        if (_meshAlgorithm == "simplifiedTriangle") {
            this->meshAlgorithm = MeshAlgorithm::SimplifiedTriangle;
        } else if (_meshAlgorithm == "simplifiedQuad") {
            this->meshAlgorithm = MeshAlgorithm::SimplifiedQuad;
        } else if (_meshAlgorithm == "quad") {
            this->meshAlgorithm = MeshAlgorithm::Quad;
        } else if (_meshAlgorithm == "greedy") {
            this->meshAlgorithm = MeshAlgorithm::Greedy;
        } else {
            throw std::runtime_error("Unknown mesh algorithm " + _meshAlgorithm);
        }
    }
};
}
