#pragma once

#include "potatoengine/core/application.h"

extern potatoengine::Application *potatoengine::CreateApp(potatoengine::CLArgs args);

int main(int argc, char **argv) {
    try {
        potatoengine::Log::Init();

        std::vector<const char *> args(argv, argv + argc);
        potatoengine::CLArgs clargs{.args = std::span<const char *>{args}};
        auto app = potatoengine::CreateApp(clargs);

        app->run();

        delete app;
    } catch (const std::exception &ex) {
        CORE_CRITICAL(ex.what());
    }
}