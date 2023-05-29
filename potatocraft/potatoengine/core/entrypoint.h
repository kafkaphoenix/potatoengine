#pragma once
#include "potatoengine/core/application.h"

extern potatoengine::Application *potatoengine::CreateApp(
    const std::string &name, CommandLineArgs args);

int main(int argc, char **argv) {
    potatoengine::Log::Init();

    auto app = potatoengine::CreateApp("potatocraft", {argc, argv});

    app->run();

    delete app;
}