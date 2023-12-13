#pragma once

#include "pch.h"
#include "utils/numericComparator.h"
#include <glad/glad.h>

namespace potatoengine {

class Shader {
  public:
    Shader(std::filesystem::path&& fp);
    ~Shader();
    Shader& operator=(const Shader&) = delete;

    const std::map<std::string, std::string, NumericComparator>& getInfo();

    bool operator==(const Shader& other) const noexcept { return m_filepath == other.m_filepath; }
    operator GLuint() const noexcept { return m_id; }

  private:
    std::string m_filepath{};
    uint32_t m_id{};
    GLenum m_type{};

    std::map<std::string, std::string, NumericComparator> m_info{};
};

}