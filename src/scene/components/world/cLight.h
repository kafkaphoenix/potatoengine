#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>

#include "scene/entity.h"

namespace potatoengine {

struct CLight {
    enum class Type { // https://docs.unity3d.com/Manual/Lighting.html
      Point,
      Spot,
      Directional,
      Area,
    };

    std::string _type{};
    Type type;
    glm::vec3 color{glm::vec3(1.f)};
    float intensity{};
    float range{};
    float innerConeAngle{};
    float outerConeAngle{};

    CLight() = default;
    explicit CLight(std::string&& t) : _type(std::move(t)) {}
    explicit CLight(std::string&& t, glm::vec3&& c, float i, float r, float ica, float oca)
        : _type(std::move(t)), color(std::move(c)), intensity(i), range(r), innerConeAngle(ica), outerConeAngle(oca) {}

    void print() const {
      ENGINE_TRACE("\t\ttype: {0}\n\t\t\t\tcolor: {1}\n\t\t\t\tintensity: {2}\n\t\t\t\trange: "
                   "{3}\n\t\t\t\tinnerConeAngle: {4}\n\t\t\t\touterConeAngle: {5}",
                   _type, color, intensity, range, innerConeAngle, outerConeAngle);
    }
};
}

template <> void engine::SceneManager::onComponentAdded(Entity& e, CLight& c) {
  if (c._type == "directional") {
    c.type = CLight::Type::Directional;
  } else if (c._type == "point") {
    c.type = CLight::Type::Point;
  } else if (c._type == "spot") {
    c.type = CLight::Type::Spot;
  } else if (c._type == "area") {
    c.type = CLight::Type::Area;
  } else {
    ENGINE_ASSERT(false, "Unknown light type {}", c._type);
  }

  e.update<CLight>(c);
}