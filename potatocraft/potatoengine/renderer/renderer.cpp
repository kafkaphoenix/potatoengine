#include "potatoengine/renderer/renderer.h"

#include "potatoengine/renderer/rendererAPI.h"

namespace potatoengine {

void Renderer::Init() {
    RendererAPI::Init();
}

void Renderer::Shutdown() {
    // TODO something to add?
}

void Renderer::OnWindowResize(uint32_t w, uint32_t h) {
    RendererAPI::SetViewport(0, 0, w, h);
}

void Renderer::BeginScene(const Camera& c) {
    s_sceneData->viewProjectionMatrix = c.getViewProjection();
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::shared_ptr<Program>& sp, const std::shared_ptr<VAO>& vao, const glm::mat4& t) {
    sp->use();
    sp->setMat4("viewProjection", s_sceneData->viewProjectionMatrix);
    sp->setMat4("transform", t);

    RendererAPI::DrawIndexed(vao);
}

void Renderer::Link(const std::shared_ptr<Program>& sp, const Shader& vs, const Shader& fs) {
    sp->attach(vs);
    sp->attach(fs);
    sp->link();
    sp->detach(vs);
    sp->detach(fs);
}
}