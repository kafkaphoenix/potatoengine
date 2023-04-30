#include "vao.h"

#include <glad/glad.h>

namespace potatoengine {

    VAO::VAO() {
        glCreateVertexArrays(1, &m_id);
    }

    VAO::~VAO() {
        glDeleteVertexArrays(1, &m_id);
    }

    void VAO::bind() const {
        glBindVertexArray(m_id);
    }

    void VAO::attachVertex(const Ref<VBO>& vbo) {
        glVertexArrayVertexBuffer(m_id, 0, vbo->getId(), 0, sizeof(Vertex));
        m_vbos.emplace_back(vbo);
    
        glEnableVertexArrayAttrib(m_id, 0);
        glVertexArrayAttribFormat(m_id, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
        glVertexArrayAttribBinding(m_id, 0, m_vboIndex);

        glEnableVertexArrayAttrib(m_id, 1);
        glVertexArrayAttribFormat(m_id, 1, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, col));
        glVertexArrayAttribBinding(m_id, 1, m_vboIndex);
        
        glEnableVertexArrayAttrib(m_id, 2);
        glVertexArrayAttribFormat(m_id, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));
        glVertexArrayAttribBinding(m_id, 2, m_vboIndex);

        ++m_vboIndex;
    }

    void VAO::setIndex(const Ref<IBO>& ibo) {
        glVertexArrayElementBuffer(m_id, ibo->getId());
        m_ibo = ibo;
    }

    Ref<VAO> VAO::Create() {
        return createRef<VAO>();
    }
}