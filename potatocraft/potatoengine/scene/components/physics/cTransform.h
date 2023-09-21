#pragma once

#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace potatoengine {

struct CTransform {
    glm::vec3 position{};
    glm::quat rotation{};
    glm::vec3 scale{glm::vec3{1.f}};

    CTransform() = default;
    explicit CTransform(glm::vec3&& p, glm::quat&& r, glm::vec3&& s) : position(std::move(p)), rotation(std::move(r)), scale(std::move(s)) {}

    glm::mat4 calculate() const {
        // T * R * S
        return glm::scale(glm::translate(glm::mat4(1.f), position) * glm::mat4_cast(rotation), scale);
    }

    void rotate(float angle, const glm::vec3& axis) {
        rotation = glm::angleAxis(glm::radians(angle), axis) * rotation;
    }

    void rotate(const glm::quat& q) {
        rotation = q * rotation;
    }

    void print() const {
        CORE_INFO("\t\tposition: {0}\n\t\t\t\trotation: {1}\n\t\t\t\tscale: {2}", glm::to_string(position), glm::to_string(rotation), glm::to_string(scale));
    }


};
}