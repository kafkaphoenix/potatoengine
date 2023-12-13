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
    Demos(engine::Config&& c, engine::CLArgs&& args) : engine::Application(std::move(c), std::move(args)) {
      APP_INFO("Registering app components...");
      registerComponents();
      APP_INFO("Loading initial state...");
      pushState(GameState::Create(std::weak_ptr<engine::AssetsManager>(m_assetsManager),
                                  std::weak_ptr<engine::Renderer>(m_renderer), serializers::load_settings()));
      APP_INFO("State loaded!");
    }

    ~Demos() override { APP_WARN("Deleting Demos application"); }
};

}

engine::Application* engine::CreateApp(engine::CLArgs&& args) {
  demos::Settings settings = demos::serializers::load_settings();
  LogManager::GetEngineLogger()->set_level(static_cast<spdlog::level::level_enum>(settings.debugLevel));
  LogManager::GetEngineLogger()->flush_on(static_cast<spdlog::level::level_enum>(settings.debugLevel));
  LogManager::GetAppLogger()->set_level(static_cast<spdlog::level::level_enum>(settings.debugLevel));
  LogManager::GetAppLogger()->flush_on(static_cast<spdlog::level::level_enum>(settings.debugLevel));

  if (not settings.logFilePath.empty()) {
    LogManager::CreateFileLogger(settings.root + "/" + settings.logFilePath);
  }

  APP_INFO("Loading settings...");
  CursorMode cursorMode;
  if (settings.cursorMode == 0) {
    cursorMode = CursorMode::Normal;
  } else if (settings.cursorMode == 1) {
    cursorMode = CursorMode::Hidden;
  } else if (settings.cursorMode == 2) {
    cursorMode = CursorMode::Disabled;
  } else {
    APP_ASSERT(false, "Invalid cursor mode!");
  }

  Config appConfig = {.name = settings.appName,
                      .root = settings.root,
                      .windowIconPath = settings.windowIconPath,
                      .windowWidth = settings.windowWidth,
                      .windowHeight = settings.windowHeight,
                      .depthBits = settings.depthBits,
                      .refreshRate = settings.refreshRate,
                      .fullscreen = settings.fullscreen,
                      .primaryMonitor = settings.primaryMonitor,
                      .vSync = settings.vSync,
                      .resizable = settings.resizable,
                      .openglMajorVersion = settings.openglMajorVersion,
                      .openglMinorVersion = settings.openglMinorVersion,
                      .cursorIconPath = settings.cursorIconPath,
                      .cursorMode = cursorMode};

  APP_INFO("Creating Demos application");
  return new demos::Demos(std::move(appConfig), std::move(args));
}