#pragma once

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>

#include "assets/assetsManager.h"
#include "pch.h"
#include "renderer/framebuffer.h"
#include "renderer/rendererAPI.h"
#include "renderer/shaderProgram.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/physics/cTransform.h"
#include "utils/numericComparator.h"

namespace potatoengine {

class Renderer {
  public:
    Renderer(std::weak_ptr<AssetsManager> am);

    void init() const;
    void shutdown();

    void onWindowResize(uint32_t w, uint32_t h) const;

    void beginScene(const CCamera& c, const CTransform& t);
    void endScene();

    void addShader(std::string&& shaderProgram);
    void addFramebuffer(std::string&& framebuffer, uint32_t width,
                        uint32_t height, uint32_t bufferType);
    const std::unordered_map<std::string, std::unique_ptr<ShaderProgram>>&
    getShaderPrograms() const noexcept {
      return m_shaderPrograms;
    }
    const std::unordered_map<std::string, std::unique_ptr<FBO>>&
    getFramebuffers() const noexcept {
      return m_framebuffers;
    }
    bool contains(std::string_view shaderProgram) const {
      return m_shaderPrograms.contains(shaderProgram.data());
    }
    std::unique_ptr<ShaderProgram>& get(std::string_view shaderProgram);

    void render(const std::shared_ptr<VAO>& vao, const glm::mat4& transform,
                std::string_view shaderProgram);
    void renderFBO(const std::shared_ptr<VAO>& vao, std::string_view fbo);
    void renderInsideImGui(const std::shared_ptr<VAO>& vao,
                           std::string_view fbo, std::string_view title,
                           glm::vec2 size, glm::vec2 position);
    int getShaderProgramsCount() const { return m_shaderPrograms.size(); }
    int getFramebuffersCount() const { return m_framebuffers.size(); }
    void clear();
    void resetMetrics();
    const std::map<std::string, std::string, NumericComparator>& getMetrics();
    static std::shared_ptr<Renderer> Create(std::weak_ptr<AssetsManager> am);

  private:
    glm::mat4 m_view{};
    glm::mat4 m_projection{};
    glm::vec3 m_cameraPosition{};
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>>
      m_shaderPrograms;
    std::unordered_map<std::string, std::unique_ptr<FBO>> m_framebuffers;
    std::weak_ptr<AssetsManager> m_assetsManager;
    std::map<std::string, std::string, NumericComparator> m_metrics{};
    int m_drawCalls{};
    int m_triangles{};
    int m_vertices{};
    int m_indices{};
};
}