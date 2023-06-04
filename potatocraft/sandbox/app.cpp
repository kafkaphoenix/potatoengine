#pragma once

#include "potatoengine/core/entrypoint.h"
#include "potatoengine/engineAPI.h"
#include "sandbox/gameState.h"

namespace potatocraft {

class Sandbox : public potatoengine::Application {
   public:
    Sandbox(const std::string &name, potatoengine::CommandLineArgs args) : potatoengine::Application(name, args) {
        CORE_INFO("Loading assets...");
        m_assetsManager->load<potatoengine::Shader>(potatoengine::asset::shader::ID::VBasic, "..\\assets\\shaders\\basic.vert");
        m_assetsManager->load<potatoengine::Shader>(potatoengine::asset::shader::ID::FBasic, "..\\assets\\shaders\\basic.frag");
        m_assetsManager->load<potatoengine::Texture>(potatoengine::asset::texture::ID::Cube, "..\\assets\\textures\\wall.jpg");
        CORE_INFO("Assets loaded!");

        CORE_INFO("Loading game state...");
        pushState(GameState::Create(m_assetsManager));
        CORE_INFO("Game state loaded!");
    }
};

}

potatoengine::Application *potatoengine::CreateApp(const std::string &name, potatoengine::CommandLineArgs args) {
    return new potatocraft::Sandbox(name, args);
}