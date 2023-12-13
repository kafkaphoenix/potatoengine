#pragma once

#include <glm/glm.hpp>

#include "scene/components/world/cChunk.h"
#include "scene/components/world/cChunkManager.h"

namespace potatoengine {
using ChunkMap = std::unordered_map<glm::ivec3, CChunk>;
class Entity;

class ChunkManager {
  public:
    void loadConfig(Entity&& config);
    void loadChunk(glm::ivec3 position);
    void createChunk(Entity&& chunk, glm::ivec3 position);
    void updateChunk(glm::ivec3 position);
    void clearChunk(glm::ivec3 position);
    void print();

  private:
    ChunkMap m_loadedChunks;
    CChunkManager m_managerConfig;
};
}