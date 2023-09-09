#include "vao.h"

#include <glad/glad.h>

namespace potatoengine {

VAO::VAO() {
    glCreateVertexArrays(1, &m_id);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &m_id);
}

void VAO::bind() {
    glBindVertexArray(m_id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

// cppcheck-suppress unusedFunction
void VAO::attachVertex(std::unique_ptr<VBO> vbo) {
    glVertexArrayVertexBuffer(m_id, 0, vbo->getID(), 0, sizeof(Vertex));
    m_vbos.emplace_back(std::move(vbo));

    glEnableVertexArrayAttrib(m_id, 0);
    glVertexArrayAttribFormat(m_id, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribBinding(m_id, 0, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 1);
    glVertexArrayAttribFormat(m_id, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexArrayAttribBinding(m_id, 1, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 2);
    glVertexArrayAttribFormat(m_id, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, textureCoords));
    glVertexArrayAttribBinding(m_id, 2, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 3);
    glVertexArrayAttribFormat(m_id, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));
    glVertexArrayAttribBinding(m_id, 3, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 4);
    glVertexArrayAttribFormat(m_id, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, bitangent));
    glVertexArrayAttribBinding(m_id, 4, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 5);
    glVertexArrayAttribFormat(m_id, 5, 4, GL_INT, GL_FALSE, offsetof(Vertex, boneIDs));
    glVertexArrayAttribBinding(m_id, 5, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 6);
    glVertexArrayAttribFormat(m_id, 6, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, boneWeights));
    glVertexArrayAttribBinding(m_id, 6, m_vboIdx);

    glEnableVertexArrayAttrib(m_id, 7);
    glVertexArrayAttribFormat(m_id, 7, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
    glVertexArrayAttribBinding(m_id, 7, m_vboIdx);

    ++m_vboIdx;
}

// cppcheck-suppress unusedFunction
void VAO::setIndex(std::unique_ptr<IBO> ibo) {
    glVertexArrayElementBuffer(m_id, ibo->getID());
    m_ibo = std::move(ibo);
}

std::shared_ptr<VAO> VAO::Create() noexcept {
    return std::make_shared<VAO>();
}
}