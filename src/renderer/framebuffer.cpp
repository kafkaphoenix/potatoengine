#include "renderer/framebuffer.h"

#include <glad/glad.h>

#include "core/application.h"
#include "utils/mapJsonSerializer.h"

namespace potatoengine {

FBO::FBO(uint32_t w, uint32_t h, uint32_t t) : m_depthBufferType(t) {
  const auto& settings = Application::Get().getSettings();
  int windowWidth;
  int windowHeight;
  if (settings->fullscreen) {
    int monitorCount;
    GLFWmonitor* monitor =
      (glfwGetMonitors(&monitorCount))[settings->primaryMonitor];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    windowWidth = mode->width;
    windowHeight = mode->height;
  } else {
    windowWidth = settings->windowWidth;
    windowHeight = settings->windowHeight;
  }
  m_width = w == 0 ? windowWidth : w;
  m_height = h == 0 ? windowHeight : h;
  glCreateFramebuffers(1, &m_id);
  glNamedFramebufferDrawBuffer(m_id, GL_COLOR_ATTACHMENT0);
  attachTexture();
  if (m_depthBufferType == DEPTH_TEXTURE) {
    attachDepthTexture();
  } else if (m_depthBufferType == DEPTH_RENDERBUFFER) {
    attachDepthRenderBuffer();
  } else if (m_depthBufferType == STENCIL_RENDERBUFFER) {
    attachStencilRenderBuffer();
  }
  uint32_t status = glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER);
  ENGINE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer error: {}",
                status);
}

FBO::~FBO() {
  ENGINE_WARN("Deleting framebuffer {}", m_id);
  glDeleteFramebuffers(1, &m_id);
  glDeleteRenderbuffers(1, &m_depthRenderBuffer);
  glDeleteRenderbuffers(1, &m_stencilRenderBuffer);
}

void FBO::attachTexture() {
  m_colorTexture = assets::Texture::Create(m_width, m_height, GL_RGBA8, assets::Texture::WRAP);
  glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, m_colorTexture->getID(),
                            0);
}

void FBO::attachDepthTexture() {
  // slower than renderbuffer but can be sampled in shaders
  m_depthTexture = assets::Texture::Create(m_width, m_height, GL_DEPTH_COMPONENT24,
                                   assets::Texture::DONT_WRAP);
  glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, m_depthTexture->getID(),
                            0);
}

void FBO::attachDepthRenderBuffer() {
  glCreateRenderbuffers(1, &m_depthRenderBuffer);
  glNamedRenderbufferStorage(m_depthRenderBuffer, GL_DEPTH_COMPONENT24, m_width,
                             m_height);
  glNamedFramebufferRenderbuffer(m_id, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                 m_depthRenderBuffer);
}

void FBO::attachStencilRenderBuffer() {
  glCreateRenderbuffers(1, &m_stencilRenderBuffer);
  glNamedRenderbufferStorage(m_stencilRenderBuffer, GL_STENCIL_INDEX8, m_width,
                             m_height);
  glNamedFramebufferRenderbuffer(m_id, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                 m_stencilRenderBuffer);
}

void FBO::attachDepthStencilRenderBuffer() {
  glCreateRenderbuffers(1, &m_depthStencilRenderBuffer);
  glNamedRenderbufferStorage(m_depthStencilRenderBuffer, GL_DEPTH24_STENCIL8,
                             m_width, m_height);
  glNamedFramebufferRenderbuffer(m_id, GL_DEPTH_STENCIL_ATTACHMENT,
                                 GL_RENDERBUFFER, m_depthStencilRenderBuffer);
}

uint32_t FBO::getBufferID() const {
  if (m_depthBufferType == DEPTH_TEXTURE) {
    return m_depthTexture->getID();
  } else if (m_depthBufferType == DEPTH_RENDERBUFFER) {
    return m_depthRenderBuffer;
  } else if (m_depthBufferType == STENCIL_RENDERBUFFER) {
    return m_stencilRenderBuffer;
  } else if (m_depthBufferType == DEPTH_STENCIL_RENDERBUFFER) {
    return m_depthStencilRenderBuffer;
  } else {
    ENGINE_ERROR("No buffer attached to framebuffer {}", m_id);
    return 0;
  }
}

std::string_view FBO::getBufferType() const noexcept {
  if (m_depthBufferType == DEPTH_TEXTURE) {
    return "Depth Texture";
  } else if (m_depthBufferType == DEPTH_RENDERBUFFER) {
    return "Depth Renderbuffer";
  } else if (m_depthBufferType == STENCIL_RENDERBUFFER) {
    return "Stencil Renderbuffer";
  } else if (m_depthBufferType == DEPTH_STENCIL_RENDERBUFFER) {
    return "Depth Stencil Renderbuffer";
  } else {
    ENGINE_ERROR("No buffer attached to framebuffer {}", m_id);
    return "None";
  }
}

const std::map<std::string, std::string, NumericComparator>& FBO::getInfo() {
  if (not m_info.empty()) {
    return m_info;
  }

  m_info["Type"] = "FBO";
  m_info["ID"] = std::to_string(m_id);
  m_info["Width"] = std::to_string(m_width);
  m_info["Height"] = std::to_string(m_height);
  m_info["Buffer ID"] = std::to_string(getBufferID());
  m_info["Buffer type"] = getBufferType();
  if (m_colorTexture) {
    m_info["Color texture"] = MapToJson(m_colorTexture->getInfo());
  }
  if (m_depthTexture) {
    m_info["Depth texture"] = MapToJson(m_depthTexture->getInfo());
  }

  return m_info;
}

void FBO::bindToDraw() {
  const auto& renderer = Application::Get().getRenderer();
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
  renderer->onWindowResize(m_width, m_height);
}

void FBO::bindToRead() {
  m_colorTexture->bindSlot(100);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
  glNamedFramebufferReadBuffer(m_id, GL_COLOR_ATTACHMENT0);
}

void FBO::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // default framebuffer
  auto& app = Application::Get();
  const auto& settings = app.getSettings();
  const auto& renderer = app.getRenderer();
  if (settings->fullscreen) {
    int monitorCount;
    GLFWmonitor* monitor =
      (glfwGetMonitors(&monitorCount))[settings->primaryMonitor];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    renderer->onWindowResize(mode->width, mode->height);
  } else {
    renderer->onWindowResize(settings->windowWidth, settings->windowHeight);
  }
}

void FBO::clear(const float color[4], const float depth) {
  glClearNamedFramebufferfv(m_id, GL_COLOR, 0, color);
  glClearNamedFramebufferfv(m_id, GL_DEPTH, 0, &depth);
}

void FBO::resize(uint32_t width, uint32_t height) {
  m_width = width;
  m_height = height;
  bindToDraw();
}

std::unique_ptr<FBO> FBO::Create(uint32_t width, uint32_t height,
                                 uint32_t bufferType) {
  return std::make_unique<FBO>(width, height, bufferType);
}
}