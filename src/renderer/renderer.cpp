#include "renderer/renderer.h"

#include "assets/texture.h"
#include "renderer/rendererAPI.h"

namespace potatoengine {

Renderer::Renderer(std::weak_ptr<AssetsManager> am) : m_assetsManager(am) {}

void Renderer::init() const { RendererAPI::Init(); }

void Renderer::shutdown() {
  ENGINE_WARN("Shutting down Renderer");
} // TODO: shutdown renderer

void Renderer::onWindowResize(uint32_t w, uint32_t h) const {
  RendererAPI::SetViewport(0, 0, w, h);
}

void Renderer::beginScene(const CCamera& c, const CTransform& t) {
  m_view = c.view;
  m_projection = c.projection;
  m_cameraPosition = t.position;
}

void Renderer::endScene() {
  // TODO renderer metrics
}

void Renderer::addShader(std::string&& name) {
  const auto& manager = m_assetsManager.lock();
  ENGINE_ASSERT(manager, "AssetsManager is null!");

  auto newShaderProgram = ShaderProgram::Create(std::string(name));
  const auto& vs = manager->get<Shader>("v" + name);
  const auto& fs = manager->get<Shader>("f" + name);
  newShaderProgram->attach(*vs);
  newShaderProgram->attach(*fs);
  newShaderProgram->link();
  newShaderProgram->detach(*vs);
  newShaderProgram->detach(*fs);
  ENGINE_TRACE("Shader {} linked!", name);
  m_shaderPrograms.emplace(std::move(name), std::move(newShaderProgram));
}

void Renderer::addFramebuffer(std::string&& name, uint32_t w, uint32_t h,
                              uint32_t t) {
  m_framebuffers.emplace(std::move(name), FBO::Create(w, h, t));
}

void Renderer::renderFBO(const std::shared_ptr<VAO>& vao,
                         std::string_view fbo) {
  auto& sp = get("fbo");

  sp->use();
  sp->setInt("screenTexture", 100);
  getFramebuffers().at(fbo.data())->getColorTexture()->bindSlot(100);

  RendererAPI::DrawIndexed(vao);
  sp->unuse();
}

void Renderer::render(const std::shared_ptr<VAO>& vao,
                      const glm::mat4& transform,
                      std::string_view shaderProgram) {
  auto& sp = get(shaderProgram);

  sp->use();
  sp->setMat4("projection", m_projection);
  sp->setMat4("view", m_view);
  sp->setVec3("cameraPosition", m_cameraPosition);
  sp->setMat4("model", transform);

  RendererAPI::DrawIndexed(vao);
  sp->unuse(); // DONT unuse before the draw call
}

void Renderer::clear() {
  if (not m_framebuffers.empty()) {
    m_framebuffers.clear();
    RendererAPI::ToggleDepthTest(
      true); // to avoid problems after using scenes with fbo
  }
  m_shaderPrograms.clear();
}

std::unique_ptr<Renderer> Renderer::Create(std::weak_ptr<AssetsManager> am) {
  return std::make_unique<Renderer>(am);
}

std::unique_ptr<ShaderProgram>& Renderer::get(std::string_view name) {
  ENGINE_ASSERT(m_shaderPrograms.contains(name.data()),
                "Shader program {} not found!", name);
  return m_shaderPrograms.at(name.data());
}
}