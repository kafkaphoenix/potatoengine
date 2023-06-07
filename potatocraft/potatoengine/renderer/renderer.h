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

    static void OnWindowResize(uint32_t w, uint32_t h);

    static void BeginScene(const Camera& c);
    static void EndScene();

    static void Submit(const std::shared_ptr<Program>& sp, const std::shared_ptr<VAO>& vao, const glm::mat4& t = glm::mat4(1.f));
    static void Link(const std::shared_ptr<Program>& sp, const Shader &vs, const Shader &fs);

   private:
    struct SceneData {
        glm::mat4 viewProjectionMatrix;
    };

    inline static std::unique_ptr<SceneData> s_sceneData = std::make_unique<SceneData>();
};
}