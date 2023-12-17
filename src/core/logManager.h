#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include "core/backtraceLogSink.h"

namespace potatoengine {

class LogManager {
  public:
    static void Init();
    static void CreateFileLogger(std::string_view filepath);
    static void CreateBacktraceLogger(std::string_view filepath,
                                      bool enableEngineBacktraceLogger,
                                      bool enableAppBacktraceLogger);
    static void clearAllBacktraceLogger();
    static void clearEngineBacktraceLogger();
    static void clearAppBacktraceLogger();
    static void DumpBacktrace();
    static void SetEngineLoggerLevel(spdlog::level::level_enum level);
    static void SetAppLoggerLevel(spdlog::level::level_enum level);
    static void SetEngineLoggerFlushLevel(spdlog::level::level_enum level);
    static void SetAppLoggerFlushLevel(spdlog::level::level_enum level);
    static void toggleEngineLogger(bool enable);
    static void toggleAppLogger(bool enable);
    static void toggleEngineBacktraceLogger(bool enable);
    static void toggleAppBacktraceLogger(bool enable);
    static std::shared_ptr<spdlog::logger>& GetEngineLogger() noexcept {
      return s_engineLogger;
    }
    static std::shared_ptr<spdlog::logger>& GetAppLogger() noexcept {
      return s_appLogger;
    }
    static std::shared_ptr<spdlog::logger>&
    GetEngineBacktraceLogger() noexcept {
      return s_engineBacktraceLogger;
    }
    static std::shared_ptr<spdlog::logger>& GetAppBacktraceLogger() noexcept {
      return s_appBacktraceLogger;
    }
    static std::string_view getEngineLoggerLevel() {
      return std::string_view(
        spdlog::level::to_string_view(s_engineLogger->level()));
    }
    static std::string_view getAppLoggerLevel() {
      return std::string_view(
        spdlog::level::to_string_view(s_appLogger->level()));
    }
    static std::string_view getEngineLoggerFlushLevel() {
      return std::string_view(
        spdlog::level::to_string_view(s_engineLogger->flush_level()));
    }
    static std::string_view getAppLoggerFlushLevel() {
      return std::string_view(
        spdlog::level::to_string_view(s_appLogger->flush_level()));
    }
    static bool isEngineLoggerEnabled() {
      return s_engineLogger->level() != spdlog::level::off;
    }
    static bool isAppLoggerEnabled() {
      return s_appLogger->level() != spdlog::level::off;
    }
    static bool isEngineBacktraceLoggerEnabled() {
      return s_engineBacktraceLogger->level() != spdlog::level::off;
    }
    static bool isAppBacktraceLoggerEnabled() {
      return s_appBacktraceLogger->level() != spdlog::level::off;
    }

  private:
    inline static std::shared_ptr<spdlog::logger> s_engineLogger;
    inline static std::shared_ptr<spdlog::logger> s_appLogger;
    inline static std::shared_ptr<spdlog::logger> s_engineBacktraceLogger;
    inline static std::shared_ptr<spdlog::logger> s_appBacktraceLogger;
    inline static enum ::spdlog::level::level_enum s_engineLogLevel =
      spdlog::level::trace;
    inline static enum ::spdlog::level::level_enum s_appLogLevel =
      spdlog::level::trace;
    inline static enum ::spdlog::level::level_enum s_engineFlushLevel =
      spdlog::level::trace;
    inline static enum ::spdlog::level::level_enum s_appFlushLevel =
      spdlog::level::trace;
};

}

namespace engine = potatoengine;

#define ENGINE_TRACE(...)                                                      \
  ::engine::LogManager::GetEngineLogger()->trace(__VA_ARGS__)
#define ENGINE_DEBUG(...)                                                      \
  ::engine::LogManager::GetEngineLogger()->debug(__VA_ARGS__)
#define ENGINE_INFO(...)                                                       \
  ::engine::LogManager::GetEngineLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)                                                       \
  ::engine::LogManager::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)                                                      \
  ::engine::LogManager::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...)                                                   \
  ::engine::LogManager::GetEngineLogger()->critical(__VA_ARGS__)

#define APP_TRACE(...) ::engine::LogManager::GetAppLogger()->trace(__VA_ARGS__)
#define APP_DEBUG(...) ::engine::LogManager::GetAppLogger()->debug(__VA_ARGS__)
#define APP_INFO(...) ::engine::LogManager::GetAppLogger()->info(__VA_ARGS__)
#define APP_WARN(...) ::engine::LogManager::GetAppLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...) ::engine::LogManager::GetAppLogger()->error(__VA_ARGS__)
#define APP_CRITICAL(...)                                                      \
  ::engine::LogManager::GetAppLogger()->critical(__VA_ARGS__)

#define ENGINE_BACKTRACE(...)                                                  \
  ::engine::LogManager::GetEngineBacktraceLogger()->debug(__VA_ARGS__)
#define APP_BACKTRACE(...)                                                     \
  ::engine::LogManager::GetAppBacktraceLogger()->debug(__VA_ARGS__)
