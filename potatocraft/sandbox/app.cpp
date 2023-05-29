#pragma once

#include "potatoengine/core/entrypoint.h"
#include "potatoengine/engineAPI.h"
#include "sandbox/gameState.h"

namespace potatocraft {

class Sandbox : public potatoengine::Application {
   public:
    Sandbox(const std::string &name, potatoengine::CommandLineArgs args) : potatoengine::Application(name, args) {
        pushState(new GameState());
    }

    ~Sandbox() {
    }
};

}

potatoengine::Application *potatoengine::CreateApp(const std::string &name, potatoengine::CommandLineArgs args) {
    return new potatocraft::Sandbox(name, args);
}