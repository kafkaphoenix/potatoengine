#pragma once

#define GLM_FORCE_CTOR_INIT

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CInput {
    enum class Mode { _None, _3D, _2D };

    std::string _mode;
    Mode mode;
    float mouseSensitivity = 0.1f;
    float translationSpeed = 20.f;
    float verticalSpeed = 45.f;
    float rotationSpeed = 180.f;

    CInput() = default;
    explicit CInput(std::string&& m, float ms, float ts, float vs, float rs)
      : _mode(std::move(m)), mouseSensitivity(ms), translationSpeed(ts),
        verticalSpeed(vs), rotationSpeed(rs) {}

    void print() const {
      ENGINE_BACKTRACE(
        "\t\tmode: {0}\n\t\t\t\tmouseSensitivity: "
        "{1}\n\t\t\t\ttranslationSpeed: {2}\n\t\t\t\tverticalSpeed: "
        "{3}\n\t\t\t\trotationSpeed: {4}",
        _mode, mouseSensitivity, translationSpeed, verticalSpeed,
        rotationSpeed);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["mode"] = _mode;
      info["mouseSensitivity"] = std::to_string(mouseSensitivity);
      info["translationSpeed"] = std::to_string(translationSpeed);
      info["verticalSpeed"] = std::to_string(verticalSpeed);
      info["rotationSpeed"] = std::to_string(rotationSpeed);

      return info;
    }

    void setMode() {
      if (_mode == "3D") {
        mode = Mode::_3D;
      } else if (_mode == "2D") {
        mode = Mode::_2D;
      } else if (_mode == "none") {
        mode = Mode::_None;
      } else {
        ENGINE_ASSERT(false, "Invalid input mode!");
      }
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e, CInput& c) {
  c.setMode();

  m_registry.replace<CInput>(e, c);
}