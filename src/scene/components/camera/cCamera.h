#pragma once

#define GLM_FORCE_CTOR_INIT

#include "utils/numericComparator.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace potatoengine {

struct CCamera {
    enum class CameraType { Perspective, Orthographic };

    enum class AspectRatio {
      _16_9,
      _4_3,
    };

    enum class Mode { _None, _3D, _2D };

    std::string _type = "perspective";
    CameraType type = CameraType::Perspective;
    std::string _aspectRatio = "16:9";
    AspectRatio aspectRatio = AspectRatio::_16_9;
    std::string _mode;
    Mode mode;
    float aspectRatioValue = 16.f / 9.f;
    float fov = 90.f;
    float zoomFactor = 1.f;
    float zoomMin = 1.f;
    float zoomMax = 4.f;
    float nearClip = 0.1f;
    float farClip = 1000.f;
    glm::mat4 view{};
    glm::mat4 projection{};
    float rightAngle{};
    float upAngle{};

    CCamera() = default;
    explicit CCamera(std::string&& t, std::string&& ar, std::string&& m,
                     float f, float zf, float zm, float nc, float fc)
      : _type(std::move(t)), _aspectRatio(std::move(ar)), _mode(std::move(m)),
        fov(f), zoomFactor(zf), zoomMin(zm), zoomMax(zm), nearClip(nc),
        farClip(fc) {}

    void print() const {
      ENGINE_BACKTRACE(
        "\t\ttype: {0}\n\t\t\t\taspectRatio: {1}\n\t\t\t\tmode: "
        "{2}\n\t\t\t\tfov: {3}\n\t\t\t\tzoomFactor: {4}\n\t\t\t\tzoomMin: "
        "{5}\n\t\t\t\tzoomMax: {6}\n\t\t\t\tnearClip: {7}\n\t\t\t\tfarClip: "
        "{8}",
        _type, _aspectRatio, _mode, fov, zoomFactor, zoomMin, zoomMax, nearClip,
        farClip);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["type"] = _type;
      info["aspectRatio"] = _aspectRatio;
      info["mode"] = _mode;
      info["fov"] = std::to_string(fov);
      info["zoomFactor"] = std::to_string(zoomFactor);
      info["zoomMin"] = std::to_string(zoomMin);
      info["zoomMax"] = std::to_string(zoomMax);
      info["nearClip"] = std::to_string(nearClip);
      info["farClip"] = std::to_string(farClip);

      return info;
    }

    void setCameraType() {
      if (_type == "perspective") {
        type = CameraType::Perspective;
      } else if (_type == "orthographic") {
        type = CameraType::Orthographic;
      }
    }

    void setAspectRatio() {
      if (_aspectRatio == "16:9") {
        aspectRatio = AspectRatio::_16_9;
        aspectRatioValue = 16.f / 9.f;
      } else if (_aspectRatio == "4:3") {
        aspectRatio = AspectRatio::_4_3;
        aspectRatioValue = 4.f / 3.f;
      }
    }

    void setMode() {
      if (_mode == "3D") {
        mode = Mode::_3D;
      } else if (_mode == "2D") {
        mode = Mode::_2D;
      } else if (_mode == "none") {
        mode = Mode::_None;
      } else {
        ENGINE_ASSERT(false, "Invalid camera mode!");
      }
    }

    void calculateProjection() {
      if (type == CameraType::Perspective) {
        zoomFactor = std::clamp(zoomFactor, zoomMin, zoomMax);
        projection = glm::perspective(glm::radians(fov * 1.f / zoomFactor),
                                      aspectRatioValue, nearClip, farClip);
      } else if (type == CameraType::Orthographic) {
        projection = glm::ortho(-aspectRatioValue * zoomFactor,
                                aspectRatioValue * zoomFactor, -zoomFactor,
                                zoomFactor, nearClip, farClip);
      }
    }

    void calculateView(const glm::vec3& position, const glm::quat& rotation) {
      glm::mat4 rotated = glm::mat4_cast(glm::conjugate(rotation));
      glm::mat4 translated = glm::translate(rotated, -position);
      view = std::move(translated);
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e, CCamera& c) {
  c.setCameraType();
  c.setAspectRatio();
  c.setMode();
  c.calculateProjection();

  m_registry.replace<CCamera>(e, c);
}