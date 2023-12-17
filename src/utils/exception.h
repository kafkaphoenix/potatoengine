#pragma once

#include <format>
#include <sstream>
#include <stdexcept>
#include <string>

namespace potatoengine {

class CustomException : public std::runtime_error {
  public:
    CustomException(const std::string& arg, const char* file, int line)
      : std::runtime_error(arg) {
      std::ostringstream o;
      o << file << ":" << line << ": " << arg;
      m_msg = o.str();
    }

    ~CustomException() throw() {}

    const char* what() const throw() { return m_msg.c_str(); }

  private:
    std::string m_msg;
};

class EngineException : public CustomException {
  public:
    EngineException(const std::string& arg, const char* file, int line)
      : CustomException(arg, file, line) {}
};

class AppException : public CustomException {
  public:
    AppException(const std::string& arg, const char* file, int line)
      : CustomException(arg, file, line) {}
};

}

#define throw_engine_exception(arg)                                            \
  throw potatoengine::EngineException(std::format(arg), __FILE__, __LINE__)
#define throw_app_exception(arg)                                               \
  throw potatoengine::AppException(std::format(arg), __FILE__, __LINE__)
