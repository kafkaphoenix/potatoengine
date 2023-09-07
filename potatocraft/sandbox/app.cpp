#include "potatoengine/core/entrypoint.h"
#include "potatoengine/engineAPI.h"
#include "sandbox/gameState.h"

namespace potatocraft {

class Sandbox : public engine::Application {
   public:
    Sandbox(const engine::Config& c, engine::CLArgs args) : engine::Application(c, args) {
#ifdef DEBUG
        CORE_INFO("Loading assets...");  // TODO: Move this to a loading screen state
#endif
        m_assetsManager->load<engine::Shader>("vbasic", "assets/shaders/basic.vert");
        m_assetsManager->load<engine::Shader>("fbasic", "assets/shaders/basic.frag");
        m_assetsManager->load<engine::Texture>(engine::assets::TextureID::Cube, "assets/textures/wooden_block.jpg");
        m_assetsManager->load<engine::Texture>(engine::assets::TextureID::Default, "assets/textures/default.jpg");
        m_assetsManager->load<engine::Prefab>(engine::assets::PrefabID::Player, "assets/prefabs/entities.json", "player");
        m_assetsManager->load<engine::Prefab>(engine::assets::PrefabID::Chicken, "assets/prefabs/entities.json", "chicken");
        m_assetsManager->load<engine::Prefab>(engine::assets::PrefabID::BrickBlock, "assets/prefabs/entities.json", "brick_block");
        m_assetsManager->load<engine::Prefab>(engine::assets::PrefabID::StoneBlock, "assets/prefabs/entities.json", "stone_block");
        auto m_timer = engine::Timer();
        m_assetsManager->load<engine::Model>("assets/models/rock/rock.obj", "assets/models/rock/rock.obj");
        CORE_INFO("Loading models TIME: {0}", m_timer.elapsedSeconds());
        m_timer = engine::Timer();
        m_assetsManager->load<engine::Model>("assets/models/cube/cube.obj", "assets/models/cube/cube.obj");
        CORE_INFO("Loading models TIME: {0}", m_timer.elapsedSeconds());
#ifdef DEBUG
        CORE_INFO("Assets loaded!");
        m_assetsManager->print();
        CORE_INFO("Loading game state...");
#endif
        pushState(GameState::Create(std::weak_ptr<engine::AssetsManager>(m_assetsManager), std::weak_ptr<engine::Renderer>(m_renderer)));

#ifdef DEBUG
        CORE_INFO("Game state loaded!");
#endif
    }
};

}

engine::Application* engine::CreateApp(engine::CLArgs args) {
    return new potatocraft::Sandbox(Config{.name = "PotatoCraft", .root = "..", .width = 1280, .height = 720}, args);
}