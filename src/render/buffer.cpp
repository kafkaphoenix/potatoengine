#include "render/buffer.h"

#include <glad/glad.h>

namespace potatoengine {

static constexpr GLbitfield mapping_flags =
    GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
static constexpr GLbitfield storage_flags =
    GL_DYNAMIC_STORAGE_BIT | mapping_flags; // allow modification of the buffer but not resizing

VBO::VBO(const std::vector<Vertex>& vertices) : m_count(vertices.size()), m_immutable(true) {
  if (m_immutable) {
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, sizeof(Vertex) * vertices.size(), vertices.data(), storage_flags);
  } else {
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
  }
}

VBO::VBO(const std::vector<ShapeVertex>& vertices): m_count(vertices.size()) {
  glCreateBuffers(1, &m_id);
  glNamedBufferStorage(m_id, sizeof(ShapeVertex) * vertices.size(), vertices.data(), storage_flags);
}

VBO::VBO(const std::vector<TerrainVertex>& vertices) : m_count(vertices.size()) {
  glCreateBuffers(1, &m_id);
  glNamedBufferStorage(m_id, sizeof(TerrainVertex) * vertices.size(), vertices.data(), storage_flags);
}

void VBO::reload(const std::vector<Vertex>& vertices) {
  if (m_immutable) {
    glNamedBufferSubData(m_id, 0, sizeof(Vertex) * vertices.size(), vertices.data());
  } else {
    glNamedBufferData(m_id, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
  }
}

VBO::~VBO() {
  ENGINE_WARN("Deleting VBO {}", m_id);
  glDeleteBuffers(1, &m_id);
}

std::unique_ptr<VBO> VBO::Create(const std::vector<Vertex>& vertices) { return std::make_unique<VBO>(vertices); }

std::unique_ptr<VBO> VBO::CreateShape(const std::vector<ShapeVertex>& vertices) {
  return std::make_unique<VBO>(vertices);
}

std::unique_ptr<VBO> VBO::CreateTerrain(const std::vector<TerrainVertex>& vertices) {
  return std::make_unique<VBO>(vertices);
}

IBO::IBO(const std::vector<uint32_t>& indices) : m_count(indices.size()), m_immutable(true) {
  if (m_immutable) {
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, sizeof(uint32_t) * indices.size(), indices.data(), storage_flags);
  } else {
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, sizeof(uint32_t) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
  }
}

void IBO::reload(const std::vector<uint32_t>& indices) {
  if (m_immutable) {
    glNamedBufferSubData(m_id, 0, sizeof(uint32_t) * indices.size(), indices.data());
  } else {
    glNamedBufferData(m_id, sizeof(uint32_t) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
  }
  m_count = indices.size();
}

IBO::~IBO() {
  ENGINE_WARN("Deleting IBO {}", m_id);
  glDeleteBuffers(1, &m_id);
}

std::unique_ptr<IBO> IBO::Create(const std::vector<uint32_t>& indices) { return std::make_unique<IBO>(indices); }
}