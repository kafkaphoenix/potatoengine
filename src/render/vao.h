#pragma once

#include "render/buffer.h"
#include "utils/numericComparator.h"

namespace potatoengine {

class VAO {
  public:
    VAO();
    ~VAO();

    void bind();
    void unbind();

    enum class VertexType { VERTEX, SHAPE_VERTEX, TERRAIN_VERTEX };
    void attachVertex(std::shared_ptr<VBO>&& vbo, VertexType type);
    void attachVertexAttributes();
    void attachShapeVertexAttributes();
    void attachTerrainVertexAttributes();
    void updateVertex(std::shared_ptr<VBO>&& vbo, uint32_t idx,
                      VertexType type);
    void clearVBOs();
    void setIndex(std::unique_ptr<IBO>&& ibo);

    const std::vector<std::shared_ptr<VBO>>& getVBOs() const noexcept {
      return m_vbos;
    }
    const std::unique_ptr<IBO>& getEBO() const noexcept { return m_ibo; }
    uint32_t getVBOIndex() const noexcept { return m_vboIDX; }
    uint32_t getID() const noexcept { return m_id; }
    bool isBinded() const noexcept { return m_binded; }
    const std::map<std::string, std::string, NumericComparator>& getInfo();

    static std::shared_ptr<VAO> Create();

  private:
    uint32_t m_id{};
    uint32_t m_vboIDX{};
    std::vector<std::shared_ptr<VBO>> m_vbos;
    std::unique_ptr<IBO> m_ibo;
    std::map<std::string, std::string, NumericComparator> m_info;
    bool m_dirty{};
    bool m_binded{};
};

}