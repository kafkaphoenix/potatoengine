#include "assets/shader.h"

#include "pch.h"

namespace potatoengine {
Shader::Shader(std::filesystem::path&& fp)
  : m_filepath(std::move(fp.string())) {
  std::ifstream f(fp);
  ENGINE_ASSERT(f.is_open(), "Failed to open shader file!");
  ENGINE_ASSERT(f.peek() not_eq std::ifstream::traits_type::eof(),
                "Shader file is empty!");
  std::string data(std::istreambuf_iterator<char>(f), {});
  f.close();

  m_type = fp.extension() == ".vert" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
  m_id = glCreateShader(m_type);

  const GLchar* source = data.data();
  glShaderSource(m_id, 1, &source, 0);
  glCompileShader(m_id);

  int32_t status = GL_FALSE;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
  if (status not_eq GL_TRUE) [[unlikely]] {
    int infoLogLength = 0;
    glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
    ENGINE_ASSERT(infoLogLength > 0, "Shader {} compilation failed!",
                  m_filepath);
    std::vector<GLchar> shaderInfoLog(infoLogLength);
    glGetShaderInfoLog(m_id, infoLogLength, &infoLogLength,
                       shaderInfoLog.data());
    glDeleteShader(m_id);
    ENGINE_ASSERT(false, "Shader {} compilation failed: \n{}", m_filepath,
                  std::string(shaderInfoLog.data()));
  }
}

Shader::~Shader() {
  ENGINE_WARN("Deleting shader {}", m_filepath);
  glDeleteShader(m_id);
}

const std::map<std::string, std::string, NumericComparator>& Shader::getInfo() {
  if (not m_info.empty()) {
    return m_info;
  }

  m_info["Type"] = "Shader";
  m_info["Filepath"] = m_filepath;
  m_info["ID"] = std::to_string(m_id);
  if (m_type == GL_VERTEX_SHADER) {
    m_info["Shader type"] = "Vertex";
  } else if (m_type == GL_FRAGMENT_SHADER) {
    m_info["Shader type"] = "Fragment";
  } else if (m_type == GL_GEOMETRY_SHADER) {
    m_info["Shader type"] = "Geometry";
  } else if (m_type == GL_TESS_CONTROL_SHADER) {
    m_info["Shader type"] = "Tessellation Control";
  } else if (m_type == GL_TESS_EVALUATION_SHADER) {
    m_info["Shader type"] = "Tessellation Evaluation";
  } else if (m_type == GL_COMPUTE_SHADER) {
    m_info["Shader type"] = "Compute";
  } else {
    m_info["Shader type"] = "Unknown";
  }

  return m_info;
}
}