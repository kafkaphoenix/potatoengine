#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "utils/numericComparator.h"

namespace potatoengine {

struct CSkybox {
    bool useFog{};
    glm::vec3 fogColor{};
    float fogDensity{};
    float fogGradient{};
    float rotationSpeed{};

    CSkybox() = default;
    explicit CSkybox(bool uf, glm::vec3&& fc, float fd, float fg, float rs)
      : useFog(uf), fogColor(std::move(fc)), fogDensity(fd), fogGradient(fg),
        rotationSpeed(rs) {}

    void print() const {
      ENGINE_BACKTRACE("\t\tuseFog: {0}\n\t\t\t\tfogColor: "
                       "{1}\n\t\t\t\tfogDensity: {2}\n\t\t\t\tfogGradient: "
                       "{3}\n\t\t\t\trotationSpeed: {4}",
                       useFog, glm::to_string(fogColor), fogDensity,
                       fogGradient, rotationSpeed);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["useFog"] = useFog ? "true" : "false";
      info["fogColor"] = glm::to_string(fogColor);
      info["fogDensity"] = std::to_string(fogDensity);
      info["fogGradient"] = std::to_string(fogGradient);
      info["rotationSpeed"] = std::to_string(rotationSpeed);

      return info;
    }
};
}