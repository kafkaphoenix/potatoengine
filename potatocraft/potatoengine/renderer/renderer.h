#pragma once

#include "potatoengine/assets/assetsManager.h"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/camera/camera.h"
#include "potatoengine/renderer/rendererAPI.h"
#include "potatoengine/renderer/shaderProgram.h"

namespace potatoengine {

class Renderer {
   public:
    Renderer(std::weak_ptr<AssetsManager> am);

    void init() const noexcept;
    void shutdown() noexcept;

    void onWindowResize(uint32_t w, uint32_t h) const noexcept;

    void beginScene(const Camera& c) noexcept;
    void endScene() noexcept;

    void addShaderProgram(const std::string& name);
    void render(const std::shared_ptr<VAO>& vao, const glm::mat4& transform = glm::mat4(1.f), const std::string& shaderProgram = "basic") const noexcept;

    const std::unique_ptr<ShaderProgram>& getShaderProgram(const std::string& name) const noexcept { return m_shaderPrograms.at(name); }

    static std::unique_ptr<Renderer> Create(std::weak_ptr<AssetsManager> am) noexcept;

   private:
    glm::mat4 m_viewProjectionMatrix = glm::mat4(1.f);
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> m_shaderPrograms;
    std::weak_ptr<AssetsManager> m_assetsManager;
};
}