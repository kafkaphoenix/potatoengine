#pragma once

#include "potatoengine/renderer/buffer.h"

namespace potatoengine {

class VAO {
   public:
    VAO();
    ~VAO();

    void bind() const;

    void attachVertex(const std::shared_ptr<VBO>& vbo);
    void setIndex(const std::shared_ptr<IBO>& ibo);

    const std::vector<std::shared_ptr<VBO>>& getVBOs() const { return m_vbos; }
    const std::shared_ptr<IBO>& getEBO() const { return m_ibo; }

    static std::shared_ptr<VAO> Create();

   private:
    uint32_t m_id;
    std::vector<std::shared_ptr<VBO>> m_vbos;
    std::shared_ptr<IBO> m_ibo;
    uint32_t m_vboIndex = 0;
};

}