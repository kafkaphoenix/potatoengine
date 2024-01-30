#pragma once

#define GLM_FORCE_CTOR_INIT

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CLight {
    enum class Type { // https://docs.unity3d.com/Manual/Lighting.html
      Point,
      Spot,
      Directional,
      Area,
    };

    std::string _type;
    Type type;
    glm::vec3 color{glm::vec3(1.f)};
    float intensity{};
    float range{};
    float innerConeAngle{};
    float outerConeAngle{};

    CLight() = default;
    explicit CLight(std::string&& t) : _type(std::move(t)) {}
    explicit CLight(std::string&& t, glm::vec3&& c, float i, float r, float ica,
                    float oca)
      : _type(std::move(t)), color(std::move(c)), intensity(i), range(r),
        innerConeAngle(ica), outerConeAngle(oca) {}

    void print() const {
      ENGINE_BACKTRACE(
        "\t\ttype: {0}\n\t\t\t\t\t\tcolor: {1}\n\t\t\t\t\t\tintensity: "
        "{2}\n\t\t\t\t\t\trange: "
        "{3}\n\t\t\t\t\t\tinnerConeAngle: {4}\n\t\t\t\t\t\touterConeAngle: {5}",
        _type, glm::to_string(color), intensity, range, innerConeAngle,
        outerConeAngle);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["type"] = _type;
      info["color"] = glm::to_string(color);
      info["intensity"] = std::to_string(intensity);
      info["range"] = std::to_string(range);
      info["innerConeAngle"] = std::to_string(innerConeAngle);
      info["outerConeAngle"] = std::to_string(outerConeAngle);

      return info;
    }

    void setLightType() {
      if (_type == "directional") {
        type = Type::Directional;
      } else if (_type == "point") {
        type = Type::Point;
      } else if (_type == "spot") {
        type = Type::Spot;
      } else if (_type == "area") {
        type = Type::Area;
      } else {
        ENGINE_ASSERT(false, "Unknown light type {}", _type);
      }
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e, CLight& c) {
  c.setLightType();

  m_registry.replace<CLight>(e, c);
}