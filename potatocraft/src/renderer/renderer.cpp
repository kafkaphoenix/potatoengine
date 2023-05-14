#include "src/renderer/renderer.h"
#include "src/renderer/rendererAPI.h"

namespace potatoengine {

    void Renderer::Init() {
        RendererAPI::Init();
    }

    void Renderer::Shutdown() {

    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
        RendererAPI::SetViewport(0.f, 0.f, width, height);
    }

    void Renderer::BeginScene(const Camera& camera) {
        s_sceneData->viewProjectionMatrix = camera.getViewProjection();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const std::shared_ptr<Program>& shader, const std::shared_ptr<VAO>& vao, const glm::mat4& transform) {
        shader->use();
        shader->setMat4("viewProjection", s_sceneData->viewProjectionMatrix);
        shader->setMat4("transform", transform);

        RendererAPI::DrawIndexed(vao);
    }
}