
#pragma once

#include <entt/entt.hpp>

#include "pch.h"
#include "render/shaderProgram.h"
#include "render/framebuffer.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CFBO {
    enum class Mode {
      Normal,
      Inverse,
      Greyscale,
      Blur,
      Edge,
      Sharpen,
      NightVision,
      Emboss
    };

    std::string fbo;
    std::string _mode;
    Mode mode;
    std::string _attachment;
    uint32_t attachment{};
    uint32_t width{};
    uint32_t height{};

    CFBO() = default;
    explicit CFBO(std::string&& fbo, Mode m, std::string&& attachment,
                  uint32_t w, uint32_t h)
      : fbo(std::move(fbo)), mode(m), _attachment(std::move(attachment)),
        width(w), height(h) {}

    void print() const {
      ENGINE_BACKTRACE("\t\tfbo: {0}\n\t\t\t\t\t\tmode: {1}\n\t\t\t\t\t\tattachment: "
                       "{2}\n\t\t\t\t\t\twidth: {3}\n\t\t\t\t\t\theight: {4}",
                       fbo, _mode, _attachment, width, height);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["fbo"] = fbo;
      info["mode"] = _mode;
      info["attachment"] = _attachment;
      info["width"] = std::to_string(width);
      info["height"] = std::to_string(height);

      return info;
    }

    void setAttachment() {
      if (_attachment == "depth_texture") {
        attachment = FBO::DEPTH_TEXTURE;
      } else if (_attachment == "depth_renderbuffer") {
        attachment = FBO::DEPTH_RENDERBUFFER;
      } else if (_attachment == "stencil_renderbuffer") {
        attachment = FBO::STENCIL_RENDERBUFFER;
      } else if (_attachment == "depth_stencil_renderbuffer") {
        attachment = FBO::DEPTH_STENCIL_RENDERBUFFER;
      } else {
        ENGINE_ASSERT(false, "Unknown fbo attachment: {}", _attachment);
      }
    }

    void setMode() {
      if (_mode == "normal") {
        mode = Mode::Normal;
      } else if (_mode == "inverse") {
        mode = Mode::Inverse;
      } else if (_mode == "greyscale") {
        mode = Mode::Greyscale;
      } else if (_mode == "blur") {
        mode = Mode::Blur;
      } else if (_mode == "edge") {
        mode = Mode::Edge;
      } else if (_mode == "sharpen") {
        mode = Mode::Sharpen;
      } else if (_mode == "night_vision") {
        mode = Mode::NightVision;
      } else if (_mode == "emboss") {
        mode = Mode::Emboss;
      } else {
        ENGINE_ASSERT(false, "Unknown fbo mode {}", _mode);
      }
    }

    void setupProperties(const std::unique_ptr<ShaderProgram>& sp) {
      sp->resetActiveUniforms();
      sp->use();
      if (mode == Mode::Normal) {
        sp->setFloat("mode", 0.f);
      } else if (mode == Mode::Inverse) {
        sp->setFloat("mode", 1.f);
      } else if (mode == Mode::Greyscale) {
        sp->setFloat("mode", 2.f);
      } else if (mode == Mode::Blur) {
        sp->setFloat("mode", 3.f);
      } else if (mode == Mode::Edge) {
        sp->setFloat("mode", 4.f);
      } else if (mode == Mode::Sharpen) {
        sp->setFloat("mode", 5.f);
      } else if (mode == Mode::NightVision) {
        sp->setFloat("mode", 6.f);
      } else if (mode == Mode::Emboss) {
        sp->setFloat("mode", 7.f);
      }
      sp->unuse();
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e, CFBO& c) {
  c.setMode();
  c.setAttachment();

  m_registry.replace<CFBO>(e, c);
}