#include "core/application.h"
#include "core/entrypoint.h"
#include "core/settingsManager.h"
#include "engineAPI.h"
#include "serializers/sSettings.h"
#include "states/gameState.h"
#include "states/menuState.h"
#include "utils.h"
#include "utils/getDefaultRoamingPath.h"

namespace demos {

class Demos : public engine::Application {
  public:
    Demos(std::unique_ptr<engine::SettingsManager>&& s, engine::CLArgs&& args)
      : engine::Application(std::move(s), std::move(args)) {
      engine::RenderAPI::SetClearColor(m_settings_manager->clearColor);
      engine::RenderAPI::SetClearDepth(m_settings_manager->clearDepth);
      APP_TRACE("Registering app components...");
      RegisterComponents();
      APP_TRACE("Loading initial state...");
      if (m_settings_manager->activeScene == "Flappy Bird") {
        m_states_manager->pushState(states::MenuState::Create());
      } else {
        m_states_manager->pushState(states::GameState::Create());
      }
      APP_TRACE("State loaded!");
    }

    ~Demos() override { APP_WARN("Deleting Demos application"); }
};

}

engine::Application* engine::CreateApp(engine::CLArgs&& args) {
  auto settings_manager = serializers::load_settings("Demos");
  LogManager::SetEngineLoggerLevel(
    static_cast<spdlog::level::level_enum>(settings_manager->engineLogLevel));
  LogManager::SetEngineLoggerFlushLevel(
    static_cast<spdlog::level::level_enum>(settings_manager->engineFlushLevel));
  LogManager::SetAppLoggerLevel(
    static_cast<spdlog::level::level_enum>(settings_manager->appLogLevel));
  LogManager::SetAppLoggerFlushLevel(
    static_cast<spdlog::level::level_enum>(settings_manager->appFlushLevel));
  LogManager::ToggleEngineLogger(settings_manager->enableEngineLogger);
  LogManager::ToggleAppLogger(settings_manager->enableAppLogger);

  if (not settings_manager->logFilePath.empty()) {
    LogManager::CreateFileLogger(
      (get_default_roaming_path("Demos") / settings_manager->logFilePath).string());
  }

  if (not settings_manager->backtraceLogFilePath.empty()) {
    LogManager::CreateBacktraceLogger(
      (get_default_roaming_path("Demos") /settings_manager->backtraceLogFilePath).string(),
      settings_manager->enableEngineBacktraceLogger,
      settings_manager->enableAppBacktraceLogger);
  }

  APP_INFO("Loading settings...");
  APP_INFO("Initializating Demos application");
  return new demos::Demos(std::move(settings_manager), std::move(args));
}