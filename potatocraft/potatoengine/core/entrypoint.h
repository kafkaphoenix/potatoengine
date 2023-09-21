#pragma once

#include "potatoengine/core/application.h"

namespace engine = potatoengine;

extern engine::Application *engine::CreateApp(engine::CLArgs&& args);

int main(int argc, char **argv) {
    try {
        engine::Log::Init();

        std::vector<const char*> args(argv, argv + argc);
        engine::CLArgs clargs{std::span<const char*>{args}};
        engine::Application* app = engine::CreateApp(std::move(clargs));

        app->run();

        delete app;
    } catch (const std::exception &ex) {
        CORE_CRITICAL(ex.what());
    }
}