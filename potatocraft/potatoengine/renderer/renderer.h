#pragma once

#include "potatoengine/pch.h"
#include "potatoengine/renderer/camera/camera.h"
#include "potatoengine/renderer/rendererAPI.h"
#include "potatoengine/renderer/shader/program.h"

namespace potatoengine {

class Renderer {
   public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(const Camera& camera);
    static void EndScene();

    static void Submit(const std::shared_ptr<Program>& shader, const std::shared_ptr<VAO>& vao, const glm::mat4& transform = glm::mat4(1.f));

   private:
    struct SceneData {
        glm::mat4 viewProjectionMatrix;
    };

    inline static std::unique_ptr<SceneData> s_sceneData = std::make_unique<SceneData>();
};
}