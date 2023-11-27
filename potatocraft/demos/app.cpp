#include "demos/settings.h"
#include "demos/states/gameState.h"
#include "demos/utils.h"
#include "potatoengine/core/application.h"
#include "potatoengine/core/entrypoint.h"
#include "potatoengine/engineAPI.h"

namespace potatocraft {

class Demos : public engine::Application {
   public:
    Demos(engine::Config&& c, engine::CLArgs&& args) : engine::Application(std::move(c), std::move(args)) {
        m_assetsManager->load<engine::Scene>(DEFAULT_SCENE, DEFAULT_SCENE_PATH);
        CORE_INFO("Registering game components...");
        registerComponents();
        CORE_INFO("Loading game state...");
        pushState(GameState::Create(std::weak_ptr<engine::AssetsManager>(m_assetsManager), std::weak_ptr<engine::Renderer>(m_renderer)));
        CORE_INFO("Game state loaded!");
    }
};

}

engine::Application* engine::CreateApp(engine::CLArgs&& args) {
    CursorMode cursor_mode;
    if (CURSOR_MODE == 0) {
        cursor_mode = CursorMode::Normal;
    } else if (CURSOR_MODE == 1) {
        cursor_mode = CursorMode::Hidden;
    } else if (CURSOR_MODE == 2) {
        cursor_mode = CursorMode::Disabled;
    } else {
        throw std::runtime_error("Invalid cursor mode!");
    }
    engine::Log::GetLogger()->set_level(static_cast<spdlog::level::level_enum>(DEBUG_LEVEL));
    Config appConfig = {
        .name = "Potatocraft - Demos",
        .root = "..",
        .windowIconPath = WINDOW_ICON_PATH,
        .width = DISPLAY_WIDTH,
        .height = DISPLAY_HEIGHT,
        .depthBits = DEPTH_BITS,
        .refreshRate = REFRESH_RATE,
        .fullscreen = FULLSCREEN,
        .primaryMonitor = PRIMARY_MONITOR,
        .vSync = VSYNC,
        .resizable = RESIZABLE,
        .openGLMajorVersion = OPENGL_MAJOR_VERSION,
        .openGLMinorVersion = OPENGL_MINOR_VERSION,
        .cursorIconPath = CURSOR_ICON_PATH,
        .cursorMode = cursor_mode,
        .debugEnabled = DEBUG_ENABLED,
        .debugLevel = DEBUG_LEVEL,
        .displayFPS = DISPLAY_FPS,
    };
    return new potatocraft::Demos(std::move(appConfig), std::move(args));
}