#include "core/logManager.h"

#pragma warning(push, 0)
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

#include "imgui/imguiLogSink.h"

namespace potatoengine {

void LogManager::Init() {
  std::vector<spdlog::sink_ptr> logSinks;
  logSinks.reserve(2);
  logSinks.emplace_back(
    std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  logSinks.emplace_back(std::make_shared<ImGuiLogSink>());

  logSinks[0]->set_pattern("%^[%T] %n: %v%$");

  s_engineLogger =
    std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
  spdlog::register_logger(s_engineLogger);
  s_engineLogger->set_level(s_engineLogLevel);
  s_engineLogger->flush_on(s_engineFlushLevel);

  s_appLogger =
    std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
  spdlog::register_logger(s_appLogger);
  s_appLogger->set_level(s_appLogLevel);
  s_appLogger->flush_on(s_appFlushLevel);
}

void LogManager::CreateFileLogger(std::string_view filepath) {
  auto fileSink =
    std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath.data(), true);

  fileSink->set_pattern("[%D %T] [%l] %n: %v");

  s_engineLogger->sinks().emplace_back(fileSink);
  s_appLogger->sinks().emplace_back(fileSink);
}

void LogManager::CreateBacktraceLogger(std::string_view filepath,
                                       bool enableEngineBacktraceLogger,
                                       bool enableAppBacktraceLogger) {
  auto backtraceSink = std::make_shared<BacktraceLogSink>(filepath.data());
  s_engineBacktraceLogger =
    std::make_shared<spdlog::logger>("ENGINE", backtraceSink);
  s_appBacktraceLogger = std::make_shared<spdlog::logger>("APP", backtraceSink);

  if (enableEngineBacktraceLogger) {
    s_engineBacktraceLogger->set_level(spdlog::level::debug);
    s_engineBacktraceLogger->flush_on(spdlog::level::debug);
  } else {
    s_engineBacktraceLogger->set_level(spdlog::level::off);
    s_engineBacktraceLogger->flush_on(spdlog::level::off);
  }

  if (enableAppBacktraceLogger) {
    s_appBacktraceLogger->set_level(spdlog::level::debug);
    s_appBacktraceLogger->flush_on(spdlog::level::debug);
  } else {
    s_appBacktraceLogger->set_level(spdlog::level::off);
    s_appBacktraceLogger->flush_on(spdlog::level::off);
  }
}

void LogManager::ClearAllBacktraceLogger() {
  auto sink =
    dynamic_cast<BacktraceLogSink*>(s_engineBacktraceLogger->sinks()[0].get());
  sink->Clear("all");
}

void LogManager::ClearEngineBacktraceLogger() {
  auto sink =
    dynamic_cast<BacktraceLogSink*>(s_engineBacktraceLogger->sinks()[0].get());
  sink->Clear("ENGINE");
}

void LogManager::ClearAppBacktraceLogger() {
  auto sink =
    dynamic_cast<BacktraceLogSink*>(s_appBacktraceLogger->sinks()[0].get());
  sink->Clear("APP");
}

void LogManager::DumpBacktrace() {
  auto sink =
    dynamic_cast<BacktraceLogSink*>(s_engineBacktraceLogger->sinks()[0].get());
  sink->DumpToFile();
}

void LogManager::SetEngineLoggerLevel(spdlog::level::level_enum level) {
  s_engineLogLevel = level;
  s_engineLogger->set_level(s_engineLogLevel);
}

void LogManager::SetAppLoggerLevel(spdlog::level::level_enum level) {
  s_appLogLevel = level;
  s_appLogger->set_level(s_appLogLevel);
}

void LogManager::SetEngineLoggerFlushLevel(spdlog::level::level_enum level) {
  s_engineFlushLevel = level;
  s_engineLogger->flush_on(level);
}

void LogManager::SetAppLoggerFlushLevel(spdlog::level::level_enum level) {
  s_appFlushLevel = level;
  s_appLogger->flush_on(level);
}

void LogManager::ToggleEngineLogger(bool enable) {
  if (enable and not IsEngineLoggerEnabled()) {
    s_engineLogger->set_level(s_engineLogLevel);
    s_engineLogger->flush_on(s_engineFlushLevel);
  } else if (not enable and IsEngineLoggerEnabled()) {
    s_engineLogger->set_level(spdlog::level::off);
    s_engineLogger->flush_on(spdlog::level::off);
  }
}

void LogManager::ToggleAppLogger(bool enable) {
  if (enable and not IsAppLoggerEnabled()) {
    s_appLogger->set_level(s_appLogLevel);
    s_appLogger->flush_on(s_appFlushLevel);
  } else if (not enable and IsAppLoggerEnabled()) {
    s_appLogger->set_level(spdlog::level::off);
    s_appLogger->flush_on(spdlog::level::off);
  }
}

void LogManager::ToggleEngineBacktraceLogger(bool enable) {
  if (enable and not IsEngineBacktraceLoggerEnabled()) {
    s_engineBacktraceLogger->set_level(spdlog::level::debug);
    s_engineBacktraceLogger->flush_on(spdlog::level::debug);
  } else if (not enable and IsEngineBacktraceLoggerEnabled()) {
    s_engineBacktraceLogger->set_level(spdlog::level::off);
    s_engineBacktraceLogger->flush_on(spdlog::level::off);
  }
}

void LogManager::ToggleAppBacktraceLogger(bool enable) {
  if (enable and not IsAppBacktraceLoggerEnabled()) {
    s_appBacktraceLogger->set_level(spdlog::level::debug);
    s_appBacktraceLogger->flush_on(spdlog::level::debug);
  } else if (not enable and IsAppBacktraceLoggerEnabled()) {
    s_appBacktraceLogger->set_level(spdlog::level::off);
    s_appBacktraceLogger->flush_on(spdlog::level::off);
  }
}

}