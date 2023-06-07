#pragma once

#include "potatoengine/renderer/buffer.h"

namespace potatoengine {

class VAO {
   public:
    VAO();
    ~VAO();

    void bind() const;

    void attachVertex(std::unique_ptr<VBO> vbo);
    void setIndex(std::unique_ptr<IBO> ibo);

    const std::vector<std::unique_ptr<VBO>>& getVBOs() const { return m_vbos; }
    const std::unique_ptr<IBO>& getEBO() const { return m_ibo; }

    static std::shared_ptr<VAO> Create();

   private:
    uint32_t m_id;
    std::vector<std::unique_ptr<VBO>> m_vbos;
    std::unique_ptr<IBO> m_ibo;
    uint32_t m_vboIdx = 0;
};

}