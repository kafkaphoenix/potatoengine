#pragma once

#include "src/engineAPI.h"
#include "src/core/entrypoint.h"

#include "src/sandbox/gameState.h"

namespace potatocraft {

    class Sandbox : public potatoengine::Application
    {
        public:
            Sandbox(const std::string &name, potatoengine::CommandLineArgs args): potatoengine::Application(name, args) {
                pushState(new GameState());
            }

            ~Sandbox() {

            }
    };

}

potatoengine::Application* potatoengine::CreateApp(const std::string &name, potatoengine::CommandLineArgs args) {
    return new potatocraft::Sandbox(name, args);
}