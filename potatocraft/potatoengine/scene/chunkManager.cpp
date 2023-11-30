#include "potatoengine/scene/chunkManager.h"
#include "potatoengine/utils/uuid.h"
#include "potatoengine/utils/multiArray.h"
#include "potatoengine/scene/entity.h"

namespace potatoengine {


void ChunkManager::loadConfig(Entity&& config) {
    //m_managerConfig = config.get<CChunkManager>();
}

// void ChunkManager::loadChunk(std::string_view id) {
//     if (m_loadedChunks.contains(id.data())) {
//         return;
//     }
//     m_loadedChunks.emplace(id.data(), ChunkLoader{m_assetsManager, m_registry, id});
// }

void ChunkManager::createChunk(Entity&& chunk, glm::ivec3 position) {
    // ENGINE_ASSERT(not m_loadedChunks.contains(position), "Chunk already exists");
    
    // CBlock block;
    // glm::ivec3 blockPosition{};
    // BlockArray blocks;
    // CChunk& cChunk = chunk.get<CChunk>();
    // for (int x = 0; x < cChunk.width; ++x) {
    //     for (int y = 0; y < cChunk.height; ++y) {
    //         for (int z = 0; z < cChunk.depth; ++z) {
    //             block._type = "stone";
    //             block.setBlockType();
    //             blocks[x][y][z] = block;
    //         }
    //     }
    // }
    // cChunk.blocks = blocks;
    // cChunk.buildMesh();
    // m_loadedChunks.emplace(position, cChunk);
}   

// void ChunkManager::updateChunk(std::string_view id) {
//     if (not m_loadedChunks.contains(id.data())) {
//         return;
//     }
//     m_loadedChunks.at(id.data()).update();
// }

// void ChunkManager::clearChunk(std::string_view id) {
//     if (not m_loadedChunks.contains(id.data())) {
//         return;
//     }
//     m_loadedChunks.at(id.data()).clear();
//     m_loadedChunks.erase(id.data());
// }

// void ChunkManager::print() {
//     for (auto& [id, chunk] : m_loadedChunks) {
//         chunk.print();
//     }
// }
}