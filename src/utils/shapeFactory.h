#pragma once

#include "renderer/vao.h"

namespace potatoengine {

class ShapeFactory {
  public:
    static std::shared_ptr<VAO> CreateTriangle(float size);
    static std::shared_ptr<VAO> CreateRectangle(float width, float height, bool repeatTexture);
    static std::shared_ptr<VAO> CreateCube(float width, float height, float depth, bool repeatTexture);
    static std::shared_ptr<VAO> CreateCircle(float radius, uint32_t segments);

  private:
    static std::shared_ptr<VAO> CreateShape(const std::vector<ShapeVertex>& vertices,
                                            const std::vector<uint32_t>& indices);
};
}