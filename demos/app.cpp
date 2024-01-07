#include "core/application.h"
#include "core/entrypoint.h"
#include "engineAPI.h"
#include "serializers/sSettings.h"
#include "settings.h"
#include "states/gameState.h"
#include "utils.h"

namespace demos {

class Demos : public engine::Application {
  public:
    Demos(std::unique_ptr<engine::Settings>&& s, engine::CLArgs&& args)
      : engine::Application(std::move(s), std::move(args)) {
      APP_TRACE("Registering app components...");
      RegisterComponents();
      APP_TRACE("Loading initial state...");
      pushState(GameState::Create(m_settings));
      APP_TRACE("State loaded!");
    }

    ~Demos() override { APP_WARN("Deleting Demos application"); }
};

}

engine::Application* engine::CreateApp(engine::CLArgs&& args) {
  auto settings = serializers::load_settings("Demos");
  LogManager::SetEngineLoggerLevel(
    static_cast<spdlog::level::level_enum>(settings->engineLogLevel));
  LogManager::SetEngineLoggerFlushLevel(
    static_cast<spdlog::level::level_enum>(settings->engineFlushLevel));
  LogManager::SetAppLoggerLevel(
    static_cast<spdlog::level::level_enum>(settings->appLogLevel));
  LogManager::SetAppLoggerFlushLevel(
    static_cast<spdlog::level::level_enum>(settings->appFlushLevel));
  LogManager::ToggleEngineLogger(settings->enableEngineLogger);
  LogManager::ToggleAppLogger(settings->enableAppLogger);

  if (not settings->logFilePath.empty()) {
    LogManager::CreateFileLogger(settings->root + "/" + settings->logFilePath);
  }

  if (not settings->backtraceLogFilePath.empty()) {
    LogManager::CreateBacktraceLogger(settings->backtraceLogFilePath,
                                      settings->enableEngineBacktraceLogger,
                                      settings->enableAppBacktraceLogger);
  }
  LogManager::ToggleEngineBacktraceLogger(
    settings->enableEngineBacktraceLogger);
  LogManager::ToggleAppBacktraceLogger(settings->enableAppBacktraceLogger);

  APP_INFO("Loading settings...");
  APP_INFO("Initializating Demos application");
  return new demos::Demos(std::move(settings), std::move(args));
}