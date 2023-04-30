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
        s_sceneData->viewProjectionMatrix = camera.getView();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const Ref<Program>& shader, const Ref<VAO>& vao, const glm::mat4& transform) {
        /*shader.use();// todo shader is not const we modify it maybe
        shader.setMat4("view", s_sceneData->viewProjectionMatrix);

        const GLfloat render_x = 3840.f, render_y = 2160.f; // TODO: REMOVE?
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), render_x / render_y, 0.01f, 3000.0f);
        s_sceneData->render.setMat4("projection", projection);*/

        RendererAPI::DrawIndexed(vao);
    }
}