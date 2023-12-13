
#pragma once

#include "pch.h"
#include "scene/entity.h"

namespace potatoengine {

struct CFBO {
    enum class Mode { Normal, Inverse, Greyscale, Blur, Edge, Sharpen, NightVision, Emboss };

    std::string fbo{};
    std::string _mode{};
    Mode mode;
    std::string attachment{};
    uint32_t width{};
    uint32_t height{};

    CFBO() = default;
    explicit CFBO(std::string&& fbo, Mode m, std::string&& attachment, uint32_t w, uint32_t h)
        : fbo(std::move(fbo)), mode(m), attachment(std::move(attachment)), width(w), height(h) {}

    void print() const {
      ENGINE_TRACE("\t\tfbo: {0}\n\t\t\t\tmode: {1}\n\t\t\t\tattachment: {2}\n\t\t\t\twidth: {3}\n\t\t\t\theight: {4}",
                   fbo, _mode, attachment, width, height);
    }

    void setupProperties(std::unique_ptr<ShaderProgram>& sp) {
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

template <> void engine::SceneManager::onComponentAdded(Entity& e, CFBO& c) {
  if (c._mode == "normal") {
    c.mode = CFBO::Mode::Normal;
  } else if (c._mode == "inverse") {
    c.mode = CFBO::Mode::Inverse;
  } else if (c._mode == "greyscale") {
    c.mode = CFBO::Mode::Greyscale;
  } else if (c._mode == "blur") {
    c.mode = CFBO::Mode::Blur;
  } else if (c._mode == "edge") {
    c.mode = CFBO::Mode::Edge;
  } else if (c._mode == "sharpen") {
    c.mode = CFBO::Mode::Sharpen;
  } else if (c._mode == "night_vision") {
    c.mode = CFBO::Mode::NightVision;
  } else if (c._mode == "emboss") {
    c.mode = CFBO::Mode::Emboss;
  } else {
    ENGINE_ASSERT(false, "Unknown fbo mode {}", c._mode);
  }

  e.update<CFBO>(c);
}