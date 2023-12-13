#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace potatoengine {

struct CCamera {
    enum class CameraType { Perspective, Orthographic };

    enum class AspectRatio {
      _16_9,
      _4_3,
    };

    std::string _type = "perspective";
    CameraType type = CameraType::Perspective;
    std::string _aspectRatio = "16:9";
    AspectRatio aspectRatio = AspectRatio::_16_9;
    float aspectRatioValue = 16.f / 9.f;
    float fov = 90.f;
    float zoomFactor = 1.f;
    float zoomMin = 1.f;
    float zoomMax = 4.f;
    float nearClip = 0.1f;
    float farClip = 1000.f;
    float mouseSensitivity = 0.1f;
    float translationSpeed = 20.f;
    float verticalSpeed = 45.f;
    float rotationSpeed = 180.f;
    glm::mat4 view{};
    glm::mat4 projection{};
    float rightAngle{};
    float upAngle{};

    CCamera() = default;
    explicit CCamera(std::string&& t, float f, float zf, float zm, float zM, float ms, float nc, float fc, float ts,
                     float vs, float rs)
        : _type(std::move(t)), fov(f), zoomFactor(zf), zoomMin(zm), zoomMax(zM), mouseSensitivity(ms), nearClip(nc),
          farClip(fc), translationSpeed(ts), verticalSpeed(vs), rotationSpeed(rs) {}

    void print() const {
      ENGINE_TRACE(
          "\t\ttype: {0}\n\t\t\t\taspectRatio: {1}\n\t\t\t\tfov: {2}\n\t\t\t\tzoomFactor: {3}\n\t\t\t\tzoomMin: "
          "{4}\n\t\t\t\tzoomMax: {5}\n\t\t\t\tnearClip: {6}\n\t\t\t\tfarClip: {7}\n\t\t\t\tmouseSensitivity: "
          "{8}\n\t\t\t\ttranslationSpeed: {9}\n\t\t\t\tverticalSpeed: {10}\n\t\t\t\trotationSpeed: {11}",
          _type, _aspectRatio, fov, zoomFactor, zoomMin, zoomMax, nearClip, farClip, mouseSensitivity, translationSpeed,
          verticalSpeed, rotationSpeed);
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

    void calculateProjection() {
      if (type == CameraType::Perspective) {
        zoomFactor = std::clamp(zoomFactor, zoomMin, zoomMax);
        projection = glm::perspective(glm::radians(fov * 1.f / zoomFactor), aspectRatioValue, nearClip, farClip);
      } else if (type == CameraType::Orthographic) {
        projection = glm::ortho(-aspectRatioValue * zoomFactor, aspectRatioValue * zoomFactor, -zoomFactor, zoomFactor,
                                nearClip, farClip);
      }
    }

    void calculateView(const glm::vec3& position, const glm::quat& rotation) {
      glm::mat4 rotated = glm::mat4_cast(glm::conjugate(rotation));
      glm::mat4 translated = glm::translate(rotated, -position);
      view = std::move(translated);
    }
};
}
