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

void VAO::attachVertex(std::unique_ptr<VBO> vbo) {
    glVertexArrayVertexBuffer(m_id, 0, vbo->getID(), 0, sizeof(Vertex));
    m_vbos.emplace_back(std::move(vbo));

    glEnableVertexArrayAttrib(m_id, 0);
    glVertexArrayAttribFormat(m_id, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribBinding(m_id, 0, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 1);
    glVertexArrayAttribFormat(m_id, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nor));
    glVertexArrayAttribBinding(m_id, 1, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 2);
    glVertexArrayAttribFormat(m_id, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));
    glVertexArrayAttribBinding(m_id, 2, m_vboIdx);

    ++m_vboIdx;
}

void VAO::setIndex(std::unique_ptr<IBO> ibo) {
    glVertexArrayElementBuffer(m_id, ibo->getID());
    m_ibo = std::move(ibo);
}

std::shared_ptr<VAO> VAO::Create() {
    return std::make_shared<VAO>();
}
}