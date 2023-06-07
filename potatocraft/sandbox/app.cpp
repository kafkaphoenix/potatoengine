#pragma once

#include "potatoengine/core/entrypoint.h"
#include "potatoengine/engineAPI.h"
#include "sandbox/gameState.h"

namespace potatocraft {

class Sandbox : public potatoengine::Application {
   public:
    Sandbox(const potatoengine::Config& cfg, potatoengine::CLArgs args) : potatoengine::Application(cfg, args) {
        CORE_INFO("Loading assets..."); // TODO: Move this to a loading screen state
        m_assetsManager->load<potatoengine::Shader>(potatoengine::asset::shader::ID::VBasic, "assets\\shaders\\basic.vert");
        m_assetsManager->load<potatoengine::Shader>(potatoengine::asset::shader::ID::FBasic, "assets\\shaders\\basic.frag");
        m_assetsManager->load<potatoengine::Texture>(potatoengine::asset::texture::ID::Cube, "assets\\textures\\brick_block.jpg");
        m_assetsManager->load<potatoengine::Prefab>(potatoengine::asset::prefab::ID::Player, "assets\\prefabs\\entities.json", "player");
        m_assetsManager->load<potatoengine::Prefab>(potatoengine::asset::prefab::ID::Chicken, "assets\\prefabs\\entities.json", "chicken");
        m_assetsManager->load<potatoengine::Prefab>(potatoengine::asset::prefab::ID::BrickBlock, "assets\\prefabs\\entities.json", "brick_block");
        CORE_INFO("Assets loaded!");

        CORE_INFO("Loading game state...");
        pushState(GameState::Create(m_assetsManager));
        CORE_INFO("Game state loaded!");
    }
};

}

potatoengine::Application* potatoengine::CreateApp(potatoengine::CLArgs args) {
    return new potatocraft::Sandbox(Config{.name = "PotatoCraft", .root = "..", .width = 1280, .height = 720}, args);
}