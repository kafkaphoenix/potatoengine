#pragma once

#include "src/pch.h"
#include "src/renderer/shader/program.h"
#include "src/renderer/camera/camera.h"
#include "src/renderer/rendererAPI.h"

namespace potatoengine {

    class Renderer
    {
        public:
            static void Init();
            static void Shutdown();

            static void OnWindowResize(uint32_t width, uint32_t height);

            static void BeginScene(const Camera& camera);
            static void EndScene();

            static void Submit(const std::shared_ptr<Program>& shader, const std::shared_ptr<VAO>& vao, const glm::mat4& transform = glm::mat4(1.f));

        private:
            struct SceneData
            {
                glm::mat4 viewProjectionMatrix;
            };

            inline static std::unique_ptr<SceneData> s_sceneData = std::make_unique<Renderer::SceneData>();
    };
}