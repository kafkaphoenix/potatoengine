#include "render/renderManager.h"

#include "assets/texture.h"
#include "render/renderAPI.h"
#include "imgui/imscene.h"

namespace potatoengine {

void RenderManager::init() const { RenderAPI::Init(); }

void RenderManager::shutdown() {
  ENGINE_WARN("Shutting down render manager");
}

void RenderManager::onWindowResize(uint32_t w, uint32_t h) const {
  RenderAPI::SetViewport(0, 0, w, h);
}

void RenderManager::beginScene(glm::mat4 view, glm::mat4 projection,
                          glm::vec3 cameraPosition) {
  m_view = view;
  m_projection = projection;
  m_cameraPosition = cameraPosition;
}

void RenderManager::endScene() {}

void RenderManager::addShaderProgram(
  std::string&& name,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  auto newShaderProgram = ShaderProgram::Create(std::string(name));
  const auto& vs = assets_manager->get<assets::Shader>("v" + name);
  const auto& fs = assets_manager->get<assets::Shader>("f" + name);
  newShaderProgram->attach(*vs);
  newShaderProgram->attach(*fs);
  newShaderProgram->link();
  newShaderProgram->detach(*vs);
  newShaderProgram->detach(*fs);
  ENGINE_TRACE("Shader {} linked!", name);
  m_shaderPrograms.emplace(std::move(name), std::move(newShaderProgram));
}

void RenderManager::addFramebuffer(std::string&& name, uint32_t w, uint32_t h,
                              uint32_t t) {
  m_framebuffers.emplace(std::move(name), FBO::Create(w, h, t));
}

void RenderManager::deleteFramebuffer(std::string_view name) {
  m_framebuffers.erase(name.data());
}

void RenderManager::renderFBO(const std::shared_ptr<VAO>& vao,
                         std::string_view fbo) {
  auto& sp = getShaderProgram("fbo");

  sp->use();
  sp->setInt("screenTexture", 100);
  m_framebuffers.at(fbo.data())->getColorTexture()->bindSlot(100);

  RenderAPI::DrawIndexed(vao);

  m_drawCalls++;
  m_triangles += vao->getEBO()->getCount() / 3;
  for (const auto& vbo : vao->getVBOs()) {
    m_vertices += vbo->getCount();
  }
  m_indices += vao->getEBO()->getCount();
  sp->unuse();
}

void RenderManager::renderInsideImGui(const std::shared_ptr<VAO>& vao,
                                 std::string_view fbo, std::string_view title,
                                 glm::vec2 size, glm::vec2 position,
                                 bool fitToWindow) {
  auto& fbo_ = m_framebuffers.at(fbo.data());
  renderScene(fbo_->getColorTexture()->getID(), title, size, position,
                  fitToWindow);

  m_drawCalls++;
  m_triangles += vao->getEBO()->getCount() / 3;
  for (const auto& vbo : vao->getVBOs()) {
    m_vertices += vbo->getCount();
  }
  m_indices += vao->getEBO()->getCount();
}

void RenderManager::render(const std::shared_ptr<VAO>& vao,
                      const glm::mat4& transform,
                      std::string_view shaderProgram) {
  auto& sp = getShaderProgram(shaderProgram);

  sp->use();
  sp->setMat4("projection", m_projection);
  sp->setMat4("view", m_view);
  sp->setVec3("cameraPosition", m_cameraPosition);
  sp->setMat4("model", transform);

  RenderAPI::DrawIndexed(vao);

  m_drawCalls++;
  m_triangles += vao->getEBO()->getCount() / 3;
  for (const auto& vbo : vao->getVBOs()) {
    m_vertices += vbo->getCount();
  }
  m_indices += vao->getEBO()->getCount();
  sp->unuse(); // DONT unuse before the draw call
}

void RenderManager::clear() {
  if (not m_framebuffers.empty()) {
    m_framebuffers.clear();
    // to avoid problems after using scenes with fbo
    RenderAPI::ToggleDepthTest(true);
  }
  m_shaderPrograms.clear();
}

std::unique_ptr<RenderManager> RenderManager::Create() {
  return std::make_unique<RenderManager>();
}

const std::unique_ptr<ShaderProgram>&
RenderManager::getShaderProgram(std::string_view name) {
  ENGINE_ASSERT(m_shaderPrograms.contains(name.data()),
                "Shader program {} not found!", name);
  return m_shaderPrograms.at(name.data());
}

const std::map<std::string, std::string, NumericComparator>&
RenderManager::getMetrics() {
  m_metrics["Framebuffers"] = std::to_string(m_framebuffers.size());
  m_metrics["Shader programs"] = std::to_string(m_shaderPrograms.size());
  m_metrics["Draw calls"] = std::to_string(m_drawCalls);
  m_metrics["Triangles"] = std::to_string(m_triangles);
  m_metrics["Vertices"] = std::to_string(m_vertices);
  m_metrics["Indices"] = std::to_string(m_indices);

  return m_metrics;
}

void RenderManager::resetMetrics() {
  m_drawCalls = 0;
  m_triangles = 0;
  m_vertices = 0;
  m_indices = 0;
  m_metrics.clear();
}
}