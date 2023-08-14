#include "potatoengine/renderer/renderer.h"

#include "potatoengine/assets/texture.h"
#include "potatoengine/renderer/rendererAPI.h"

namespace potatoengine {

Renderer::Renderer(std::weak_ptr<AssetsManager> am) : m_assetsManager(am) {}

void Renderer::init() const noexcept {
    RendererAPI::Init();
}

void Renderer::shutdown() noexcept {
    // TODO something to add? reset statistics maybe?
}

void Renderer::onWindowResize(uint32_t w, uint32_t h) const noexcept {
    RendererAPI::SetViewport(0, 0, w, h);
}

void Renderer::beginScene(const Camera& c) noexcept {
    m_viewProjectionMatrix = c.getViewProjection();
}

void Renderer::endScene() noexcept {
}

void Renderer::addShaderProgram(const std::string& name) {
#ifdef DEBUG
    CORE_INFO("\tComputing shader program...");
#endif
    const auto& manager = m_assetsManager.lock();
    if (!manager) {
        throw std::runtime_error("AssetsManager is null!");
    }
    auto newShaderProgram = ShaderProgram::Create(name);
    const auto& vs = manager->get<Shader>("v" + name);
    const auto& fs = manager->get<Shader>("f" + name);
    newShaderProgram->attach(*vs);
    newShaderProgram->attach(*fs);
    newShaderProgram->link();
    newShaderProgram->detach(*vs);
    newShaderProgram->detach(*fs);

    newShaderProgram->use();
    newShaderProgram->setVec3("light_dir", {0.5f, 0.5f, 1.f});
    newShaderProgram->unuse();
    m_shaderPrograms.emplace(name, std::move(newShaderProgram));
#ifdef DEBUG
    CORE_INFO("\tShaders computed!");
#endif
}

void Renderer::render(const std::shared_ptr<VAO>& vao, const glm::mat4& transform, const std::string& shaderProgram) const noexcept {
    auto& sp = m_shaderPrograms.at(shaderProgram);

    sp->use();
    sp->setMat4("viewProjection", m_viewProjectionMatrix);
    sp->setMat4("transform", transform);

    RendererAPI::DrawIndexed(vao);
    sp->unuse();
}

std::unique_ptr<Renderer> Renderer::Create(std::weak_ptr<AssetsManager> am) noexcept {
    return std::make_unique<Renderer>(am);
}

}