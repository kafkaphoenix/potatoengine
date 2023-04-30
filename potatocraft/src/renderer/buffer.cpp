#include "src/pch.h"
#include "src/renderer/buffer.h"
#include <glad/glad.h>

namespace potatoengine {

    constexpr GLbitfield 
	mapping_flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT,
	storage_flags = GL_DYNAMIC_STORAGE_BIT | mapping_flags;

    VBO::VBO(const std::vector<Vertex>& vertices) {
        glCreateBuffers(1, &m_id);
        glNamedBufferStorage(m_id, sizeof(Vertex) * vertices.size(), vertices.data(), storage_flags);
    }

    VBO::~VBO() {
        glDeleteBuffers(1, &m_id);
    }

    Ref<VBO> VBO::Create(const std::vector<Vertex>& vertices) {
        return createRef<VBO>(vertices);
    }

    IBO::IBO(const std::vector<uint32_t>& indices) : m_count(indices.size()) {
        glCreateBuffers(1, &m_id);
        glNamedBufferStorage(m_id, sizeof(uint32_t) * indices.size(), indices.data(), storage_flags);
    }

    IBO::~IBO() {
        glDeleteBuffers(1, &m_id);
    }

    Ref<IBO> IBO::Create(const std::vector<uint32_t>& indices) {
        return createRef<IBO>(indices);
    }
}