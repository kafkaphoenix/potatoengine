#include "vao.h"

#include <glad/glad.h>

namespace potatoengine {

VAO::VAO() { glCreateVertexArrays(1, &m_id); }

VAO::~VAO() {
  ENGINE_WARN("Deleting VAO {}", m_id);
  glDeleteVertexArrays(1, &m_id);
}

void VAO::bind() { glBindVertexArray(m_id); }

void VAO::unbind() { glBindVertexArray(0); }

void VAO::attachVertex(std::shared_ptr<VBO>&& vbo, VertexType type) {
  size_t vertexSize = 0;
  if (type == VertexType::VERTEX) {
    vertexSize = sizeof(Vertex);
  } else if (type == VertexType::SHAPE_VERTEX) {
    vertexSize = sizeof(ShapeVertex);
  } else if (type == VertexType::TERRAIN_VERTEX) {
    vertexSize = sizeof(TerrainVertex);
  }

  glVertexArrayVertexBuffer(m_id, 0, vbo->getID(), 0, vertexSize);
  m_vbos.emplace_back(std::move(vbo));

  if (type == VertexType::VERTEX) {
    attachVertexAttributes();
  } else if (type == VertexType::SHAPE_VERTEX) {
    attachShapeVertexAttributes();
  } else if (type == VertexType::TERRAIN_VERTEX) {
    attachTerrainVertexAttributes();
  }
}

void VAO::attachVertexAttributes() {
  glEnableVertexArrayAttrib(m_id, 0);
  glVertexArrayAttribFormat(m_id, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
  glVertexArrayAttribBinding(m_id, 0, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 1);
  glVertexArrayAttribFormat(m_id, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
  glVertexArrayAttribBinding(m_id, 1, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 2);
  glVertexArrayAttribFormat(m_id, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, textureCoords));
  glVertexArrayAttribBinding(m_id, 2, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 3);
  glVertexArrayAttribFormat(m_id, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));
  glVertexArrayAttribBinding(m_id, 3, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 4);
  glVertexArrayAttribFormat(m_id, 4, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, bitangent));
  glVertexArrayAttribBinding(m_id, 4, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 5);
  glVertexArrayAttribFormat(m_id, 5, 4, GL_INT, GL_FALSE, offsetof(Vertex, boneIDs));
  glVertexArrayAttribBinding(m_id, 5, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 6);
  glVertexArrayAttribFormat(m_id, 6, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, boneWeights));
  glVertexArrayAttribBinding(m_id, 6, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 7);
  glVertexArrayAttribFormat(m_id, 7, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
  glVertexArrayAttribBinding(m_id, 7, m_vboIDX);

  ++m_vboIDX;
}

void VAO::attachShapeVertexAttributes() {
  glEnableVertexArrayAttrib(m_id, 0);
  glVertexArrayAttribFormat(m_id, 0, 3, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, position));
  glVertexArrayAttribBinding(m_id, 0, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 1);
  glVertexArrayAttribFormat(m_id, 1, 2, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, textureCoords));
  glVertexArrayAttribBinding(m_id, 1, m_vboIDX);

  ++m_vboIDX;
}

void VAO::attachTerrainVertexAttributes() {
  glEnableVertexArrayAttrib(m_id, 0);
  glVertexArrayAttribFormat(m_id, 0, 3, GL_FLOAT, GL_FALSE, offsetof(TerrainVertex, position));
  glVertexArrayAttribBinding(m_id, 0, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 1);
  glVertexArrayAttribFormat(m_id, 1, 3, GL_FLOAT, GL_FALSE, offsetof(TerrainVertex, normal));
  glVertexArrayAttribBinding(m_id, 1, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 2);
  glVertexArrayAttribFormat(m_id, 2, 2, GL_FLOAT, GL_FALSE, offsetof(TerrainVertex, textureCoords));
  glVertexArrayAttribBinding(m_id, 2, m_vboIDX);

  glEnableVertexArrayAttrib(m_id, 3);
  glVertexArrayAttribFormat(m_id, 3, 3, GL_FLOAT, GL_FALSE, offsetof(TerrainVertex, color));
  glVertexArrayAttribBinding(m_id, 3, m_vboIDX);

  ++m_vboIDX;
}

void VAO::updateVertex(std::unique_ptr<VBO>&& vbo, uint32_t idx, VertexType type) {
  // TODO: update only the vbo at idx
}

void VAO::clearVBOs() { // TODO: move to on detach on component? Do i need it?
  m_vbos.clear();
  m_vboIDX = 0;
}

void VAO::setIndex(std::unique_ptr<IBO>&& ibo) {
  glVertexArrayElementBuffer(m_id, ibo->getID());
  m_ibo = std::move(ibo);
}

std::shared_ptr<VAO> VAO::Create() { return std::make_shared<VAO>(); }
}