#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
// with SPDLOG_USE_STD_FORMAT this does not work and we need to go back to use glm::to_string everywhere
// cannot be defined before #include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <glm/gtx/string_cast.hpp>

namespace potatoengine {

class LogManager {
  public:
    static void Init();
    static void CreateFileLogger(std::string_view filepath);
    static std::shared_ptr<spdlog::logger>& GetEngineLogger() noexcept { return s_engineLogger; }
    static std::shared_ptr<spdlog::logger>& GetAppLogger() noexcept { return s_appLogger; }

  private:
    inline static std::shared_ptr<spdlog::logger> s_engineLogger;
    inline static std::shared_ptr<spdlog::logger> s_appLogger;
};

}

namespace engine = potatoengine;

#define ENGINE_TRACE(...) ::engine::LogManager::GetEngineLogger()->trace(__VA_ARGS__)
#define ENGINE_DEBUG(...) ::engine::LogManager::GetEngineLogger()->debug(__VA_ARGS__)
#define ENGINE_INFO(...) ::engine::LogManager::GetEngineLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...) ::engine::LogManager::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...) ::engine::LogManager::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...) ::engine::LogManager::GetEngineLogger()->critical(__VA_ARGS__)

#define APP_TRACE(...) ::engine::LogManager::GetAppLogger()->trace(__VA_ARGS__)
#define APP_DEBUG(...) ::engine::LogManager::GetAppLogger()->debug(__VA_ARGS__)
#define APP_INFO(...) ::engine::LogManager::GetAppLogger()->info(__VA_ARGS__)
#define APP_WARN(...) ::engine::LogManager::GetAppLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...) ::engine::LogManager::GetAppLogger()->error(__VA_ARGS__)
#define APP_CRITICAL(...) ::engine::LogManager::GetAppLogger()->critical(__VA_ARGS__)

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector) {
  return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix) {
  return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion) {
  return os << glm::to_string(quaternion);
}