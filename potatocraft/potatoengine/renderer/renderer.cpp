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
    m_viewMatrix = c.getView();
    m_projectionMatrix = c.getProjection();
    m_cameraPosition = c.getPosition();
}

void Renderer::endScene() noexcept {
}

void Renderer::add(std::string&& name) {
    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("AssetsManager is null!");
    }
    
    auto newShaderProgram = ShaderProgram::Create(std::string(name));
    const auto& vs = manager->get<Shader>("v" + name);
    const auto& fs = manager->get<Shader>("f" + name);
    newShaderProgram->attach(*vs);
    newShaderProgram->attach(*fs);
    newShaderProgram->link();
    newShaderProgram->detach(*vs);
    newShaderProgram->detach(*fs);
#ifdef DEBUG
    CORE_INFO("Shader {} linked!", name);
#endif
    m_shaderPrograms.emplace(std::move(name), std::move(newShaderProgram));
}

void Renderer::render(const std::shared_ptr<VAO>& vao, const glm::mat4& transform, std::string_view shaderProgram) {
    auto& sp = get(shaderProgram);

    sp->use();
    sp->setMat4("projection", m_projectionMatrix);
    sp->setMat4("view", m_viewMatrix);
    sp->setMat4("model", transform);

    RendererAPI::DrawIndexed(vao);
    sp->unuse(); // DONT unuse before the draw call
}

std::unique_ptr<Renderer> Renderer::Create(std::weak_ptr<AssetsManager> am) noexcept {
    return std::make_unique<Renderer>(am);
}

std::unique_ptr<ShaderProgram>& Renderer::get(std::string_view name) {
    if (m_shaderPrograms.contains(name.data())) {
        return m_shaderPrograms.at(name.data());
    } else {
        throw std::runtime_error("Shader program not found");
    }
}
}