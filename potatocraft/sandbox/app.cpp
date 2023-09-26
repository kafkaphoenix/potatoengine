#include "potatoengine/core/entrypoint.h"
#include "potatoengine/engineAPI.h"
#include "sandbox/states/gameState.h"
#include "sandbox/utils.h"

namespace potatocraft {

class Sandbox : public engine::Application {
   public:
    Sandbox(engine::Config&& c, engine::CLArgs&& args) : engine::Application(std::move(c), std::move(args)) {
        m_assetsManager->load<engine::Scene>("cubes", "assets/scenes/cubes.json");
#ifdef DEBUG
        CORE_INFO("Registering game components...");
#endif
        registerComponents();
#ifdef DEBUG
        CORE_INFO("Loading game state...");
#endif
        pushState(GameState::Create(std::weak_ptr<engine::AssetsManager>(m_assetsManager), std::weak_ptr<engine::Renderer>(m_renderer)));
#ifdef DEBUG
        CORE_INFO("Game state loaded!");
#endif
    }
};

}

engine::Application* engine::CreateApp(engine::CLArgs&& args) {
    return new potatocraft::Sandbox(Config{.name = "PotatoCraft", .root = "..", .width = 1280, .height = 720}, std::move(args));
}